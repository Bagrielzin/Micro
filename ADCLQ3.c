#include <avr/io.h>
#include <avr/interrupt.h>

#define SENSOR_LUZ_CHANNEL 0  // Canal ADC para sensor de luz
#define SENSOR_GAS_CHANNEL 1   // Canal ADC para sensor de gás

int sensores[2];  // Vetor para armazenar leituras dos sensores

void initADC() {
    ADMUX = (1 << REFS0);                 // Referência de tensão AVcc, canal 0
    ADCSRA = (1 << ADEN) |                // Habilitar ADC
              (1 << ADPS2) | (1 << ADPS1); // Divisor de clock 64 (F_CPU/64)
}

uint16_t readADC(uint8_t channel) {
    ADMUX = (ADMUX & 0xF8) | (channel & 0x07); // Seleciona o canal ADC
    ADCSRA |= (1 << ADSC);                  // Inicia a conversão
    while (ADCSRA & (1 << ADSC));           // Aguarda a conversão terminar
    return ADC;                             // Retorna o valor lido
}

void readSensors() {
    // Lê o sensor de luz
    uint16_t luzRaw = readADC(SENSOR_LUZ_CHANNEL);
    // Converte a leitura para percentual (10 a 90%)
    sensores[0] = (luzRaw * 80) / 1023 + 10;

    // Lê o sensor de gás
    uint16_t gasRaw = readADC(SENSOR_GAS_CHANNEL);
    // Converte a leitura para percentual (0 a 100%)
    sensores[1] = (gasRaw * 100) / 1023;
}

// Configura o Timer 0 para gerar uma interrupção a cada 333 ms
void initTimer() {
    TCCR0A = (1 << WGM01);                 // Modo CTC
    TCCR0B = (1 << CS02) | (1 << CS00);     // Prescaler de 1024
    OCR0A = 78;                            // Para 333 ms (F_CPU = 16 MHz, 333 ms)
    TIMSK0 |= (1 << OCIE0A);                // Habilita interrupção de comparação
}

ISR(TIMER0_COMPA_vect) {
    readSensors();                          // Lê os sensores quando o timer gera a interrupção
}

int main(void) {
    initADC();                             // Inicializa o ADC
    initTimer();                           // Inicializa o Timer
    sei();                                 // Habilita interrupções globais

    while (1) {
        readSensors();
    }
}
