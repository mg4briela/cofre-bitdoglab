#include "task_buzzer.h"
#include "buzzer.h"
#include "FreeRTOS.h"
#include "task.h"

QueueHandle_t fila_buzzer;

void task_buzzer(void *params) {
    buzzer_cmd_t cmd;

    buzzer_init();

    while (true) {
        if (xQueueReceive(fila_buzzer, &cmd, portMAX_DELAY)) {

            switch (cmd) {
                case BUZZER_CMD_OK:
                    buzzer_ok();
                    break;

                case BUZZER_CMD_ERROR:
                    buzzer_error();
                    break;

                case BUZZER_CMD_OPEN:
                    buzzer_open();
                    break;

                case BUZZER_CMD_ALARM:
                    buzzer_alarm();
                    break;
            }
        }
    }
}
