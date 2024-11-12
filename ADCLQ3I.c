#include <avr/io.h>
#include <avr/interrupt.h>

#define SENSOR_LUZ_CHANNEL 0  // Canal ADC para sensor de luz
#define SENSOR_GAS_CHANNEL 1   // Canal ADC para sensor de gás

int sensores[2];  // Vetor para armazenar leituras dos sensores

void initADC() {
    ADMUX = (1 << REFS0); // AVcc como referência, pino ADC0
    ADCSRA = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Prescaler de 128
    ADCSRA |= (1 << ADEN) | (1 << ADIE); // Habilita o ADC
    sei(); // Habilita interrupções
}

void ADC_start_conversion(uint8_t channel) {
    ADMUX = (ADMUX & 0xF8) | (channel & 0x07); // Seleciona o canal ADC
    ADCSRA |= (1 << ADSC);                  // Inicia a conversão
}

uint8_t adc_result;
uint8_t channel = SENSOR_LUZ_CHANNEL;
ISR(TIMER1_COMPA_vect) {
    ADC_start_conversion(channel);
}

ISR(ADC_vect){
    adc_result = ADC;
    if(channel == SENSOR_LUZ_CHANNEL){
        sensores[channel] = (adc_result * 80) / 1023 + 10;
        channel = SENSOR_GAS_CHANNEL;
    }
    else if(channel == SENSOR_GAS_CHANNEL){
        sensores[channel] = (adc_result * 100) / 1023;
        channel = SENSOR_LUZ_CHANNEL;
    }
}

int main(void) {
    initADC(); // Inicializa o ADC
    TCCR1A = 0;
    TCCR1B = (1 << WGM12)| (1 << CS12) | (1 << CS10);// Modo CTC, Prescaler de 1024
    OCR1A = 5207; // (F_CPU = 16 MHz, f_des = 3Hz, prescaler = 1024)
    TIMSK1 |= (1 << OCIE1A); // Habilita interrupção de comparação

    while (1) {

    }
}
