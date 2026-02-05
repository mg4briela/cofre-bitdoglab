#include <stdio.h>
#include <string.h>
#include <math.h>
#include "pico/stdlib.h"

// tasks
#include "tasks/task_buzzer.h"
#include "tasks/task_cofre.h"
#include "tasks/task_teclado.h"
#include "tasks/task_acelerometro.h"

// FreeRTOS
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "event_groups.h"

// módulos
#include "teclado.h"
#include "display.h"
#include "matriz_led.h"
#include "servo.h"
#include "buzzer.h"
#include "acelerometro.h"

// defines do projeto
#define SENHA_TAM 4
#define SENHA_CORRETA "8888"
#define SERVO_GPIO 2
#define LIMITE_INCLINACAO 10.0

// RTOS objects
QueueHandle_t fila_teclas;
QueueHandle_t fila_buzzer;
EventGroupHandle_t eventos;

// bits de evento
#define EVT_VIOLACAO (1 << 0)
#define EVT_ABRIR    (1 << 1)

// /////////////////////////////////

int main() {
    stdio_init_all();
    sleep_ms(3000);

    // inicialização de hardware
    teclado_init();
    display_init();
    matriz_init();
    servo_init(SERVO_GPIO);
    buzzer_init();
    acelerometro_init();

    printf("[UART] Hardware inicializado!\n");

    // RTOS objects
    fila_teclas = xQueueCreate(10, sizeof(char));
    fila_buzzer = xQueueCreate(5, sizeof(buzzer_cmd_t));
    eventos = xEventGroupCreate();

    printf("[UART] Filas e eventos criados!\n");

    // criação das tasks
    xTaskCreate(task_teclado, "Teclado", 256, NULL, 2, NULL);
    xTaskCreate(task_cofre, "Cofre", 512, NULL, 3, NULL);
    xTaskCreate(task_acelerometro, "Accel", 256, NULL, 4, NULL);
    xTaskCreate(task_buzzer, "Buzzer", 256, NULL, 5, NULL);

    printf("[UART] Tasks criadas. Iniciando Scheduler...\n");

    // inicia o RTOS
    vTaskStartScheduler();

    while (1) {}
}
