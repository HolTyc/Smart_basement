#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

int main (void)
{
    DDRB |= _BV(PORTB0);      // set
    for (;;) {
        PORTB |= _BV(PORTB0);  // set pin 0 of port B high
        _delay_ms(2000);
        PORTB &= ~_BV(PORTB0); // set pin 0 of port B low
       	_delay_ms(2000);
    }
}
