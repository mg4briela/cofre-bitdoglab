#ifndef OLED_H
#define OLED_H

#include "ssd1306.h"
#include <stdio.h>

// Pinos corretos da BitDogLab (I2C1)
#define OLED_SDA_PIN 14
#define OLED_SCL_PIN 15

// Parâmetros do display
#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define OLED_ADDR 0x3C

extern ssd1306_t oled;

void oled_init();
void oled_clear();
void oled_update();

// Escreve texto sem apagar o fundo
void oled_text_no_clear(const char *text, uint32_t x, uint32_t y, uint32_t scale);

// Escreve texto apagando a tela antes
void oled_text(const char *text, uint32_t x, uint32_t y, uint32_t scale);

#endif
