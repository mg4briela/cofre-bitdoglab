#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "task_teclado.h"
#include "teclado.h"

/* Fila criada no main */
extern QueueHandle_t fila_teclas;

void task_teclado(void *p) {
    char tecla;

    while (1) {
        tecla = teclado_read();

        if (tecla != 0) {
            xQueueSend(fila_teclas, &tecla, 0);
        }

        vTaskDelay(pdMS_TO_TICKS(30));
    }
}
