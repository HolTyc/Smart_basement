#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay_basic.h>

int main (void)
{
    DDRD |= (1<<PORTD2); // Nastaveni Led pinu
    //DDRB |= (1<<PINB0); Myslel jsem si ze musim nastavit digital inpu
    PORTD &= ~(1<<PORTD2); // Zhasnuti vsech LED
    while(1) {
	if (PINB & (1<<PINB0)) {
		PORTD |= (1<<PORTD2);
	} else {
		PORTD &= ~(1<<PORTD2);
	}
    }
}
