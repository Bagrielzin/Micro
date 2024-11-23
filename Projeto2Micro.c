#define SENSOR_QUARTO_CHANNEL 0
#define SENSOR_SALA_CHANNEL 1

uint8_t channel;
uint16_t adc_result;

// Função de configuração do UART
void UART_init(int baud)
{
    int MYUBRR = 16000000 / 16 / baud - 1;
    UBRR0H = (unsigned char)(MYUBRR >> 8);
    UBRR0L = (unsigned char)(MYUBRR);
    UCSR0C = (1 << UCSZ00) | (1 << UCSZ01); // Formato 8N1
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);   // Ativa TX e RX
}

// Função para envio de dados via UART
void UART_send(const char *TX_buffer)
{
    while (*TX_buffer)
    {
        UDR0 = *TX_buffer;
        while (!(UCSR0A & (1 << UDRE0))) {}
        TX_buffer++;
    }
}

// Configuração de interrupções externas
void setup_INT()
{
    EICRA |= (1 << ISC11) | (1 << ISC01); // Transição de descida em INT0 e INT1
    EIMSK |= (1 << INT0) | (1 << INT1);   // Habilita INT0 e INT1
    PORTD |= (1 << PD2) | (1 << PD3);     // Pull-up nos pinos PD2 e PD3
}

// Configuração do ADC
void ADC_init()
{
    ADMUX = (1 << REFS0);                                 // Vref = AVCC
    ADCSRA = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);  // Prescaler = 128
    ADCSRA |= (1 << ADEN) | (1 << ADIE);                  // Liga ADC e interrupção
}

// Inicia conversão ADC
void ADC_start_conversion(uint8_t channel)
{
    ADMUX = (ADMUX & 0xF8) | (channel & 0x07); // Seleciona o canal
    ADCSRA |= (1 << ADSC);                     // Inicia conversão
}

// Interrupção de INT0
ISR(INT0_vect)
{
    UART_send("Pessoa presente no quarto: ");
    channel = SENSOR_QUARTO_CHANNEL;
    ADC_start_conversion(channel);
}

// Interrupção de INT1
ISR(INT1_vect)
{
    UART_send("Pessoa presente na sala: ");
    channel = SENSOR_SALA_CHANNEL;
    ADC_start_conversion(channel);
}

// Interrupção do ADC
ISR(ADC_vect)
{
    adc_result = ADC;
    uint8_t porcentagem_iluminacao = (adc_result * 100) / 1023;

    if (channel == SENSOR_QUARTO_CHANNEL)
        UART_send("Porcentagem de iluminação do quarto: ");
    else if (channel == SENSOR_SALA_CHANNEL)
        UART_send("Porcentagem de iluminação da sala: ");

    char porcentagem_str[5];
    itoa(porcentagem_iluminacao, porcentagem_str, 10);
    UART_send(porcentagem_str);
    UART_send("%\n");
}

int main()
{
    UART_init(9600); // Configuração UART
    setup_INT();     // Configuração interrupções externas
    ADC_init();      // Configuração ADC
    sei();           // Habilita interrupções globais

    while (1)
    {
        // Loop principal vazio
    }
}
