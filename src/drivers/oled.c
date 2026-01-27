#include "oled.h"

ssd1306_t oled;

void oled_init() {
    printf("[OLED] Inicializando display...\n");

    // Inicializa I2C1 a 400kHz
    i2c_init(i2c1, 400 * 1000);
    gpio_set_function(OLED_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(OLED_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(OLED_SDA_PIN);
    gpio_pull_up(OLED_SCL_PIN);

    if (!ssd1306_init(&oled, OLED_WIDTH, OLED_HEIGHT, OLED_ADDR, i2c1))
        printf("[OLED] ERRO ao inicializar!\n");
    else
        printf("[OLED] Display OK!\n");

    oled_clear();
    oled_update();
}

void oled_clear() {
    ssd1306_clear(&oled);
}

void oled_update() {
    ssd1306_show(&oled);
}

void oled_text_no_clear(const char *text, uint32_t x, uint32_t y, uint32_t scale) {
    ssd1306_draw_string(&oled, x, y, scale, text);
    oled_update();
}

void oled_text(const char *text, uint32_t x, uint32_t y, uint32_t scale) {
    oled_clear();
    ssd1306_draw_string(&oled, x, y, scale, text);
    oled_update();
}