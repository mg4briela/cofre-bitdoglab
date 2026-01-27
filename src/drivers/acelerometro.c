#include "acelerometro.h"

// Inicializa o MPU6050
void acelerometro_init(void) {
    // Inicializa o I2C0 em 400kHz
    i2c_init(i2c0, 400 * 1000);

    // Configura pinos I2C
    gpio_set_function(I2C0_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C0_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C0_SDA_PIN);
    gpio_pull_up(I2C0_SCL_PIN);

    sleep_ms(100);

    // Sai do modo sleep (Power Management Register)
    uint8_t cmd[2] = {0x6B, 0x00};
    i2c_write_blocking(i2c0, MPU6050_ADDR, cmd, 2, false);
}

// Lê valores brutos do acelerômetro
void acelerometro_read_raw(int16_t *ax, int16_t *ay, int16_t *az) {
    uint8_t buffer[6];

    // Registrador inicial do acelerômetro
    uint8_t reg = 0x3B;
    i2c_write_blocking(i2c0, MPU6050_ADDR, &reg, 1, true);
    i2c_read_blocking(i2c0, MPU6050_ADDR, buffer, 6, false);

    *ax = (buffer[0] << 8) | buffer[1];
    *ay = (buffer[2] << 8) | buffer[3];
    *az = (buffer[4] << 8) | buffer[5];
}

// Calcula inclinação em graus
float acelerometro_get_inclinacao(int16_t ax, int16_t ay, int16_t az) {
    float ax_g = ax / 16384.0;
    float ay_g = ay / 16384.0;
    float az_g = az / 16384.0;

    return atan2(ax_g, sqrt(ay_g * ay_g + az_g * az_g)) * (180.0 / M_PI);
}
