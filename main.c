#define F_CPU 1000000UL
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>  
#define butpress (~PINB) & (1 << PB3)
int main (void) { 
	init();
    	while (1)  
        { 	      

    	}  


     } 


void init(){  
	DDRB |= (1 << PB4);     
	DDRB &= ~ (1 << PB3); 
	PORTB |= (1 << PB4);  
	PORTB |=  (1 << PB3);    
	GIMSK |= (1 << PCIE); 
	PCMSK |= (1 << PCINT3); 
	sei();
	/*
	GTCCR |= ((1 << COM1B1) | (1 << PWM1B));  //PB4 enabled for output of pwm
	TIMSK |= (1 << TOIE1); 
	sei(); 
	TCCR1 |= (1 << CS10) | (1 << CS12) | (1 << CS11); //prescaler of 64 
	*/
} 

/*
ISR(TIMER1_OVF_vect){ 

	OCR1B = 250;

}  
*/ 

ISR(PCINT0_vect){ 
	_delay_ms(10);
	if (butpress)	{  
		PORTB &=~ (1 << PB4);
	}
	else {  
		PORTB |= (1 << PB4);
	}
}