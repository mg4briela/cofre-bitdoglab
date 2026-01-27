#include "buzzer.h"

static uint buzzer_slice;
static uint buzzer_channel;

// Inicializa PWM no buzzer
void buzzer_init(void) {
    gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM);

    buzzer_slice = pwm_gpio_to_slice_num(BUZZER_PIN);
    buzzer_channel = pwm_gpio_to_channel(BUZZER_PIN);

    pwm_config config = pwm_get_default_config();

    // Clock base: 125 MHz
    // Vamos gerar ~2 kHz
    // freq = clock / (wrap + 1)
    pwm_config_set_wrap(&config, 62500); // 125MHz / 62500 ≈ 2kHz

    pwm_init(buzzer_slice, &config, true);

    // Começa desligado
    pwm_set_chan_level(buzzer_slice, buzzer_channel, 0);
}

void buzzer_on(void) {
    pwm_set_chan_level(buzzer_slice, buzzer_channel, 5000); // ~8% duty
}

void buzzer_off(void) {
    pwm_set_chan_level(buzzer_slice, buzzer_channel, 0);
}

void buzzer_beep(uint16_t duration_ms) {
    buzzer_on();
    sleep_ms(duration_ms);
    buzzer_off();
}

void buzzer_ok(void) {
    buzzer_beep(80);
}

void buzzer_error(void) {
    for (int i = 0; i < 3; i++) {
        buzzer_beep(120);
        sleep_ms(80);
    }
}

void buzzer_alarm(void) {
    for (int i = 0; i < 5; i++) {
        buzzer_error();
        sleep_ms(200);
    }
}

void buzzer_open(void) {
    buzzer_beep(500);
}
