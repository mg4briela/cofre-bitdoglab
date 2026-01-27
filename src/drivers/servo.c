#include "servo.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

static uint servo_gpio;
static uint slice_num;
static uint channel_num;
static bool pwm_active = false;

/* Converte ângulo (0–180) em pulso em microssegundos */
static uint32_t angle_to_pulse(uint angle)
{
    if (angle > 180) angle = 180;

    return (angle * (SERVO_MAX_PULSE_US - SERVO_MIN_PULSE_US)) / 180
           + SERVO_MIN_PULSE_US;
}

/* Inicializa apenas o GPIO */
void servo_init(uint gpio)
{
    servo_gpio = gpio;

    gpio_init(servo_gpio);
    gpio_set_dir(servo_gpio, GPIO_OUT);
    gpio_put(servo_gpio, 0);
}

/* Ativa PWM somente quando necessário */
static void servo_pwm_enable(void)
{
    if (pwm_active) return;

    gpio_set_function(servo_gpio, GPIO_FUNC_PWM);

    slice_num   = pwm_gpio_to_slice_num(servo_gpio);
    channel_num = pwm_gpio_to_channel(servo_gpio);

    pwm_config cfg = pwm_get_default_config();

    float div = (float)clock_get_hz(clk_sys) / 1000000.0f; // 1 µs por tick
    pwm_config_set_clkdiv(&cfg, div);
    pwm_config_set_wrap(&cfg, SERVO_PERIOD_US);

    pwm_init(slice_num, &cfg, true);
    pwm_active = true;
}

/* Desativa PWM (anti-jitter) */
void servo_disable(void)
{
    if (!pwm_active) return;

    pwm_set_enabled(slice_num, false);
    gpio_set_function(servo_gpio, GPIO_FUNC_SIO);
    gpio_put(servo_gpio, 0);

    pwm_active = false;
}

/* Define o ângulo do servo */
void servo_set_angle(uint angle)
{
    servo_pwm_enable();
    pwm_set_chan_level(slice_num, channel_num, angle_to_pulse(angle));
}

/* Movimento padrão: abre e fecha o cofre */
void servo_open_and_close(void)
{
    servo_pwm_enable();

    // posição inicial
    pwm_set_chan_level(slice_num, channel_num, angle_to_pulse(0));
    sleep_ms(300);

    // abre
    pwm_set_chan_level(slice_num, channel_num, angle_to_pulse(90));
    sleep_ms(800);

    // fecha
    pwm_set_chan_level(slice_num, channel_num, angle_to_pulse(0));
    sleep_ms(800);

    servo_disable();
}
