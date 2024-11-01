#define OP_NORMAL 0
#define EMERGENCY_STOP 1

#define BELT (1<<PD6)
#define LED (1<<PB0)
#define BELT_INTERVAL 5000

//Global state variable
unsigned char state = OP_NORMAL;

int main ()
{
  DDRB |= LED;
  DDRD |= BELT;
  PORTB &= ~ LED
  PORTD &= ~ BELT;

  PCICR |= (1 << PCIE2);
  PCMSK2 |= (1 << PCINT18);

  sei();
    
  for(;;)
  {
    switch(state)
    {
      case OP_NORMAL:
      	PORTD ^= BELT;
        _delay_ms(BELT_INTERVAL);
        if(PCIFR & (1<<PCIFG2) != 0)
        {

        }
        break;
      
      case EMERGENCY_STOP:
        PORTB |= LED;
        PORTD |= BELT;
        break;
    }
  }

  return 0;
}

ISR(PCINT2_vect)
{
  PCIFR &= ~PCIFG2;
  PORTB |= LED;
  PORTD |= BELT;
  state = EMERGENCY_STOP;
}
