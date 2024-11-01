uint8_t modo = 0;

int main(){

    // Configurando Timer0 para Fast PWM
    DDRD |= (1 << PD6);  // Configura PD6 (OC0A) como saída (PWM)
    TCCR0A |= (1 << WGM00) | (1 << WGM01);  // Fast PWM, 8-bit
    TCCR0A |= (1 << COM0A1);  // Limpa OC0A (PD6) no compare match
    TCCR0B |= (1 << CS01) | (1 << CS00);  // Prescaler de 64

    //configurando a interrupção do botão
    EICRA |= (1 << ISC01);
    EIMSK |= (1 << INT0);

    sei();

    while(1){
        //
    }

    return 0;
}

ISR(INT0_vect){

    switch (modo)
    {
        case 0: //silencioso
            OCR0A = 77;
            modo = 1;
            break;
        case 1: //normal
            OCR0A = 153;
            modo = 2;
            break;
        case 2: //turbo
            OCR0A = 255;
            modo = 0;
            break;
        
        default:
            break;
    }
}