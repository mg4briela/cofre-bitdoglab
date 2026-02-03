#ifndef BUZZER_H
#define BUZZER_H

#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define BUZZER_PIN 21
#define BUZZER_FREQ 2000   // 2 kHz

void buzzer_init(void);
void buzzer_on(void);
void buzzer_off(void);

void buzzer_beep(uint16_t duration_ms);
void buzzer_ok(void);
void buzzer_error(void);
void buzzer_open(void);
void buzzer_alarm(void);

#endif
