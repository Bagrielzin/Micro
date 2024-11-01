void ADC_init(){

    ADMUX = (1 << REFS0);  // Usa AVcc como referência de tensão
    ADCSRA = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);  // Prescaler de 128
    ADCSRA |= (1 << ADEN);  // Habilita o ADC

}

uint16_t ADC_read(){

    ADCSRA |= (1 << ADSC);  // Inicia a conversão

    while (ADCSRA & (1 << ADSC));  // Espera até a conversão completar

    return ADC;  // Retorna o valor de 10 bits do ADC
}

int main(){
    
    ADC_init();  // Inicializa o ADC
    
    uint16_t adc_result;

    // Configurando Timer0 para Fast PWM
    DDRD |= (1 << PD6);  // Configura PD6 (OC0A) como saída (PWM)
    TCCR0A |= (1 << WGM00) | (1 << WGM01);  // Fast PWM, 8-bit
    TCCR0A |= (1 << COM0A1);  // Limpa OC0A (PD6) no compare match
    TCCR0B |= (1 << CS01) | (1 << CS00);  // Prescaler de 64

    while (1) {
        adc_result = ADC_read();  // Lê o valor do ADC
        OCR0A = adc_result / 4;   // Ajusta para 8 bits e define como nível PWM
    }
        
    return 0;
}