#include <avr/io.h>
#include <stdint.h>

void ADC_init() {
    // Configura a referência de tensão (AREF) para 360 mV (exemplo teórico)
    ADMUX = (1 << REFS0);  // Assume AREF ou uma fonte de tensão externa ajustada para 360 mV
    
    // Configura o prescaler para 128, para uma frequência de ADC de 125 kHz (16 MHz / 128)
    ADCSRA = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    
    // Habilita o ADC
    ADCSRA |= (1 << ADEN);
}

uint16_t ADC_read() {
    // Inicia a conversão ADC
    ADCSRA |= (1 << ADSC);

    // Espera até a conversão completar
    while (ADCSRA & (1 << ADSC));

    // Retorna o valor de 10 bits do ADC
    return ADC;
}

float convert_to_temperature(uint16_t adc_value) {
    // Converte a leitura do ADC para temperatura em °C (0 a 90 °C)
    return (adc_value * 90.0) / 1023.0;
}

int main() {
    uint16_t adc_result;
    float temperature;

    ADC_init();  // Inicializa o ADC

    while (1) {
        adc_result = ADC_read();  // Lê o valor do ADC
        temperature = convert_to_temperature(adc_result);  // Converte para temperatura
        // Agora você pode usar o valor de 'temperature' em °C
    }

    return 0;
}
