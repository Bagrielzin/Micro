#include <avr/io.h>
#include <avr/interrupt.h>

#define DUTY_CYCLE 75      // Ciclo de trabalho em porcentagem (0-100)
#define F_CPU 4000000UL    // Frequência do microcontrolador (4 MHz)
#define PWM_FREQUENCY 500  // Frequência desejada para o PWM em Hz

void init_timer0_ctc_pwm() {
    // Configuração do pino de saída para o PWM (OC0A - PB0)
    DDRB |= (1 << PB0);  // Define PB0 como saída

    // Configuração do Timer0 em modo CTC com prescaler de 64
    TCCR0A |= (1 << WGM01);  // Modo CTC
    TCCR0B |= (1 << CS01) | (1 << CS00);  // Prescaler de 64

    // Configuração de OCR0A para definir a frequência do PWM
    OCR0A = (F_CPU / (64 * PWM_FREQUENCY)) - 1;

    // Habilita interrupção de comparação A
    TIMSK0 |= (1 << OCIE0A);

    // Habilita interrupções globais
    sei();
}

ISR(TIMER0_COMPA_vect) {
    static uint8_t counter = 0;

    // Controla o ciclo de trabalho do PWM
    if (counter < (OCR0A * DUTY_CYCLE) / 100) {
        PORTB |= (1 << PB0);  // Sinal alto (dentro do ciclo de trabalho)
    } else {
        PORTB &= ~(1 << PB0);  // Sinal baixo (fora do ciclo de trabalho)
    }

    counter++;
    
    // Reinicia o contador quando atinge o valor de OCR0A
    if (counter >= OCR0A) {
        counter = 0;
    }
}

int main(void) {
    init_timer0_ctc_pwm();  // Inicializa o Timer0 com PWM
    while (1) {
        // Loop principal vazio
    }
}
