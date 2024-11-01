#include <avr/io.h>

int main(void) {

    //Questão 1
    DDRD |= (1 << PD2);
    PORTD |= (1 << PD2);

    //Questão 2
    PORTD &= ~(1 << PD2); 

    //Questão 3
    DDRD |= (1 << PD3);
    DDRB |= (1 << PB4);
    while(1){
        PORTD ^= (1 << PD3);
        PORTB ^= (1 << PB4);  
    }

    //Questão 4
    DDRD &= ~(1 << PD5);
    PORTD |= (1 << PD5);
    if(PIND & (1 << PD5)){

    }

    //Questão 5
    DDRD &= ~(1 << PD5);
    PORTD |= (1 << PD5);
    if(PIND & (1 << PD5)){
        PORTD &= ~(1 << PD5);
    }
    

    //Questão 6
    DDRD |= (1 << PD6) | (1 << PD7);
    PORTD &= ~(1 << PD6);

    //Questão 7
    DDRD &= ~(1 << PD2);
    DDRD |= (1 << PD3);
    while(1){
        if(PIND & (1 << PD2)){
            PORTD |= (1 << PD3);
        }
    }

    //Questão 8
    DDRD &= ~(1 << PD2);
    DDRD |= (1 << PD3);
    while(1){
        if(PIND & (1 << PD2)){
            PORTD &= ~(1 << PD3);
        }
    }

    //Questão 9
    DDRD |= (1 << PD4) | (1 << PD5) | (1 << PD6);

    //Questão 10
    DDRD &= ~(1 << PD0);
    DDRD &= ~(1 << PD1);
    DDRD &= ~(1 << PD2);
    if ((DDRD & ((1 << PD0) | (1 << PD1) | (1 << PD2))) == 0){
        //
    }

    //Questão 11
    DDRD &= ~(1 << PD0);
    PORTD |= (1 << PD0); //resistor pull-up
    DDRD |= (1 << PD1);

    while (1) {
        // Verifica se o botão foi pressionado (nível baixo)
        if (!(PIND & (1 << PD0))) { 
            PORTD ^= (1 << PD1); // Alterna o estado do LED
        }
    }

    //Questão 12
    DDRD &= ~(1 << PD0);
    PORTD |= (1 << PD0); //resistor pull-up
    DDRD |= (1 << PD1);
    DDRD |= (1 << PD2);

    while(1){
        // Verifica se o botão foi pressionado (nível baixo)
        if (!(PIND & (1 << PD0))) {
            //Verifica se os LEDS estão com valores iguais
            if(((PIND & (1 << PD1)) && (PIND & (1 << PD2))) || (!(PIND & (1 << PD1)) && !(PIND & (1 << PD2)))){
                PORTD ^= (1 << PD1); // Alterna o estado do LED1
            }
            //Verifica se os LEDS estão com valores alternados
            if(((PIND & (1 << PD1)) && !(PIND & (1 << PD2))) || (!(PIND & (1 << PD1)) && PIND & (1 << PD2))){
                PORTD ^= (1 << PD1); // Alterna o estado do LED1
                PORTD ^= (1 << PD2); // Alterna o estado do LED2
            }
        }
    }

    //Questão 13
    DDRB &= ~(1 << PB0);
    DDRB &= ~(1 << PB1);
    DDRB &= ~(1 << PB2);
    DDRB &= ~(1 << PB3);
    DDRB &= ~(1 << PB4);
    DDRD |= (1 << PD1); // Led 1
    DDRC |= (1 << PC2); // Led 2

    while(1){
        //Verifica se eles estão intercalados
        uint8_t interc1 = PINB &(1 << PB0) && !(PINB &(1 << PB1)) && PINB &(1 << PB2) && !(PINB &(1 << PB3)) && PINB &(1 << PB4);
        uint8_t interc2 = !(PINB &(1 << PB0)) && PINB &(1 << PB1) && !(PINB &(1 << PB2)) && PINB &(1 << PB3) && !(PINB &(1 << PB4));
        
        //Verifica se eles estão todos ligados ou apagados
        uint8_t allLow = !(PINB &(1 << PB0)) && !(PINB &(1 << PB1)) && !(PINB &(1 << PB2)) && !(PINB &(1 << PB3)) && !(PINB &(1 << PB4));
        uint8_t allHigh = PINB &(1 << PB0) && PINB &(1 << PB1) && PINB &(1 << PB2) && PINB &(1 << PB3) && PINB &(1 << PB4);

        if(interc1 || interc2){
            PORTD |= (1 << PD1);
            PORTC &= ~(1 << PC2);
        }

        else if(allLow || allHigh){
            PORTD &= ~(1 << PD1);
            PORTD |= (1 << PC2);
        }

        else{
            PORTD &= ~(1 << PD1);
            PORTC &= ~(1 << PC2);
        }
    }

    //Questão 14
    DDRB &= ~(1 << PB1);
    DDRB &= ~(1 << PB2);
    DDRB &= ~(1 << PB3);
    DDRB &= ~(1 << PB4);
    DDRD |= (1 << PD3);// trava do sistema
    PORTB |= (1 << PB1) | (1 << PB2) | (1 << PB3) | (1 << PB4);
    while(1){
        if(!(PINB & (1 << PB2)))
            if(!(PINB & (1 << PB2)) && !(PINB & (1 << PB3)))
                if(!(PINB & (1 << PB2)) && !(PINB & (1 << PB3)) && !(PINB & (1 << PB4)))   
                    if(!(PINB & (1 << PB2)) && !(PINB & (1 << PB3)) && !(PINB & (1 << PB4)) && !(PINB & (1 << PB1))     )
                        PORTD |= (1 << PD3);     
    }


}   
