// *******************************************************
//AVR_USART_ATMEGA328p_RX
// *******************************************************
//Specs of the USART:
// 1 start bit
// 8 data bits 
// no parity
// 1 stop bit
#define F_CPU 16000000UL

#include <stdio.h>
#include <avr/io.h> 
#include <avr/fuse.h>
#include <stdio.h>

#include <util/delay.h>
#include <avr/interrupt.h> 
#define BAUD 9600 //same rate as in the Makefile baud is in bits per second
#define ubbrn ((F_CPU/16/BAUD) - 1)
#define buffer_size 128

#define bufferSize 3


/* volatile uint8_t rxBuffer[bufferSize] = {0x00, 0x00, 0x00}; */
volatile char rxBuffer[bufferSize] = {'0', '0', '0'};
volatile int  rxWritePos = 0;
volatile int  rxReadPos = 0;

char num1 = '1'; 
char ser = '0';
char cmd = '0';
char syn = '0';

/* volatile uint8_t num1 = 0x1F; */
/* volatile uint8_t ser = 0x00; */
/* volatile uint8_t cmd = 0x00; */
/* volatile uint8_t syn = 0x00; */


char appendRx(void)
{
    char ret = '\0';
    if (rxReadPos != rxWritePos){
        ret = rxBuffer[rxReadPos];
        rxReadPos++;
        if(rxReadPos >= bufferSize){
            rxReadPos = 0;
        }
    }
    return ret;
}



/* void rxPacket(char rxByte, char command){ */
/*     appendTx(syncByte); */
/*     appendTx(rxByte); */
/*     appendTx(command); */
/*     nullByteIfEmpty(); */
/* } */



int main(void){
    DDRB |= (1 << PB5) | (1 << PB4) | (1 << PB3) | (1 << PB2) | (1 << PB1);
    PORTB &= ~ (1 << PB5) | (1 << PB4) | (1 << PB3) | (1 << PB2) | ( 1 << PB1 );

    DDRD |= (1 << PD7) | (1 << PD6);
    PORTD &=~ (1 << PD7)| ( 1 << PD6 );
    /* PORTB |= (1 << PB1); */
    //High and low bits
    UBRR0H = (ubbrn >> 8); 
    UBRR0L = ubbrn; 
    ////////////////
    uint8_t addr = 0x00;

    UCSR0A = 0x00;
    //transimit and recieve enable
    UCSR0B =  (1 << RXEN0) | (1 << RXCIE0) | (1 << TXEN0) | (1 << TXCIE0);
    // UCSR0B = (1 << RXEN0) | (1 << RXCIE0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);  //8 bit data format
    sei();
    ////////////////////////////////////////////////////////////////
    UDR0 = 0;
    while (1){

        /* char c = appendRx(); */
        char syn = appendRx();
        char ser = appendRx();
        char cmd = appendRx();
        UDR0 = '0';
        UDR0 = syn;
        /* UDR0 = '0'; */
        /* UDR0 = ser; */
        /* UDR0 = '0'; */
        /* UDR0 = cmd; */

       /* if ( (ser == num1) ){ */
       /*     /\* PORTD &=~ ( 1 << PD6 ); *\/ */
       /*       switch (cmd){ */
       /*          case ('f'): */
       /*              PORTB |= (1 << PB1); */
       /*              break; */
       /*          case('b'): */
       /*              PORTB |= (1 << PB4); */
       /*              break; */
       /*          case('l'): */
       /*              PORTB |=  (1 << PB3); */
       /*              break; */
       /*          case('r'): */
       /*              PORTB |=  (1 << PB2); */
       /*              break; */
       /*          case('n'): */
       /*              PORTB &=~ (1 << PB1); */
       /*              PORTB &=~ (1 << PB4); */
       /*              PORTB &=~ (1 << PB3); */
       /*              PORTB &=~ (1 << PB2); */
       /*              break; */
       /*      } */
       /*  } */
       /*  else{ */
       /*           PORTD |= (1 << PD6); */
       /*          /\* PORTB &=~ (1 << PB5); *\/ */
       /*          /\* PORTB &=~ (1 << PB4); *\/ */
       /*          /\* PORTB &=~ (1 << PB3); *\/ */
       /*          /\* PORTB &=~ (1 << PB2); *\/ */
       /*  } */
        }

    return 0;
}  

ISR(USART_RX_vect)
{
    rxBuffer[rxWritePos] = UDR0;
    UDR0 = rxBuffer[rxWritePos];
    rxWritePos++;
    if (rxWritePos >= bufferSize){
        rxWritePos = 0;
    }
}

ISR(BADISR_vect){
    PORTD ^= PD7;
}
