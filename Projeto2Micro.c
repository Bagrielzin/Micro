#define SENSOR_QUARTO_CHANNEL 0
#define SENSOR_SALA_CHANNEL 1
uint8_t channel;

// Variáveis para entrada e saída
char RX_buffer[32];
char RX_index = 0;

char msg_tx[20];

// Buffer para estado anterior do RX
char old_rx_hs[32];

// A inicialização do UART consiste em definir a taxa de transmissão,
// definir o formato de quadro, e ativar o Transmissor ou o receptor.

// Função de configuração do UART
void UART_init(int baud)
{
    // Calcula a taxa de transmissão
    int MYUBRR = 16000000 / 16 / baud - 1;

    // Definindo a taxa de transmissão
    UBRR0H = (unsigned char)(MYUBRR >> 8);
    UBRR0L = (unsigned char)(MYUBRR);

    // Definindo o formato de quadro, 8 bits e 1 stop bit
    UCSR0C = (0 << USBS0) | (1 << UCSZ00) | (1 << UCSZ01);

    // Ativa o Transmissor, receptor e define a interrupção
    UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
}

// Função para envio de dados via UART
void UART_send(char *TX_buffer)
{
    // Enquanto tiver caracteres para enviar
    while (*TX_buffer != 0)
    {
        // Prepara o buffer para o envio
        UDR0 = *TX_buffer;

        // Espera o envio ser completado
        while (!(UCSR0A & (1 << UDRE0))){};

        // Avança o ponteiro do buffer
        TX_buffer++;
    }
}

// Limpa o buffer de entrada e saída
void limpa_RX_buffer(void)
{
    unsigned char dummy;

    // Enquanto houver dados no buffer
    while (UCSR0A & (1 << RXC0))
    {
        // Lê o dado
        dummy = UDR0;
    }

    // Reseta o índice
    RX_index = 0;

    // Limpa todos os dados do buffer
    for (int i = 0; i < 32; i++)
    {
        old_rx_hs[i] = RX_buffer[i];
        RX_buffer[i] = 0;
    }
}

// Função ISR que salva um array de dados recebidos via UART
ISR(USART_RX_vect)
{
    // Salva o dado recebido
    RX_buffer[RX_index] = UDR0;
    RX_buffer[RX_index +1] = 0;

    //UART_send("Recebido\n");

    // Adiciona mais 1 na contagem
    RX_index++;
}

void setup_INT(){
    EICRA |= (1 << ISC11) | (1 << ISC01); // configura INT0 e INT1 na transição de descida
    EIMSK |= (1 << INT0) | (1 << INT1); // habilita as interrupções no INT0 e INT1
    PORTD |= (1 << PD2) | (1 << PD3); // pull-up dos botões
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
ISR(ADC_vect)
{
    adc_result = ADC;

    uint8_t porcentagem_iluminacao = (adc_result * 100) / 1023;

    if (channel == SENSOR_QUARTO_CHANNEL)
    {
        UART_send("Porcentagem de iluminação da luz do quarto: ");
    }
    else if (channel == SENSOR_SALA_CHANNEL)
    {
        UART_send("Porcentagem de iluminação da luz da sala: ");
    }

    char porcentagem_str[5];
    itoa(porcentagem_iluminacao, porcentagem_str, 10);
    UART_send(porcentagem_str);
    UART_send("%\n");

    // Limpa o buffer de recepção da UART após a leitura do ADC
    limpa_RX_buffer();  
}

int main(){
    
    UART_init(9600); // Configuração do UART
    setup_INT();
    ADC_init();
    sei();

    while(1){
        //
    }
}