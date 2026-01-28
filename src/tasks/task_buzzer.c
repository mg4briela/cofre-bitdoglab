// task_buzzer.c
#include "task_buzzer.h"   // declara a task
#include "buzzer.h"        // declara BUZZER_CMD_OK, buzzer_ok(), etc
#include "FreeRTOS.h"      // tipos do FreeRTOS
#include "queue.h"         // xQueueReceive
#include "task_buzzer.h"

void task_buzzer(void *p) {
    buzzer_cmd_t cmd;
    while (1) {
        if (xQueueReceive(fila_buzzer, &cmd, portMAX_DELAY)) {
            switch (cmd) {
                case BUZZER_CMD_OK:    buzzer_ok(); break;
                case BUZZER_CMD_ERROR: buzzer_error(); break;
                case BUZZER_CMD_ALARM: buzzer_alarm(); break;
                case BUZZER_CMD_OPEN:  buzzer_open(); break;
            }
        }
    }
}
