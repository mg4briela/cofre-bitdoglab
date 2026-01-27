#include "servo.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

#define SERVO_MIN_PULSE_US 500
#define SERVO_MAX_PULSE_US 2500
#define SERVO_PERIOD_US   20000

static uint servo_gpio;
static uint slice_num;
static uint channel_num;

static uint32_t angle_to_pulse(uint angle) {
    if (angle > 180) angle = 180;
    return (angle * (SERVO_MAX_PULSE_US - SERVO_MIN_PULSE_US)) / 180
           + SERVO_MIN_PULSE_US;
}

void servo_init(uint gpio) {
    servo_gpio = gpio;

    // ⚠️ NÃO ativa PWM aqui
    gpio_init(servo_gpio);
    gpio_set_dir(servo_gpio, GPIO_OUT);
    gpio_put(servo_gpio, 0);
}

static void servo_pwm_enable(void) {
    gpio_set_function(servo_gpio, GPIO_FUNC_PWM);

    slice_num   = pwm_gpio_to_slice_num(servo_gpio);
    channel_num = pwm_gpio_to_channel(servo_gpio);

    pwm_config cfg = pwm_get_default_config();

    float div = (float)clock_get_hz(clk_sys) / 1000000.0f;
    pwm_config_set_clkdiv(&cfg, div);
    pwm_config_set_wrap(&cfg, SERVO_PERIOD_US);

    pwm_init(slice_num, &cfg, true);
}

static void servo_pwm_disable(void) {
    pwm_set_enabled(slice_num, false);
    gpio_set_function(servo_gpio, GPIO_FUNC_SIO);
    gpio_put(servo_gpio, 0);
}

void servo_open_and_close(void) {
    servo_pwm_enable();

    // posição inicial garantida
    pwm_set_chan_level(slice_num, channel_num, angle_to_pulse(0));
    sleep_ms(300);

    // abre
    pwm_set_chan_level(slice_num, channel_num, angle_to_pulse(90));
    sleep_ms(800);

    // fecha
    pwm_set_chan_level(slice_num, channel_num, angle_to_pulse(0));
    sleep_ms(800);

    // 🔒 DESLIGA O PWM
    servo_pwm_disable();
}
