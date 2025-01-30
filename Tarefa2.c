#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/time.h"

// Definir os pinos dos LEDs para as cores do semáforo
#define LED_VERMELHO_PIN  13
#define LED_AMARELO_PIN   12
#define LED_VERDE_PIN     11

// Definir os estados do semáforo como constantes
#define VERMELHO 0
#define AMARELO  1
#define VERDE    2

int estado_atual = VERMELHO; // Começa no estado vermelho

// Função de callback para o temporizador (retorna bool)
bool repeating_timer_callback(struct repeating_timer *t) {
    // Primeiro, desliga todos os LEDs
    gpio_put(LED_VERMELHO_PIN, 0);
    gpio_put(LED_AMARELO_PIN, 0);
    gpio_put(LED_VERDE_PIN, 0);

    // Atualiza o estado
    switch (estado_atual) {
        case VERMELHO:
            estado_atual = AMARELO; // Próximo estado: amarelo
            break;
        case AMARELO:
            estado_atual = VERDE;   // Próximo estado: verde
            break;
        case VERDE:
            estado_atual = VERMELHO; // Próximo estado: vermelho
            break;
    }

    // Acende o LED correspondente ao novo estado
    switch (estado_atual) {
        case VERMELHO:
            gpio_put(LED_VERMELHO_PIN, 1); // Liga o vermelho
            printf("Mudança de estado: VERMELHO\n");
            break;
        case AMARELO:
            gpio_put(LED_AMARELO_PIN, 1); // Liga o amarelo
            printf("Mudança de estado: AMARELO\n");
            break;
        case VERDE:
            gpio_put(LED_VERDE_PIN, 1);   // Liga o verde
            printf("Mudança de estado: VERDE\n");
            break;
    }

    return true; // Continua executando o temporizador
}

// Função principal
int main() {
    stdio_init_all(); // Inicializa a comunicação serial

    // Inicializar os pinos dos LEDs como saída
    gpio_init(LED_VERMELHO_PIN);
    gpio_init(LED_AMARELO_PIN);
    gpio_init(LED_VERDE_PIN);
    
    gpio_set_dir(LED_VERMELHO_PIN, GPIO_OUT);
    gpio_set_dir(LED_AMARELO_PIN, GPIO_OUT);
    gpio_set_dir(LED_VERDE_PIN, GPIO_OUT);
    
    // Acender o LED vermelho no início
    gpio_put(LED_VERMELHO_PIN, 1);
    gpio_put(LED_AMARELO_PIN, 0);
    gpio_put(LED_VERDE_PIN, 0);

    printf("Estado inicial: VERMELHO\n");

    // Inicializar o temporizador para mudar o estado a cada 3 segundos
    struct repeating_timer timer;
    add_repeating_timer_ms(3000, repeating_timer_callback, NULL, &timer);
    
    // Loop principal imprimindo mensagens a cada 1 segundo
    while (true) {
        printf("Estado atual do semáforo: %d\n", estado_atual);
        sleep_ms(1000);
    }

    return 0;
}