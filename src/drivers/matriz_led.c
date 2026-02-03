#include "matriz_led.h"
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2818b.pio.h"
#include <stdint.h>

#define LED_CENTRAL 12

typedef struct {
    uint8_t G, R, B;
} npLED_t;

static npLED_t leds[LED_COUNT];
static PIO np_pio;
static uint sm;

void matriz_init(void) {
    uint offset = pio_add_program(pio0, &ws2818b_program);
    np_pio = pio0;

    sm = pio_claim_unused_sm(np_pio, false);
    if (sm < 0) {
        np_pio = pio1;
        sm = pio_claim_unused_sm(np_pio, true);
    }

    ws2818b_program_init(np_pio, sm, offset, LED_PIN, 800000.f);
    matriz_clear();
    matriz_write();
}

void matriz_set_led(uint32_t index, uint8_t r, uint8_t g, uint8_t b) {
    leds[index].R = r;
    leds[index].G = g;
    leds[index].B = b;
}

void matriz_clear(void) {
    for (uint i = 0; i < LED_COUNT; i++) {
        matriz_set_led(i, 0, 0, 0);
    }
}

void matriz_write(void) {
    for (uint i = 0; i < LED_COUNT; i++) {
        pio_sm_put_blocking(np_pio, sm, leds[i].G);
        pio_sm_put_blocking(np_pio, sm, leds[i].R);
        pio_sm_put_blocking(np_pio, sm, leds[i].B);
    }
    sleep_us(100);
}

// estados do cofre
void matriz_ok(void) {
    matriz_clear();
    matriz_set_led(LED_CENTRAL, 0, 255, 0); // verde
    matriz_write();
}

void matriz_erro(void) {
    matriz_clear();
    matriz_set_led(LED_CENTRAL, 255, 0, 0); // vermelho
    matriz_write();
}

void matriz_espera(void) {
    matriz_clear();
    matriz_set_led(LED_CENTRAL, 0, 0, 255); // azul
    matriz_write();
}