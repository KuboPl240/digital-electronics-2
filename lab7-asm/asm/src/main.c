/*
 * Implementation of LFSR-based (Linear Feedback Shift Register) 
 * pseudo-random generator in AVR assembly.
 * (c) 2017-2024 Tomas Fryza, MIT license
 *
 * Developed using PlatformIO and AVR 8-bit Toolchain 3.6.2.
 * Tested on Arduino Uno board and ATmega328P, 16 MHz.
 * 
 * NOTE:
 *   To see assembly listing, run the following command in Terminal
 *   after the compilation.
 * 
 *   Windows:
 *   C:\Users\YOUR-LOGIN\.platformio\packages\toolchain-atmelavr\bin\avr-objdump -S -d -m avr .pio/build/uno/firmware.elf > firmware.lst
 * 
 *   Linux, Mac:
 *   ~/.platformio/packages/toolchain-atmelavr/bin/avr-objdump -S -d -m avr .pio/build/uno/firmware.elf > firmware.lst
 * 
 * SEE ALSO:
 *   https://five-embeddev.com/baremetal/platformio/
 */

// -- Includes -------------------------------------------------------
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include <uart.h>           // Peter Fleury's UART library
#include <lfsr.h>           // Assembly implementation of LFSR
#include <stdlib.h>         // C library. Needed for number conversions


// -- Function definitions -------------------------------------------
/*
 * Function: Main function where the program execution begins
 * Purpose:  Generate a new pseudo-random value using 4- and/or 8-bit
 *           LFSR structure and send it to UART.
 * Returns:  none
 */
int main(void)
{
    // Initialize USART to asynchronous, 8-N-1, 115200
    // NOTE: Add `monitor_speed = 115200` to `platformio.ini`
    uart_init(UART_BAUD_SELECT(115200, F_CPU));

    TIM1_ovf_33ms();
    TIM1_ovf_enable();

    sei();

    // Infinite empty loop
    while (1)
    {
    }

    // Will never reach this
    return 0;
}


// -- Interrupt service routines -------------------------------------
/*
 * Function: Timer/Counter1 overflow interrupt
 * Purpose:  Generate one pseudo-random value using 4- and/or 8-bit
 *           LFSR structure.
 *
 * Note: (4,3): ...
 *       (4,2): ...
 *       (4,1): ...
 */
ISR(TIMER1_OVF_vect)
{
    static uint8_t value = 0;
    static uint8_t n_values = 0;
    char string[3];  // String for converting numbers by itoa()

    // Multiply-and-accumulate in Assembly
    //                                 c + (a*b)
    //value = multiply_accumulate_asm(value, 3, 7);

    // Send value to UART
    itoa(value, string, 10);
    uart_puts(string);
    uart_puts(", ");

    // WRITE YOUR CODE HERE
    value = lfsr4_fibonacci_asm(value);
    n_values++;
    if(value == 0)
    {
      itoa(n_values, string, 10);
      uart_puts("\r\nLength: ");
      uart_puts(string);
      uart_puts("\r\n"); 
      n_values = 0;
    }
}