#define LED_PIN PD6           // Pino para o LED
#define BUTTON_PIN PD2        // Pino para o botão de leitura (INT0)
#define ADC_CHANNEL 0         // Canal ADC (ADC0)

volatile uint16_t leitura_anterior = 0;   // Última leitura do ADC
volatile uint8_t contador = 0;            // Contador de leituras
volatile uint8_t led_pisca = 0;           // Estado de piscar do LED

uint16_t leitura_adc() {
    ADCSRA |= (1 << ADSC);                // Inicia uma conversão ADC
    while (ADCSRA & (1 << ADSC));         // Aguarda a conversão terminar
    return ADC;
}

ISR(INT0_vect) {                          // Interrupção do botão LEITURA
    uint16_t leitura_atual = leitura_adc();  // Lê o valor atual do ADC

    // Ignora a primeira leitura
    if (leitura_anterior != 0) {
        if (leitura_atual <= (leitura_anterior * 0.8)) {  // Verifica se a leitura é 20% menor
            contador++;                    // Incrementa o contador
            if (contador == 5) {
                PORTD |= (1 << LED_PIN);   // Liga o LED ao atingir 5 contagens
            }
            else if (contador >= 10) {
                led_pisca = 1;             // Ativa o modo de pisca ao atingir 10 contagens
                TCCR1B |= (1 << CS12) | (1 << CS10);  // Habilita o Timer1 com prescaler de 1024
            }
        }
    }

    leitura_anterior = leitura_atual;      // Atualiza a leitura anterior
}

ISR(TIMER1_COMPA_vect) {                   // Interrupção para pisca do LED
    if (led_pisca) {
        PORTD ^= (1 << LED_PIN);           // Inverte o estado do LED a cada 250ms
    }
}

int main(){
    
    // Configuração do botão de leitura
    DDRD &= ~(1 << BUTTON_PIN);           // Configura botão como entrada
    PORTD |= (1 << BUTTON_PIN);           // Ativa pull-up interno

    // Configuração do LED
    DDRD |= (1 << LED_PIN);               // Configura LED como saída
    PORTD &= ~(1 << LED_PIN);             // LED inicialmente desligado

    // Configuração da interrupção externa INT0 (botão LEITURA)
    EICRA |= (1 << ISC01);                // INT0 na borda de descida
    EIMSK |= (1 << INT0);                 // Habilita interrupção INT0

    // Configuração do ADC
    ADMUX = (1 << REFS0) | ADC_CHANNEL;   // Referência AVcc e canal ADC0
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Habilita ADC e prescaler de 128

    // Configuração do Timer1 para controle de pisca do LED
    TCCR1B |= (1 << WGM12);               // Modo CTC (Clear Timer on Compare Match)
    OCR1A = 15624;                        // Define valor de comparação para 2 Hz (250ms ligado, 250ms desligado)
    TIMSK1 |= (1 << OCIE1A);              // Habilita interrupção de comparação

    return 0;
}