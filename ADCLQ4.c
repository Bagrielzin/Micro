#include <avr/io.h>
#include <avr/interrupt.h>

void ADC_init(){
    ADMUX = (1 << REFS0) | (1 << REFS1); // Seleciona a referência de 1.1V
    ADCSRA = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Prescaler de 128
    ADCSRA |= (1 << ADEN); // Habilita o ADC
}

uint16_t ADC_read() {    
    ADCSRA |= (1 << ADSC); // Inicia a conversão
    while (ADCSRA & (1 << ADSC)); // Aguarda a conversão completar
    return ADC; // Retorna o resultado
}

int main() {
    ADC_init();
    uint16_t adc_result;
    uint16_t freq = 0;
    
    while(1) {
        adc_result = ADC_read(); // Lê o ADC
        
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
}
