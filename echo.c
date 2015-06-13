#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"

#include <util/delay.h>

#ifdef __GNUC__
#  define UNUSED(x) UNUSED_ ## x __attribute__((__unused__))
#else
#  define UNUSED(x) UNUSED_ ## x
#endif

#ifdef __GNUC__
#  define UNUSED_FUNCTION(x) __attribute__((__unused__)) UNUSED_ ## x
#else
#  define UNUSED_FUNCTION(x) UNUSED_ ## x
#endif
 
enum {
 BLINK_DELAY_MS = 1000,
};

int uart0_send_byte(char data, FILE* UNUSED(stream))
{
    if (data == '\n')
    {
        uart0_putc('\r');
    }
    uart0_putc(data);
    return 0;
}

int uart0_receive_byte(FILE* UNUSED(stream))
{
    uint8_t data = uart0_getc();
    return data;
}

static FILE uart0_stream = FDEV_SETUP_STREAM(
                            uart0_send_byte,
                            uart0_receive_byte,
                            _FDEV_SETUP_RW);
 
int main (void)
{
    sei();

    stdin = stdout = &uart0_stream;

    /* set pin 7 of PORTB for output*/
    DDRB |= _BV(DDB7);

    // USB Serial 0
    uart0_init(UART_BAUD_SELECT(9600, F_CPU));

    // LCD Serial 3
    uart3_init(UART_BAUD_SELECT(9600, F_CPU));

    /* while (1) { */
    /*   /\* set pin 7 high to turn led on *\/ */
    /*   PORTB |= _BV(PORTB7); */
    /*   _delay_ms(BLINK_DELAY_MS); */

    /*   uart0_putc(65); */

    /*   /\* set pin 5 low to turn led off *\/ */
    /*   PORTB &= ~_BV(PORTB7); */
    /*   _delay_ms(BLINK_DELAY_MS); */
    //}
    _delay_ms(500);
    uart3_putc(0);
    _delay_ms(100);
    uart3_putc(12);
    _delay_ms(100);
    uart3_putc(19);
    _delay_ms(100);
    uart3_puts("Hello, World!\n");

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
