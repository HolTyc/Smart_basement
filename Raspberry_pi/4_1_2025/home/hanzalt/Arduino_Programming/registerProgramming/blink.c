#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay_basic.h>

int main (void)
{
    DDRB |= _BV(PORTB0);      // set pin 0 of port B as an output pin
    unsigned long delayTime = 6250;
    for (;;) {
        PORTB |= _BV(PORTB0);  // set pin 0 of port B high
         for (int i = 0; i < 200; i++) {
		_delay_loop_2(4000);
	}
        PORTB &= ~_BV(PORTB0); // set pin 0 of port B low
       	 for (int i = 0; i < 200; i++) {
		_delay_loop_2(4000);
	}
    }
}
