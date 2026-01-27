#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "queue_shared.h"
#include "teclado.h"


void task_teclado(void *p) {
    char tecla;

    while (1) {
        tecla = teclado_read();

        if (tecla != 0) {
            xQueueSend(fila_teclado, &tecla, portMAX_DELAY);
            vTaskDelay(pdMS_TO_TICKS(250)); // debounce
        }

        vTaskDelay(pdMS_TO_TICKS(20));
    }
}
