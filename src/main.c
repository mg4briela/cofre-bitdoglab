#include <stdio.h>
#include <string.h>
#include <math.h>

#include "pico/stdlib.h"

#include "teclado.h"
#include "display.h"
#include "matriz_led.h"
#include "servo.h"
#include "buzzer.h"
#include "acelerometro.h"

#define SENHA_TAM 4
#define SENHA_CORRETA "8888"
#define SERVO_GPIO 2

#define LIMITE_INCLINACAO 50.0  // graus

int main() {
    stdio_init_all();
    sleep_ms(2000);

    // Inicializações
    teclado_init();
    display_init();
    matriz_init();
    servo_init(SERVO_GPIO);
    buzzer_init();
    acelerometro_init();

    char senha_digitada[SENHA_TAM + 1] = {0};
    int indice = 0;

    bool cofre_aberto = false;
    bool violacao = false;

    int16_t ax, ay, az;
    float inclinacao;

    display_text("Digite a senha:", 0, 0, 1);
    show_display();
    matriz_espera();

    while (true) {

        // Verificação contínua de violação
        acelerometro_read_raw(&ax, &ay, &az);
        inclinacao = acelerometro_get_inclinacao(ax, ay, az);

        if (fabs(inclinacao) > LIMITE_INCLINACAO && !cofre_aberto) {
            violacao = true;

            display_text("VIOLACAO!", 0, 0, 1);
            show_display();

            matriz_erro();
            buzzer_alarm();

            sleep_ms(1000);
        }

        if (violacao) {
            sleep_ms(200);
            continue; // bloqueia tudo até reset
        }

        // Leitura do teclado
        char tecla = teclado_read();

        if (tecla != 0) {

            // Limpar senha
            if (tecla == '#') {
                indice = 0;
                memset(senha_digitada, 0, sizeof(senha_digitada));

                buzzer_ok();

                display_text("Limpo", 0, 0, 1);
                show_display();
                matriz_espera();
                sleep_ms(800);

                display_text("Digite a senha:", 0, 0, 1);
                show_display();
            }

            // Confirmar senha
            else if (tecla == '*') {
                buzzer_ok();

                display_text("Verificando...", 0, 0, 1);
                show_display();
                sleep_ms(800);

                if (strcmp(senha_digitada, SENHA_CORRETA) == 0) {

                    cofre_aberto = true;

                    display_text("ACESSO OK", 0, 0, 1);
                    show_display();
                    matriz_ok();

                    buzzer_open();

                    // Abre e fecha o cofre
                    servo_open_and_close();

                    cofre_aberto = false;

                } else {
                    display_text("ACESSO NEGADO", 0, 0, 1);
                    show_display();
                    matriz_erro();

                    buzzer_error();
                }

                sleep_ms(1500);

                indice = 0;
                memset(senha_digitada, 0, sizeof(senha_digitada));

                display_text("Digite a senha:", 0, 0, 1);
                show_display();
                matriz_espera();
            }

            // Digitando senha
            else if (indice < SENHA_TAM) {
                senha_digitada[indice++] = tecla;

                buzzer_ok();

                clear_display();
                display_text("Senha:", 0, 0, 1);

                for (int i = 0; i < indice; i++) {
                    display_text_no_clear("*", 50 + (i * 10), 16, 1);
                }

                show_display();
                matriz_espera();
            }

            sleep_ms(250);
        }
    }
}
