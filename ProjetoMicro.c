#define SENSOR_QUARTO_CHANNEL 0
#define SENSOR_SALA_CHANNEL 1
uint8_t channel;

#define LUZ_QUARTO OCR0A
#define LUZ_SALA OCR0B

void setup_INT(){
    EICRA |= (1 << ISC11) | (1 << ISC01); // configura INT0 e INT1 na transição de descida
    EIMSK |= (1 << INT0) | (1 << INT1); // habilita as interrupções no INT0 e INT1
    PORTD |= (1 << PD2) | (1 << PD3); // pull-up dos botões
}

void setup_PWM(){
    TCCR0A |= (1 << COM0A1) | (1 << COM0B1) | (1 << WGM00) | (1 << WGM01); // ativa PWM para os canais A e B, Fast-PWM 8bit
    TCCR0B |= (1 << CS02) | (1 << CS01); // prescaler 1024
    DDRD |= (1 << PD5) | (1 << PD6); // configura os pinos OC0A e OC0B como saídas
    LUZ_QUARTO = 0;
    LUZ_SALA = 0;
}

void ADC_init(){
    ADMUX = (1 << REFS0); // Vref
    ADCSRA = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // prescaler 1024
    ADCSRA |= (1 << ADEN) | (1 << ADIE); // liga o conversor e habilita interrupção
}

void ADC_start_conversion(uint8_t channel) {
    ADMUX = (ADMUX & 0xF8) | (channel & 0x07); // Seleciona o canal ADC
    ADCSRA |= (1 << ADSC); // Inicia a conversão
}

ISR(INT0_vect){
    channel = SENSOR_QUARTO_CHANNEL;
    ADC_start_conversion(channel);
}

ISR(INT1_vect){
    channel = SENSOR_SALA_CHANNEL;
    ADC_start_conversion(channel);
}

uint16_t adc_result;
ISR(ADC_vect){
    adc_result = ADC;
    // Converte o a leitura do ADC pro led PWM
    if(channel == SENSOR_QUARTO_CHANNEL){
        LUZ_QUARTO = adc_result/4;
    }
    else if(channel == SENSOR_SALA_CHANNEL){
        LUZ_SALA = adc_result/4;
    }
}

int main(){
    setup_INT();
    setup_PWM();
    ADC_init();
    sei();

    while(1){
        //
    }
}