#define motor 0

ISR(INT0_vect) {  // Interrupção do botão LIGA
    motor = 1;
    PORTD |= (1 << PD7);   // Liga o motor
    OCR0A = 0;             // Reseta PWM

    // Habilita o Timer1 com prescaler para começar o incremento
    TCCR1B |= (1 << CS11) | (1 << CS10);  // Prescaler de 64 para Timer1
}

ISR(TIMER1_COMPA_vect) {  // Interrupção do Timer1 a cada 1 segundo
    if (OCR0A < 255) {
        OCR0A += 25;  // Incrementa duty cycle em passos (aproximadamente 10 passos para 100%)

        if (OCR0A >= 127) {  // 50% do duty cycle
            PORTD |= (1 << PD1);  // Liga LED de 50%
        }
        if (OCR0A >= 255) {  // 100% do duty cycle
            PORTD &= ~(1 << PD1); // Desliga LED de 50%
            PORTD |= (1 << PD4);  // Liga LED de 100%

            // Desativa o Timer1, pois já atingimos 100%
            TCCR1B &= ~((1 << CS11) | (1 << CS10));  // Desliga o Timer1 (prescaler zero)
        }
    }
}

ISR(INT1_vect) {  // Interrupção do botão DESLIGA
    PORTD &= ~(1 << PD1);  // Desliga LED de 50%
    PORTD &= ~(1 << PD4);  // Desliga LED de 100%
    PORTD &= ~(1 << PD7);  // Desliga o motor
    OCR0A = 0;             // Reseta PWM
    motor = 0;

    // Desativa o Timer1 para garantir que o sistema pare completamente
    TCCR1B &= ~((1 << CS11) | (1 << CS10));  // Desliga o Timer1
}

int main(){

    EICRA |= (1 << ISC01) | (1 << ISC11);
    EIMSK |= (1 << INT0) | (1 << INT1);
    DDRD |= (1 << PD6) | (1 << PD1) | (1 << PD4) | (1 << PD7);//PWM, led 50%, led 100%, motor
    TCCR0A |= (1 << COM0A1) | (1 << WGM01) | (1 << WGM00);
    TCCR0B |= (1 << CS01); // Prescaler de 8
    OCR0A = 0; // Inicia PWM com duty cycle de 0%

    // Configuração do Timer1 para controle de tempo do incremento
    TCCR1B |= (1 << WGM12); // Timer1 em modo CTC (Clear Timer on Compare Match)
    OCR1A = 15624;          // Define o valor de comparação para 1 segundo (prescaler de 64)
    TIMSK1 |= (1 << OCIE1A); // Habilita interrupção de comparação para Timer1

    sei(); // Habilita interrupções globais

    return 0;
}