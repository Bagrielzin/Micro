#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// Definição dos pinos
#define LAMP1_PIN PD0     // Pino para Lâmpada 1
#define LAMP2_PIN PD1     // Pino para Lâmpada 2
#define BUZZER_PIN PD2    // Pino para o Buzzer
#define CONTROL_PIN PD3   // Pino de entrada do controle (impulso de 5V por 500ms)

// Definição dos estados do portão
typedef enum {
    FECHADO,
    ABRINDO,
    FECHANDO,
    ABERTO,
    PARADO
} estado_portao_t;

volatile estado_portao_t estado_portao = FECHADO;  // Estado inicial do portão
volatile uint8_t controle_pressionado = 0;  // Flag para controle

// Função de interrupção para detectar o impulso do controle
ISR(INT1_vect) {
    controle_pressionado = 1; // Sinaliza que o controle foi pressionado
}

void init_interrupt() {
    // Configura INT1 (PD3) para detectar borda de subida (impulso do controle)
    EICRA |= (1 << ISC11) | (1 << ISC10);  // Configura para borda de subida
    EIMSK |= (1 << INT1);                  // Habilita a interrupção no INT1
    sei();                                 // Habilita interrupções globais
}

void init_pins() {
    // Configura LAMP1_PIN, LAMP2_PIN e BUZZER_PIN como saídas
    DDRD |= (1 << LAMP1_PIN) | (1 << LAMP2_PIN) | (1 << BUZZER_PIN);

    // Configura CONTROL_PIN (PD3) como entrada
    DDRD &= ~(1 << CONTROL_PIN);
    PORTD |= (1 << CONTROL_PIN); // Habilita pull-up interno para o controle
}

void alterna_luzes_buzzer() {
    PORTD ^= (1 << LAMP1_PIN);  // Alterna o estado da Lâmpada 1
    PORTD ^= (1 << LAMP2_PIN);  // Alterna o estado da Lâmpada 2
    PORTD ^= (1 << BUZZER_PIN); // Alterna o estado do Buzzer
}

void abrir_portao() {
    for (int i = 0; i < 10; i++) { // Simula 10 segundos de abertura
        alterna_luzes_buzzer();
        _delay_ms(1000); // Alterna a cada 1 segundo
    }
    PORTD &= ~((1 << LAMP1_PIN) | (1 << LAMP2_PIN) | (1 << BUZZER_PIN)); // Desliga tudo
    estado_portao = ABERTO; // Atualiza o estado para ABERTO
}

void fechar_portao() {
    for (int i = 0; i < 10; i++) { // Simula 10 segundos de fechamento
        alterna_luzes_buzzer();
        _delay_ms(1000); // Alterna a cada 1 segundo
    }
    PORTD &= ~((1 << LAMP1_PIN) | (1 << LAMP2_PIN) | (1 << BUZZER_PIN)); // Desliga tudo
    estado_portao = FECHADO; // Atualiza o estado para FECHADO
}

int main(void) {
    init_pins();        // Inicializa os pinos de entrada e saída
    init_interrupt();   // Inicializa a interrupção para o controle

    while (1) {
        if (controle_pressionado) {  // Se o controle foi pressionado
            controle_pressionado = 0; // Reseta o flag

            switch (estado_portao) {
                case FECHADO:
                    estado_portao = ABRINDO;
                    abrir_portao();
                    break;

                case ABERTO:
                    estado_portao = FECHANDO;
                    fechar_portao();
                    break;

                case ABRINDO:
                case FECHANDO:
                    estado_portao = PARADO;
                    // Portão interrompido, nada mais é feito até novo comando
                    break;

                case PARADO:
                    // Se estava parado, decide o que fazer com base no último estado
                    if (PORTD & (1 << LAMP1_PIN)) { // Se ainda está abrindo
                        fechar_portao();
                    } else { // Se estava fechando
                        abrir_portao();
                    }
                    break;
            }
        }
    }
}
