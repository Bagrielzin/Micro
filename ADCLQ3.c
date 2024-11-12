#include <avr/io.h>
#include <avr/interrupt.h>

#define SENSOR_LUZ_CHANNEL 0  // Canal ADC para sensor de luz
#define SENSOR_GAS_CHANNEL 1   // Canal ADC para sensor de gás

int sensores[2];  // Vetor para armazenar leituras dos sensores

void initADC() {
    ADMUX = (1 << REFS0); // AVcc como referência, pino ADC0
    ADCSRA = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Prescaler de 128
    ADCSRA |= (1 << ADEN); // Habilita o ADC
}

uint16_t readADC(uint8_t channel) {
    ADMUX = (ADMUX & 0xF8) | channel; // Seleciona o canal ADC
    ADCSRA |= (1 << ADSC);                  // Inicia a conversão
    while (ADCSRA & (1 << ADSC));           // Aguarda a conversão terminar
    return ADC;                             // Retorna o valor lido
}

uint8_t channel  = SENSOR_LUZ_CHANNEL;
ISR(TIMER1_COMPA_vect) {
    uint16_t adc_result = readADC(channel);
    if(channel == SENSOR_LUZ_CHANNEL){
        // Converte a leitura para percentual (10 a 90%)
        sensores[0] = (adc_result * 80) / 1023 + 10;
        channel = SENSOR_GAS_CHANNEL;
    }
    else if(channel == SENSOR_GAS_CHANNEL){
        // Converte a leitura para percentual (0 a 100%)
        sensores[1] = (adc_result * 100) / 1023;
        channel = SENSOR_LUZ_CHANNEL;
    }
}

int main(void) {
    initADC(); // Inicializa o ADC
    TCCR1A = 0;
    TCCR1B = (1 << WGM11)| (1 << CS12) | (1 << CS10); // Modo CTC, Prescaler de 1024
    OCR1A = 5207; // (F_CPU = 16 MHz, f_des = 3Hz, prescaler = 1024)
    TIMSK1 |= (1 << OCIE1A); // Habilita interrupção de comparação
    sei();                                 // Habilita interrupções globais

    while (1) {
        //
    }
}
