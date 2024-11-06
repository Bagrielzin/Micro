void ADC_init(){
    ADMUX = (1 << REFS0); // AVcc como referência, pino ADC0
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Prescaler de 128
    ADCSRA |= (1 << ADEN); // Habilita o ADC
}

uint16_t ADC_read(){
    ADCSRA |= (1 << ADSC); // Inicia a conversão
    while (ADCSRA & (1 << ADSC)); // Aguarda a conversão completar
    return ADC; // Retorna o resultado
}

int main(){

    ADC_init();
    uint8_t bit_sequence[50]; // Armazena a sequência de bits
    uint8_t index = 0;

    while(1){
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

}

