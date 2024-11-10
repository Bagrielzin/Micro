#include <avr/io.h>
#include <avr/interrupt.h>

void ADC_init(){
    ADMUX = (1 << REFS0) | (1 << REFS1); // Seleciona a referência de 1.1V
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Prescaler de 128
    ADCSRA |= (1 << ADEN)| (1 << ADIE); // Habilita o ADC
    sei(); // Habilita interrupções
}

void ADC_start_conversion(){
    ADCSRA |= (1 << ADSC); // Inicia a conversão
}

uint16_t adc_result;
uint16_t freq = 0;

ISR(ADC_vect){
    adc_result = ADC; // Lê o ADC
        
    // Define o valor da freq baseado na pressão de entrada
    if (adc_result == 102) {
        freq = 264;
    } else if (adc_result == 204) {
        freq = 300;
    } else if (adc_result == 307) {
        freq = 330;
    } else if (adc_result == 409) {
        freq = 352;
    } else if (adc_result == 511) {
        freq = 396;
    } else if (adc_result == 614) {
        freq = 440;
    } else if (adc_result >= 716) {
        freq = 495;
    }
    else {
        freq = 0;
    }
}

int main() {
    ADC_init();
    
    while(1) {
        ADC_start_conversion();
    }
}
