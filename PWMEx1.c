#define MAX_SPEED 255  // Velocidade máxima representada no PWM de 8 bits

// acelerador
ISR(INT0_vect){

    if(OCR0A * 1.05 < MAX_SPEED){
        OCR0A *= 1.05;
    }
    else{
        OCR0A = MAX_SPEED;
    }
}

// freio
ISR(INT1_vect){
    OCR0A = 0;
}

int main(){

    // Configuração do PWM no pino PD6
    DDRD |= (1 << PD6);  // Configura PD6 como saída
    TCCR0A |= (1 << COM0A1) | (1 << WGM01) | (1 << WGM00);  // Modo Fast PWM, não-invertido
    TCCR0B |= (1 << CS01);  // Prescaler de 8

    OCR0A = 0;  // Inicializa o PWM com 0

    EICRA |= (1 << ISC01) | (1 << ISC11);  // Configura INT0 e INT1 para ativar em borda de descida
    EIMSK |= (1 << INT0) | (1 << INT1);    // Habilita as interrupções INT0 e INT1

    sei();

    while(1){

    }
}