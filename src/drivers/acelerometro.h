#ifndef ACELEROMETRO_H
#define ACELEROMETRO_H

#include <stdio.h>
#include <stdint.h>
#include <math.h>

#include "pico/stdlib.h"
#include "hardware/i2c.h"

// Endereço I2C do MPU6050
#define MPU6050_ADDR 0x68

// I2C0 – BitDogLab
#define I2C0_SDA_PIN 0
#define I2C0_SCL_PIN 1

// Inicialização
void acelerometro_init(void);

// Leitura bruta
void acelerometro_read_raw(int16_t *ax, int16_t *ay, int16_t *az);

// Cálculo de inclinação
float acelerometro_get_inclinacao(int16_t ax, int16_t ay, int16_t az);

#endif
