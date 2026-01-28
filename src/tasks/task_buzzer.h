#ifndef TASK_BUZZER_H
#define TASK_BUZZER_H

#include "FreeRTOS.h"
#include "queue.h"

// enum de comandos do buzzer
typedef enum {
    BUZZER_CMD_OK,
    BUZZER_CMD_ERROR,
    BUZZER_CMD_ALARM,
    BUZZER_CMD_OPEN
} buzzer_cmd_t;

// fila global do buzzer
extern QueueHandle_t fila_buzzer;

// task
void task_buzzer(void *p);

#endif
