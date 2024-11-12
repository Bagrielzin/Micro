void ADC_init(){
    ADMUX = (1 << REFS0); // AVcc como referência, pino ADC0
    ADCSRA = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Prescaler de 128
    ADCSRA |= (1 << ADEN); // Habilita o ADC
}

uint16_t ADC_read(){
    ADCSRA |= (1 << ADSC); // Inicia a conversão
    while (ADCSRA & (1 << ADSC)); // Aguarda a conversão completar
    return ADC; // Retorna o resultado
}

uint8_t bit_sequence[50]; // Armazena a sequência de bits
uint8_t index = 0;
uint8_t adc_result;

ISR(TIMER1_COMPA_vect){
    adc_result = ADC_read();
    if(adc_result >= 819){ // branco
        bit_sequence[index] = 1;
        index++;
    }
    else if(adc_result <= 716){// preto
        bit_sequence[index] = 0;
        index++;
    }
    else{
        index = 0;
    }    
}

int main(){

    ADC_init();

    TCCR1A = 0;
    TCCR1B = (1 << WGM11)| (1 << CS12) | (1 << CS10);// Modo CTC, Prescaler de 1024
    OCR1A = 15624; // (F_CPU = 16 MHz, f_des = 1Hz, prescaler = 1024)
    TIMSK1 |= (1 << OCIE1A); // Habilita interrupção de comparação

    while(1){
        //
    }

}

