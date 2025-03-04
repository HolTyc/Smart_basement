#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay_basic.h>
#include <stdlib.h>

void delay_ms(int ms);
void adcInit();
void ddrInit();
int adcRead(unsigned char channel);

int main ()
{
    ddrInit();
    adcInit();

    int aData = 0;

    while(1) {
    	aData = (adcRead(0));
	if (aData>=1024/2) {
		PORTD |= (1<<PORTD2);
	} else {
		PORTD &= ~(1<<PORTD2);
	}
    }
}
void delay_ms(int ms) {
	for (int i = 0; i < ms; i++) {
		_delay_loop_2(4000);
	}
}
void adcInit(){
	// set REFS1 = 0 |REFS0 = 1 (Vref as AVCC pin) | ADLAR = 0(right adjusted) |  MUX4 to MUX0 is 0000 for ADC0
	ADMUX = 0b01000000; 
	//enable ADC module, set prescalar of 128 which gives CLK/128 	
	ADCSRA |= (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
}
void ddrInit(){
	DDRD |= (1<<PORTD2);
	PORTD &= ~(1<<PORTD2);
}
int adcRead(unsigned char channel) {
	/* set input channel to read */
	ADMUX = 0x40 | (channel & 0x07);   // 0100 0000 | (channel & 0000 0100)
	/* Start ADC conversion */
	ADCSRA |= (1<<ADSC);
	/* Wait until end of conversion by polling ADC interrupt flag */
	while (!(ADCSRA & (1<<ADIF)));
	/* Clear interrupt flag */
	ADCSRA |= (1<<ADIF);
	delay_ms(5);                      /* Wait a little bit */
 	/* Return ADC word */
	return ADCW;
}
