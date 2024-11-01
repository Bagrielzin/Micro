#include <stdio.h>

#define DESLIGADO 0
#define INDO_ELEV 1
#define SUBINDO_ELEV 2
#define INDO_VERIF 3
#define VERIF 4
#define INDO_CARR 5

void main(void){

    char M1 = 0;
    char M2 = 0;
    char ME = 0;
    char L = 0;
    char S1 = 0;
    char S2 = 0;
    char S3 = 0;
    char S4 = 0;
    char LIGA = 0;
    char DESLIGA = 0;
    char TEMPO = 0;

    char estado = 0;

    switch (estado) {
    case DESLIGADO:
        M1 = 0;
        M2 = 0;
        ME = 0;
        L = 0;

        if(LIGA == 1)
            estado = INDO_ELEV;
        break;

    case INDO_ELEV:
        M1 = 1;
        M2 = 0;
        ME = 0;
        L = 0;

        if(DESLIGADO == 1)
            estado = DESLIGADO;
        else if(S1 == 1)
            estado = SUBINDO_ELEV;
        break;

    case SUBINDO_ELEV:
        M1 = 0;
        M2 = 0;
        ME = 1;
        L = 0;
        
        if(DESLIGADO == 1)
            estado = DESLIGADO;
        else if(S2 == 1)
            estado = INDO_VERIF;
        break;

    case INDO_VERIF:
        M1 = 0;
        M2 = 1;
        ME = 0;
        L = 0;
        
        if(DESLIGADO == 1)
            estado = DESLIGADO;
        else if(S3 == 1)
            estado = VERIF;
        break;

    case VERIF:
        M1 = 0;
        M2 = 0;
        ME = 0;
        L = 1;
        
        if(DESLIGADO == 1)
            estado = DESLIGADO;
        else{    
            delay(3000);
            estado = INDO_CARR;
        }
        break;

    case INDO_CARR:
        M1 = 0;
        M2 = 2;
        ME = 0;
        L = 0;
        
        if(DESLIGADO == 1 || S4 == 1)
            estado = DESLIGADO;
        break;

    default:
        break;
    }
}