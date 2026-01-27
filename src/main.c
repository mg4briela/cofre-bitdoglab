#include <stdio.h>

#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "teclado.h"
#include "display.h"
#include "servo.h"
#include "acelerometro.h"

#include "queue_shared.h"


#include "tasks/task_buzzer.h"

extern QueueHandle_t fila_teclado;

// protótipos das outras tasks
void task_teclado(void *p);
void task_display(void *p);
void task_cofre(void *p);
void task_acelerometro(void *p);

int main() {
    stdio_init_all();
    sleep_ms(2000);

    teclado_init();
    display_init();
    servo_init(2);
    acelerometro_init();

    // fila do buzzer
    fila_buzzer = xQueueCreate(5, sizeof(buzzer_cmd_t));

    fila_teclado = xQueueCreate(10, sizeof(char));

    fila_senha   = xQueueCreate(2, sizeof(char) * 4);

    if (fila_teclado == NULL || fila_senha == NULL) {
        while (1); // erro de memória
    }


    // criação das tasks
    xTaskCreate(task_teclado,     "Teclado",      1024, NULL, 2, NULL);
    xTaskCreate(task_display,     "Display",      1024, NULL, 1, NULL);
    xTaskCreate(task_cofre,       "Cofre",        1024, NULL, 3, NULL);
    xTaskCreate(task_acelerometro,"Acelerometro", 1024, NULL, 4, NULL);
    xTaskCreate(task_buzzer,      "Buzzer",        512, NULL, 2, NULL);

    vTaskStartScheduler();

    while (1) {}
}
