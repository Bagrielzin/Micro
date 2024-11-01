#include <avr/io.h>

int main(void) {

    //a) Configuração de interrupção para borda de descida no pino INT1
    EICRA |= (1 << ISC11);  // Configura para interrupção em borda de descida (INT1)
    // ISC11 = 1, ISC10 = 0 => Borda de descida no INT1

    EIMSK |= (1 << INT1);   // Habilita a interrupção externa no pino INT1

    //b) Configuração de interrupção para borda de subida no pino INT0
    EICRA |= (1 << ISC01) | (1 << ISC00);  // Configura para interrupção em borda de subida (INT0)
    // ISC01 = 1, ISC00 = 1 => Borda de subida no INT0

    EIMSK |= (1 << INT0);   // Habilita a interrupção externa no pino INT0

    //c) Configuração de interrupção para borda de descida nos pinos INT0 e INT1
    EICRA |= (1 << ISC01);  // Configura para interrupção em borda de descida (INT0)
    // ISC01 = 1, ISC00 = 0 => Borda de descida no INT0

    EICRA |= (1 << ISC11);  // Configura para interrupção em borda de descida (INT1)
    // ISC11 = 1, ISC10 = 0 => Borda de descida no INT1

    EIMSK |= (1 << INT0) | (1 << INT1);  // Habilita as interrupções externas nos pinos INT0 e INT1




    sei();


    return 0;
}  