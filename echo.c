#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "debounce.h"
#include "joystick.h"

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

int uart3_send_byte(char data, FILE* UNUSED(stream))
{
    uart3_putc(data);
    return 0;
}

int uart3_receive_byte(FILE* UNUSED(stream))
{
    uint8_t data = uart3_getc();
    return data;
}

static FILE uart3_stream = FDEV_SETUP_STREAM(
                            uart3_send_byte,
                            uart3_receive_byte,
                            _FDEV_SETUP_RW);

static FILE* lcdout = &uart3_stream;

//static FILE* lcdin = &uart3_stream;

// Called at about 100Hz (122Hz)
ISR(TIMER0_OVF_vect)
{
    // Debounce buttons. debounce() is declared static inline
    // in debounce.h so we will not suffer from the added overhead
    // of a (external) function call    
    debounce();
}

int main (void)
{
    // Timer0 normal mode, presc 1:256
    TCCR0B = 1<<CS02;
    // Overflow interrupt. (at 16e6/256/256 = 244 Hz)
    TIMSK0 = 1<<TOIE0;

    debounce_init();

    joystick_init();

    sei();

    stdin = stdout = &uart0_stream;

    // Set all pins of PORTL for input
    //DDRL  = 0x00;
    //PORTL = 0xff;

    /* set pin 7 of PORTB for output*/
    //DDRB |= _BV(DDB7);

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
    fprintf(lcdout, "Hello, lcdout!\r");
    fprintf(lcdout, "pi = %.3f\r", 3.142);
    //uart3_puts("Hello, World!\n");

    printf("Hello, stdout!\n");
    while(1)
    {
	//uint16_t v = uart0_getc();
	//if ((v & 0xff00) == 0)
	//{
	//   uint8_t c = v & 0xff; 
        //    uart0_putc(c);
        //    uart3_putc(c);
        //    
	//}
        

        uint8_t buttons = button_down(BUTTON_MASK);

        //uint8_t buttons = PINL;
        uint8_t button0 = buttons & (1 << 0);
        uint8_t button1 = buttons & (1 << 1);
        uint8_t button2 = buttons & (1 << 2);
        uint8_t button3 = buttons & (1 << 3);
        uint8_t button4 = buttons & (1 << 4);
        uint8_t button5 = buttons & (1 << 5);
        uint8_t button6 = buttons & (1 << 6);
        uint8_t button7 = buttons & (1 << 7);

        uint16_t joystick_x = read_adc(0);
        uint16_t joystick_y = read_adc(1);

        fprintf(stdout, "%d %d %d %d %d %d %d %d %d %d\n",
                button0, button1, button2, button3,
		button4, button5, button6, button7,
		joystick_x, joystick_y);
	_delay_ms(BLINK_DELAY_MS);
    } 
    return 0;
}
