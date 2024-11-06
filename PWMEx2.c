#include <avr/io.h>
#include <avr/interrupt.h>

#define freqcpu 4000000  // 4 MHz
#define freq 50          // Frequência desejada (50 Hz)
#define duty_cycle 0.75  // Duty cycle de 75%

volatile uint16_t contador = 0;  // Variável para o contador

ISR(TIMER0_COMPA_vect) {
    uint16_t max = (uint16_t)(OCR0A * duty_cycle);  // Calcula o valor do ciclo alto com duty cycle

    if (contador < max) {
        PORTB |= (1 << PB0);  // Define PB0 em nível alto
    } else {
        PORTB &= ~(1 << PB0);  // Define PB0 em nível baixo
    }

    contador++;
    if (contador >= OCR0A) {
        contador = 0;  // Reseta o contador ao final do ciclo
    }
}

int main(void) {
    DDRB |= (1 << PB0);  // Configura PB0 como saída

    // Configuração do Timer 0 em modo CTC com prescaler de 8
    TCCR0A |= (1 << WGM01);  // Modo CTC
    TCCR0B |= (1 << CS01);   // Prescaler de 8

    // Configuração de OCR0A para atingir a frequência desejada
    OCR0A = (freqcpu / (8 * freq)) - 1;  // Define o TOP para o modo CTC

    // Habilita a interrupção de comparação do Timer 0
    TIMSK |= (1 << OCIE0A); 

    sei();  // Habilita interrupções globais

    while (1) {
        // Loop principal vazio, o controle é feito na ISR
    }
}
