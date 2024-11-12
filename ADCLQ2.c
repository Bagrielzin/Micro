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

int main(){
    DDRD |= (1 << PD0) | (1 << PD1) | (1 << PD2); // LEDs verde, amarelo, vermelho

    ADC_init();
    uint16_t adc_result;
  
    // Converte 0-5V para 0-1023 no ADC
    // 80% de 5V = 4V -> (4V/5V) * 1023 = 819.2 -> 819
    // 50% de 5V = 2.5V -> (2.5V/5V) * 1023 = 511.5 -> 511

    while(1){
        adc_result = ADC_read();
        if(adc_result >= 819){
            PORTD |= (1 << PD0);
            PORTD &= ~(1 << PD1);
            PORTD &= ~(1 << PD2);
        }
        else if(adc_result < 819 && adc_result > 511){
            PORTD &= ~(1 << PD0);
            PORTD |= (1 << PD1);
            PORTD &= ~(1 << PD2);
        }
        else{
            PORTD &= ~(1 << PD0);
            PORTD &= ~(1 << PD1);
            PORTD |= (1 << PD2);
        }
    }
}

