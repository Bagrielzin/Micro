#include <avr/io.h>
#include <avr/interrupt.h>

// Variáveis globais para controlar o estado da bomba
volatile uint8_t bomba_ligada = 0;
volatile uint8_t nivel_baixo = 0;  // Indicador de nível baixo
volatile uint8_t nivel_alto = 0;   // Indicador de nível alto

void setup_timers() {
    // Frequencia da CPU da 16MHz
    // Configurando o Timer1 para gerar interrupção a cada 2 segundos (0,5 Hz)
    TCCR1B |= (1 << WGM12) | (1 << CS02) | (1 << CS00);  // Modo CTC com prescaler de 1024
    TCCR1A = 0;                                          // Define os bits de controle para o modo CTC
    OCR1A = 31249;                                       // Valor calculado para 0,5 Hz
    // OCR1A = (fcpu)/(prescaler * fdesejada) - 1
    TIMSK1 |= (1 << OCIE1A);                             // Habilita interrupção de comparação A do Timer1
}

void setup_external_interrupts() {
    // Configurando as interrupções externas para os sensores de nível
    EIMSK |= (1 << INT0) | (1 << INT1);  // Habilita INT0 e INT1
    EICRA |= (1 << ISC01) | (1 << ISC11);  // Borda de descida para ambas as interrupções
}

ISR(TIMER1_COMPA_vect) {
    // Interrupção do temporizador: ocorre a cada 2 segundos (0,5 Hz)
    if (bomba_ligada) {
        // Lógica para desligar a bomba se o nível alto for atingido
        if (nivel_alto) {
            PORTB &= ~(1 << PB0);  // Desligar a bomba (PB0 = 0)
            bomba_ligada = 0;      // Atualiza o estado da bomba
        } else {
            PORTB ^= (1 << PB0);  // Alterna o estado da bomba (liga/desliga)
        }
    }
}

ISR(INT0_vect) {
    // Interrupção do sensor de nível alto (INT0)
    nivel_alto = 1;  // Indica que o nível alto foi atingido
    bomba_ligada = 0;  // Desliga a bomba
    PORTB &= ~(1 << PB0);  // Garante que a bomba está desligada (PB0 = 0)
}

ISR(INT1_vect) {
    // Interrupção do sensor de nível baixo (INT1)
    nivel_baixo = 1;  // Indica que o nível baixo foi detectado
    nivel_alto = 0;   // O nível alto não está mais ativo
    bomba_ligada = 1;  // Liga a bomba
}

int main() {
    // Configuração dos pinos
    DDRB |= (1 << PB0);  // Configura o PB0 como saída para controlar a bomba
    PORTB &= ~(1 << PB0);  // Garante que a bomba está desligada inicialmente

    // Configuração do temporizador e interrupções externas
    setup_timers();
    setup_external_interrupts();
    
    sei();  // Habilita interrupções globais
    
    while (1) {
        // Loop principal do programa
        // Monitoramento e ações extras podem ser adicionados aqui
        // O controle da bomba está nas interrupções
    }
    
    return 0;
}

/*tipos de ISR com timer:
    n = [0,1,2] x = [A,B]
    (TIMERn_COMPx_vect)
    (TIMERn_OVF_vect)
    (TIMER1_CAPT_vect) => exclusivo do timer 1   
*/