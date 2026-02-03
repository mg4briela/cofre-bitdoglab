#include <math.h>

#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "queue.h"

#include "task_acelerometro.h"
#include "acelerometro.h"
#include "display.h"
#include "matriz_led.h"

// RTOS objects criados no main 
extern EventGroupHandle_t eventos;
extern QueueHandle_t fila_buzzer;

// bits de evento
#define EVT_VIOLACAO (1 << 0)

// limite
#define LIMITE_INCLINACAO 50.0

// comandos do buzzer
typedef enum {
    BUZZER_CMD_OK,
    BUZZER_CMD_ERROR,
    BUZZER_CMD_ALARM,
    BUZZER_CMD_OPEN
} buzzer_cmd_t;

void task_acelerometro(void *p) {
    int16_t ax, ay, az;

    while (1) {
        acelerometro_read_raw(&ax, &ay, &az);
        float delta = acelerometro_get_delta_inclinacao(ax, ay, az);

        if (delta > LIMITE_INCLINACAO) {

            // sinaliza violação
            xEventGroupSetBits(eventos, EVT_VIOLACAO);

            display_text("VIOLACAO!", 0, 0, 1);
            show_display();
            matriz_erro();

            // manda comando pro buzzer
            buzzer_cmd_t cmd = BUZZER_CMD_ALARM;
            xQueueSend(fila_buzzer, &cmd, 0);
        }

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}
