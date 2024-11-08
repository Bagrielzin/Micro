#define freqCPU 4000000
/* Configuração básica
int main(){

    TCCR0A |= (1 << WGM01); // modo CTC
    TCCR0B |= (1 << CS02) | (1 << CS00); // prescaler 1024
    TIMSK0 |= (1 < OCIE0A); // habilita interrupção no timer 0 A
    OCR0A = (35)/(1024*(1/freqCPU)) - 1;
    sei();

    while(1){
    
    }

    return 0;
}

ISR(TIMER0_COMPA_vect){
    //...
}*/

/* Piscando LED
int main(){
    TCCR0A |= (1 << WGM01); // modo CTC
    TCCR0B |= (1 << CS02) | (1 << CS00); // prescaler 1024
    TIMSK0 |= (1 < OCIE0A); // habilita interrupção no timer 0 A
    OCR0A = (5000)/(1024*(1/freqCPU)) - 1;
    DDRD |= (1 << PD0); //LED configurado como saída
    sei();

    while(1){
    
    }
}

ISR(TIMER0_COMPA_vect){
    PORTD ^= (1 << PD0);
}*/

/* Contagem de tempo
volatile uint8_t contador_binario = 0;

int main(){
    TCCR0A |= (1 << WGM01); // modo CTC
    TCCR0B |= (1 << CS02) | (1 << CS00); // prescaler 1024
    TIMSK0 |= (1 < OCIE0A); // habilita interrupção no timer 0 A
    OCR0A = (1000)/(1024*(1/freqCPU)) - 1;
    DDRD = 0xFF; //LEDs D 0-7 configurados como saída
    sei();

    while(1){
    
    }
}

ISR(TIMER0_COMPA_vect){
    contador_binario++;
    PORTD = contador_binario;
}*/

/* Controle de velocidade
uint8_t per = 5000;// período de 5000ms

int main(){
    TCCR0A |= (1 << WGM01); // modo CTC
    TCCR0B |= (1 << CS02) | (1 << CS00); // prescaler 1024
    TIMSK0 |= (1 < OCIE0A); // habilita interrupção no timer 0 A
    OCR0A = (per)/(1024*(1/freqCPU)) - 1;
    DDRD |= (1 << PD0); //LED configurado como saída

    EICRA |= (1 << ISC11) | (1 << ISC01); // Configura INT0 e INT1 para ativar em borda de descida
    EIMSK |= (1 << INT1) | (1 << INT0); // Habilita as interrupções INT0 e INT1

    sei();
    
    while(1){

    }
}

ISR(TIMER0_COMPA_vect){
    PORTD ^= (1 << PD0);
}

ISR(INT0_vect){ // mais velocidade
    per -= 100.9;
    uint8_t max = 0.1;
    if(per >= max){
        OCR0A = (uint8_t)((per)/(1024*(1/freqCPU)) - 1);
    }
    else{
        per = max;
    }
}

ISR(INT1_vect){ // menos velocidade
    per += 100.9;
    OCR0A = (uint8_t)((per)/(1024*(1/freqCPU)) - 1);
}*/

/* Contagem regressiva
volatile uint8_t contador_regressivo = 5; // 5 estados: 100%, 80%, 60%, 40%, 20%, 0%

int main(){
    TCCR0A |= (1 << WGM01); // modo CTC
    TCCR0B |= (1 << CS02) | (1 << CS00); // prescaler 1024
    TIMSK0 |= (1 < OCIE0A); // habilita interrupção no timer 0 A
    OCR0A = (2)/(1024*freqCPU) - 1;
    DDRB = 0xFF; // Configura PB0-PB4 como saída
    
    EICRA |= (1 << ISC11);
    EIMSK |= (1 << INT1);
    sei();

    while(1){
    
    }
}

ISR(TIMER1_COMPA_vect) {
    if (contador_regressivo > 0) {
        contador_regressivo--; // Decrementa contagem regressiva
        PORTB = (0b11111 >> (5 - contador_regressivo)); // Atualiza LEDs
    } else {
        PORTB ^= 0b11111; // Alterna todos os LEDs (pisca a 2 Hz)
    }
}

ISR(INT1_vect){
    contador = 5;
}*/