#include "acelerometro.h"
#include <math.h>

static float inclinacao_ref = 0.0;

// inicializa o MPU6050
void acelerometro_init(void) {
    // inicializa o I2C
    i2c_init(i2c0, 400 * 1000);

    // configura pinos I2C
    gpio_set_function(I2C0_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C0_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C0_SDA_PIN);
    gpio_pull_up(I2C0_SCL_PIN);

    sleep_ms(100);

    // sai do modo sleep
    uint8_t cmd[2] = {0x6B, 0x00};
    i2c_write_blocking(i2c0, MPU6050_ADDR, cmd, 2, false);

    sleep_ms(300);

    int16_t ax, ay, az;

    acelerometro_read_raw(&ax, &ay, &az);
    inclinacao_ref = acelerometro_get_inclinacao(ax, ay, az);
}

// lê valores brutos do acelerômetro
void acelerometro_read_raw(int16_t *ax, int16_t *ay, int16_t *az) {
    uint8_t buffer[6];

    // registrador inicial do acelerômetro
    uint8_t reg = 0x3B;
    i2c_write_blocking(i2c0, MPU6050_ADDR, &reg, 1, true);
    i2c_read_blocking(i2c0, MPU6050_ADDR, buffer, 6, false);

    *ax = (buffer[0] << 8) | buffer[1];
    *ay = (buffer[2] << 8) | buffer[3];
    *az = (buffer[4] << 8) | buffer[5];
}

// calcula inclinação em graus
float acelerometro_get_inclinacao(int16_t ax, int16_t ay, int16_t az) {
    float ax_g = ax / 16384.0;
    float ay_g = ay / 16384.0;
    float az_g = az / 16384.0;

    return atan2(ax_g, sqrt(ay_g * ay_g + az_g * az_g)) * (180.0 / M_PI);
}

float acelerometro_get_delta_inclinacao(int16_t ax, int16_t ay, int16_t az) {
    float inclinacao_atual = acelerometro_get_inclinacao(ax, ay, az);
    return fabs(inclinacao_atual - inclinacao_ref);
}
