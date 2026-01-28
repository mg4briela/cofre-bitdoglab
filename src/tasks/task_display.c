#include <string.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "queue_shared.h"
#include "display.h"

#define SENHA_TAM 4

void task_display(void *p) {
    char tecla;
    char senha[SENHA_TAM];
    int indice = 0;

    clear_display();
    display_text("Digite a senha:", 0, 0, 1);
    show_display();

    while (1) {

        if (xQueueReceive(fila_teclado, &tecla, portMAX_DELAY)) {

            if (tecla == '#') {
                indice = 0;
                memset(senha, 0, sizeof(senha));

                clear_display();
                display_text("Digite a senha:", 0, 0, 1);
                show_display();
                continue;
            }

            if (indice < SENHA_TAM) {
                senha[indice++] = tecla;

                clear_display();
                display_text("Senha:", 0, 0, 1);

                for (int i = 0; i < indice; i++) {
                    display_text_no_clear("*", 50 + (i * 10), 16, 1);
                }

                show_display();
            }

            // senha completa → envia para o cofre
            if (indice == SENHA_TAM) {
                xQueueSend(fila_senha, senha, portMAX_DELAY);
                indice = 0;
            }
        }
    }
}
