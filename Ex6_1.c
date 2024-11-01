#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// Definição dos pinos
#define MOTOR_PIN PD0      // Pino para controlar o motor
#define EMERGENCY_LED PD1  // Pino para o LED de emergência
#define EMERGENCY_BUTTON PD2 // Pino para o botão de emergência (INT0)

volatile uint8_t emergency_stop = 0; // Flag para controle de emergência

// Função de interrupção para o botão de emergência
ISR(INT0_vect) {
    // Interrompe o motor e acende o LED de emergência
    PORTD &= ~(1 << MOTOR_PIN);  // Desliga o motor
    PORTD |= (1 << EMERGENCY_LED); // Acende o LED
    emergency_stop = 1; // Sinaliza que a emergência foi acionada
}

void init_interrupt() {
    // Configura INT0 (PD2) para detectar borda de descida (botão de emergência)
    EICRA |= (1 << ISC01);   // Configura para borda de descida
    EICRA &= ~(1 << ISC00);  // ISC01 = 1 e ISC00 = 0 -> Borda de descida no INT0
    EIMSK |= (1 << INT0);    // Habilita a interrupção no INT0
    sei();                   // Habilita interrupções globais
}

void init_pins() {
    // Configura MOTOR_PIN e EMERGENCY_LED como saídas
    DDRD |= (1 << MOTOR_PIN);
    DDRD |= (1 << EMERGENCY_LED);

    // Configura EMERGENCY_BUTTON (INT0) como entrada
    DDRD &= ~(1 << EMERGENCY_BUTTON);
    PORTD |= (1 << EMERGENCY_BUTTON); // Habilita pull-up interno para o botão
}

int main(void) {
    init_pins();        // Inicializa os pinos
    init_interrupt();   // Inicializa a interrupção para o botão de emergência

    while (1) {
        if (!emergency_stop) {  // Se o sistema não está em estado de emergência
            // Aciona o motor
            PORTD |= (1 << MOTOR_PIN);
            _delay_ms(5000); // Mantém o motor ligado por 5 segundos

            // Desliga o motor
            PORTD &= ~(1 << MOTOR_PIN);
            _delay_ms(5000); // Mantém o motor desligado por 5 segundos
        } else {
            // Estado de emergência: o LED permanece aceso e o motor desligado
            // Aguarda até que o sistema seja reiniciado manualmente
        }
    }
}

//Método 2
int main(){

    DDRD |= (1 << PD6);

    for(;;){
        PORTD ^= (1 << PD6);
        _delay_ms(5000);
        if(PIND  &(1 << PD2) != 0){
            while(1){
                PORTD |= (1 << PD6);
                PORTB |= (1 << PB0);
            }
        }
    }
}

//Método 3
#define NORMAL_OP 0
#define EMERGENCIA 1

unsigned char estado = NORMAL_OP;
int main(){

    DDRD |= (1 << PD6);

    DDRB |= (1 << PB0);

    //Transicao de subida
    EICRA |= (1 << ISC00) + (1 << ISC01);
    EIMSK |= (1<<INT0);

    sei();
    
    for(;;){
        switch (estado)
        {
        case NORMAL_OP:
            PORTD ^= (1 << PD6);
            _delay_ms(5000);
            break;
        
        case EMERGENCIA:
            while (1)
            {
                PORTD |= (1 << PD6);
                PORTB |= (1 << PB0);
            }
            break;
        }
    }

    return 0;
}

ISR(INT0_vect){
    PORTD |= (1 << PD6);
    PORTB |= (1 << PB0);
    estado = EMERGENCIA;
}