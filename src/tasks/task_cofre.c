#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "event_groups.h"

#include "task_cofre.h"
#include "display.h"
#include "matriz_led.h"
#include "servo.h"
#include "buzzer.h"

// externs vindos do main
extern QueueHandle_t fila_teclas;
extern EventGroupHandle_t eventos;

#define SENHA_TAM 4
#define SENHA_CORRETA "8888"
#define EVT_VIOLACAO (1 << 0)

void task_cofre(void *p) {

    char senha[SENHA_TAM + 1] = {0};
    int indice = 0;
    char tecla;
    int violacao_printada = 0;

    display_text("Digite a senha:", 0, 0, 1);
    show_display();
    matriz_espera();

    while (1) {

        if (xEventGroupGetBits(eventos) & EVT_VIOLACAO) {
            if (!violacao_printada) {
                printf("[UART] Violacao detectada! Cofre bloqueado.\n");
                violacao_printada = 1;
            }
            vTaskDelay(pdMS_TO_TICKS(200));
            continue;
        }

        if (xQueueReceive(fila_teclas, &tecla, portMAX_DELAY)) {

            if (tecla == '#') {
                indice = 0;
                memset(senha, 0, sizeof(senha));
                buzzer_ok();
                display_text("Limpo", 0, 0, 1);
                show_display();
                matriz_espera();
                printf("[UART] Entrada de senha limpa.\n");
                vTaskDelay(pdMS_TO_TICKS(800));
                display_text("Digite a senha:", 0, 0, 1);
                show_display();
            }

            else if (tecla == '*') {
                buzzer_ok();
                display_text("Verificando...", 0, 0, 1);
                show_display();
                vTaskDelay(pdMS_TO_TICKS(800));

                if (strcmp(senha, SENHA_CORRETA) == 0) {
                    printf("[UART] Senha correta! Acesso autorizado.\n");
                    display_text("ACESSO AUTORIZADO", 0, 0, 1);
                    show_display();
                    matriz_ok();
                    buzzer_open();
                    servo_open_and_close();
                } else {
                    printf("[UART] Senha incorreta! Acesso negado.\n");
                    display_text("ACESSO NEGADO", 0, 0, 1);
                    show_display();
                    matriz_erro();
                    buzzer_error();
                }

                vTaskDelay(pdMS_TO_TICKS(1500));
                indice = 0;
                memset(senha, 0, sizeof(senha));
                display_text("Digite a senha:", 0, 0, 1);
                show_display();
                matriz_espera();
            }

            else if (indice < SENHA_TAM) {
                senha[indice++] = tecla;
                buzzer_ok();
                clear_display();
                display_text("Senha:", 0, 0, 1);
                for (int i = 0; i < indice; i++) {
                    display_text_no_clear("*", 50 + (i * 10), 16, 1);
                }
                show_display();
            }
        }
    }
}
