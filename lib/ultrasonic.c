/*
 * 
 * code needs to be moved in asuro.c to be compilable
 */

#include <util/delay.h>
#include "asuro.h"
#include "ultrasonic.h"

int count40kHz;

/**
 * being used insted TIMER2_OVF_vect during ultrasonic polling
 */
ISR(TIMER2_COMP_vect)
{
	count40kHz++;
}

/**
 * initialises the Ultrasonic module
 * this function is automaticly called by Chirp
 */
void InitUltrasonics(void)
{
	// Change Oscillator-frequency of Timer 2
	// to 40kHz, no toggling of IO-pin:
  cli();
  TCCR2  = (1 << WGM21) | (1 << CS20);
	OCR2   = 100;              // 40kHz @8MHz crystal
	TIMSK  = (1 << OCIE2);     // OCIE2:  Timer/Counter2 Output Compare Match Interrupt Enable
	
	ADCSRA = (0 << ADEN);      // deactivate ADC
	ACSR  |= (1 << ACIS1);     // Comparator Interrupt on Falling Output Edge

	ADMUX  = 0x03;             // connect ADC3-input with comparator
	SFIOR |= (1 << ACME);      // connect ADC multiplexer to comparator
	DDRD  &= ~(1 << 6);        // use Port D Pin 6 as input (AIN0)
  sei();
}

/**
 * restores the hardware after using the Ultrasonic module
 * this function is called automaticly after a Chirp
 */
void RestoreAsuro(void)
{
  cli();
  TCCR2 = (1 << WGM20) | (1 << WGM21) | (1 << COM20) | (1 << COM21) | (1 << CS20);
	OCR2  = 0x91;              // duty cycle for 36kHz
	TIMSK &= ~(1 << OCIE2);     // OCIE2:  Timer/Counter2 Output Compare Match Interrupt Disable

	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1); // clk/64
	ACSR  |= (0 << ACIS1);

	if(autoencode) {
		EncoderInit();
	}
	sei();
	_delay_us(1);
}

/**
 * @return distance in cm
 */
int Chirp(void)
{
	unsigned int sleeptime = 0, dist = 0;

	InitUltrasonics();

	// chripen:
	count40kHz = 0;

	while(count40kHz != 20) {
		OCR2 = 100 + 20 / 2 - count40kHz;
	}

	TCCR2	= (1 << WGM21) | (1 << CS20);
	OCR2	= 100;

	// analyse echoes:
	while(TRUE) {
		_delay_us(1);
		sleeptime++;

		if((ACSR & (1 << ACI))) {
			dist = (unsigned int) ((long) ((344L * ((sleeptime * 1000L) / 72L) / 10000L) / 2L));
			ACSR |= (1 << ACI);
			break;
		}
		
		ACSR |= (1 << ACI);
		
		if(sleeptime > 3500) {
			return -1;
		}
	}
	
	RestoreAsuro();
	return dist;
}
