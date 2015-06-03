#include <avr/interrupt.h>
#include "uart.h"

#include <util/delay.h>
 
enum {
 BLINK_DELAY_MS = 1000,
};
 
int main (void) {
    
    sei();

    /* set pin 7 of PORTB for output*/
    DDRB |= _BV(DDB7);

    uart0_init(UART_BAUD_SELECT(9600, F_CPU));

    /* while (1) { */
    /*   /\* set pin 7 high to turn led on *\/ */
    /*   PORTB |= _BV(PORTB7); */
    /*   _delay_ms(BLINK_DELAY_MS); */

    /*   uart0_putc(65); */

    /*   /\* set pin 5 low to turn led off *\/ */
    /*   PORTB &= ~_BV(PORTB7); */
    /*   _delay_ms(BLINK_DELAY_MS); */
    //}
            

    while(1)
    {
       uint16_t v = uart0_getc();
       if ((v & 0xff00) == 0)
       {
	   uint8_t c = v & 0xff; 
            uart0_putc(c);
       }
    } 
    return 0;
}
