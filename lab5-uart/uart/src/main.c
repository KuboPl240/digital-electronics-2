/***********************************************************************
 * 
 * Use USART unit and transmit data between ATmega328P and computer.
 * (c) 2018-2024 Tomas Fryza, MIT license
 *
 * Developed using PlatformIO and AVR 8-bit Toolchain 3.6.2.
 * Tested on Arduino Uno board and ATmega328P, 16 MHz.
 * 
 **********************************************************************/

/* Defines -----------------------------------------------------------*/
#ifndef F_CPU
# define F_CPU 16000000  // CPU frequency in Hz required for UART_BAUD_SELECT
#endif


/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include <uart.h>           // Peter Fleury's UART library
#include <stdlib.h>         // C library. Needed for number conversions


/* Function definitions ----------------------------------------------*/
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Use Timer/Counter1 and transmit UART data four times 
 *           per second.
 * Returns:  none
 **********************************************************************/
int main(void)
{
    // Initialize USART to asynchronous, 8N1, 9600
    uart_init(UART_BAUD_SELECT(9600, F_CPU));

    // Configure 16-bit Timer/Counter1 to transmit UART data
    // Set prescaler to 262 ms and enable overflow interrupt
    TIM1_ovf_4ms();
    TIM1_ovf_enable();

    // Interrupts must be enabled, bacause of `uart_puts()`
    sei();

    // Put strings to ringbuffer for transmitting via UART
    uart_puts("\r\n");  // New line only
    uart_puts("Click to Serial monitor\r\n");
    uart_puts("and press a key on keyboard...\r\n");

    // Infinite loop
    while (1) {
    }

    // Will never reach this
    return 0;
}

/* Interrupt service routines ----------------------------------------*/
/**********************************************************************
 * Function: Timer/Counter1 overflow interrupt
 * Purpose:  Transmit UART data four times per second.
 **********************************************************************/
ISR(TIMER1_OVF_vect)
{

    uint8_t value;
    char string[8];  


    value = uart_getc();
    if (value != '\0') {

        uart_putc(value);
        uart_puts("\t0x");
        itoa(value, string, 16);
        uart_puts(string);
        uart_puts("\t");
        itoa(value, string, 10);
        uart_puts(string);
        uart_puts("\t0b");
        itoa(value, string, 2);
        uart_puts(string);
        uart_puts("\r\n");
    }

}


/*
    uart_puts("Chr \tDec \tHx \tBin \t\tEven \r\n");
    uart_puts("---------------------------------------------\r\n");
*/



/*
        // Get the Even parity
        // uint8_t even = 0;
        // Even = b0 xor b1 xor .... xor b7
        // value = b7 b6 b5 b4 b3 b2 b1 b0
        // Cycle 8 times:
        //   -- extract just one bit: LSB
        //   -- apply binary mask and logic AND
        //   -- even = even xor b0
        //   -- shift even to the right
        for (uint8_t i = 0; i < 8; i++) {
            even = even ^ (value & 0x01);
            value = value >> 1;
        }
        uart_puts("\tEven: ");
        itoa(even, string, 10);
        uart_puts(string);
*/