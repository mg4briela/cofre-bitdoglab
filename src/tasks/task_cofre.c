#include <string.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "queue_shared.h"
#include "servo.h"
#include "buzzer.h"
#include "display.h"

#define TAM_SENHA 4

void task_cofre(void *p) {

    const char senha_correta[TAM_SENHA] = {'1', '2', '3', '4'};
    char senha_recebida[TAM_SENHA];

    while (1) {

        // agora o cofre espera a senha pronta
        if (xQueueReceive(fila_senha, senha_recebida, portMAX_DELAY)) {

            if (memcmp(senha_recebida, senha_correta, TAM_SENHA) == 0) {
                clear_display();
                display_text("Cofre aberto", 0, 0, 1);
                show_display();

                servo_set_angle(90);
                buzzer_beep(200);
            } else {
                clear_display();
                display_text("Senha errada", 0, 0, 1);
                show_display();

                buzzer_error();
            }

            vTaskDelay(pdMS_TO_TICKS(1500));

            clear_display();
            display_text("Digite a senha:", 0, 0, 1);
            show_display();
        }
    }
}
