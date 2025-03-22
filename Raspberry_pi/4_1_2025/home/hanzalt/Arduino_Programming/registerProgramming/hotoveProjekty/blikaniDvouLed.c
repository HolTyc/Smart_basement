#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay_basic.h>

int main (void)
{
    DDRD |= (1<<PORTD2) | (1<<PORTD3) | (1<<PORTD4); // Definovani 3 LED - arduino D2, D3, D4
    PORTD &= ~(1<<PORTD2) & ~(1<<PORTD3) & ~(1<<PORTD4); // Zhasnuti vsech LED
    while(1) {
        PORTD |= (1<<PORTD2) | (1<<PORTD3); // Rozsviceni prvnich dvou LED
	PORTD &= ~(1<<PORTD4); // Zhasnuti posledni LED
	
        for (int i = 0; i < 200; i++) {
		_delay_loop_2(4000);
	}

	PORTD |= (1<<PORTD4); // Rozsviceni posledni LED
        PORTD &= ~(1<<PORTD2) & ~(1<<PORTD3); // Zhasnuti prvnich dvou LED
	for (int i = 0; i < 100; i++) {
		_delay_loop_2(4000);
	}
    }
}
