#include <stdio.h>
#include <stdint.h>

#define SQUARE(n) (n*n)

int main(){

    int numero;
    scanf("%d",&numero);
    printf("%d",SQUARE(numero));

    return 0;
}