#include "FreeRTOS.h"
#include "task.h"

#include "acelerometro.h"
#include "buzzer.h"
#include "display.h"

void task_acelerometro(void *p) {
    int16_t ax, ay, az;
    float inclinacao;

    while (1) {

        // Lê valores brutos do MPU
        acelerometro_read_raw(&ax, &ay, &az);

        // Calcula inclinação
        inclinacao = acelerometro_get_inclinacao(ax, ay, az);

        // Detecta violação
        if (inclinacao > 30.0f) {
            display_text("VIOLACAO!", 0, 16, 1);
            buzzer_alarm();
            vTaskDelay(pdMS_TO_TICKS(2000));
        }

        vTaskDelay(pdMS_TO_TICKS(300));
    }
}
