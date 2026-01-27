#ifndef SERVO_H
#define SERVO_H

#include "pico/stdlib.h"

// Parâmetros padrão de servo
#define SERVO_MIN_PULSE_US 500
#define SERVO_MAX_PULSE_US 2500
#define SERVO_PERIOD_US   20000

void servo_init(uint gpio);
void servo_set_angle(uint angle);
void servo_open_and_close(void);
void servo_disable(void);

#endif
