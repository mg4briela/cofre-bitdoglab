#ifndef ACELEROMETRO_H
#define ACELEROMETRO_H

#include <stdio.h>
#include <stdint.h>
#include <math.h>

#include "pico/stdlib.h"
#include "hardware/i2c.h"

// endereço I2C do MPU6050
#define MPU6050_ADDR 0x68

// I2C0 – BitDogLab
#define I2C0_SDA_PIN 0
#define I2C0_SCL_PIN 1

// inicialização
void acelerometro_init(void);

// leitura bruta
void acelerometro_read_raw(int16_t *ax, int16_t *ay, int16_t *az);

// cálculo de inclinação
float acelerometro_get_inclinacao(int16_t ax, int16_t ay, int16_t az);

/* variação em relação ao zero */
float acelerometro_get_delta_inclinacao(int16_t ax, int16_t ay, int16_t az);


#endif
