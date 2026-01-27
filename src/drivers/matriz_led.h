#ifndef MATRIZ_LED_H
#define MATRIZ_LED_H

#include <stdint.h>

#define LED_COUNT 25
#define LED_PIN 7

void matriz_init(void);
void matriz_clear(void);
void matriz_set_led(uint32_t index, uint8_t r, uint8_t g, uint8_t b);
void matriz_write(void);

void matriz_ok(void);
void matriz_erro(void);
void matriz_espera(void);

#endif
