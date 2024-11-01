#include <stdio.h>
#include <stdint.h>

#define MOTOR1 0b00000001
#define MOTOR2 0b00000010
#define MOTOR3 0b00000100
#define MOTOR4 0b00001000
#define MOTOR5 0b00010000

void activate_sensor (uint8_t *state, uint8_t sensor){
    *state = *state | sensor;
}

int main(){

    int i;
    int system_state;
    activate_sensor(&system_state, MOTOR1 + MOTOR2 + MOTOR3 + MOTOR4 + MOTOR5);

    for(i=8; i>0; i--){
        if(system_state &(1<<i)){
            printf("1");
        } else{
            printf("0");
        }
    }

    return 0;
}