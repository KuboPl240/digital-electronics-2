/***********************************************************************
 * 
 * Stopwatch by Timer/Counter2 on the Liquid Crystal Display (LCD).
 *
 * ATmega328P (Arduino Uno), 16 MHz, PlatformIO
 *
 * Copyright (c) 2017 Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 * Components list:
 *   16x2 character LCD with parallel interface
 *     VSS  - GND (Power supply ground)
 *     VDD  - +5V (Positive power supply)
 *     Vo   - (Contrast)
 *     RS   - PB0 (Register Select: High for Data transfer, Low for Instruction transfer)
 *     RW   - GND (Read/Write signal: High for Read mode, Low for Write mode)
 *     E    - PB1 (Read/Write Enable: High for Read, falling edge writes data to LCD)
 *     D3:0 - NC (Data bits 3..0, Not Connected)
 *     D4   - PD4 (Data bit 4)
 *     D5   - PD5 (Data bit 5)
 *     D6   - PD6 (Data bit 6)
 *     D7   - PD7 (Data bit 7)
 *     A+K  - Back-light enabled/disabled by PB2
 * 
 **********************************************************************/


/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include <gpio.h>           // GPIO library for AVR-GCC
#include <string.h>
#include "timer.h"          // Timer library for AVR-GCC
#include <lcd.h>            // Peter Fleury's LCD library
#include <stdlib.h>         // C library. Needed for number conversions


/* Global variables --------------------------------------------------*/
// Flag for printing new stopwatch data to LCD
volatile uint8_t update_lcd_stopwatch = 0;

// Stopwatch values
// Declaration of "stopwatch" variable with structure "Stopwatch_structure"
struct Stopwatch_structure {
    uint8_t tenths;  // Tenths of a second
    uint8_t secs;    // Seconds
    uint8_t mins;    // Minutes
    uint8_t bar; 
} stopwatch;


/* Function definitions ----------------------------------------------*/
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Update stopwatch value on LCD screen when 8-bit 
 *           Timer/Counter2 overflows.
 * Returns:  none
 **********************************************************************/
int main(void)
{
    char string[10];  // String for converted numbers by itoa()	
    // Initialize display
    lcd_init(LCD_DISP_ON);
    lcd_clrscr();

    //uint8_t new_char2[8] = {0x0,0x0,0x0,0xa,0x0,0x11,0xe,0x0};
    uint8_t bar1[8] = {0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10};
    uint8_t bar2[8] = {0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18};
    uint8_t bar3[8] = {0x1c,0x1c,0x1c,0x1c,0x1c,0x1c,0x1c,0x1c};
    uint8_t bar4[8] = {0x1e,0x1e,0x1e,0x1e,0x1e,0x1e,0x1e,0x1e};
    uint8_t bar5[8] = {0x1f,0x1f,0x1f,0x1f,0x1f,0x1f,0x1f,0x1f};
    lcd_custom_char(1,bar1);
    lcd_custom_char(2,bar2);
    lcd_custom_char(3,bar3);
    lcd_custom_char(4,bar4);
    lcd_custom_char(5,bar5);
    //lcd_putc(7);
    
    TIM1_ovf_4ms()
    TIM1_ovf_enable();
    sei();

    // Infinite loop
    while (1) {
        if (update_lcd_stopwatch == 1) { // Convert decimal value to string
            
            
            sprintf(string, "%02d:%02d.%d", stopwatch.mins,stopwatch.secs,stopwatch.tenths);
            lcd_gotoxy(0, 0);
            lcd_puts(string);
            lcd_gotoxy(0, 1);
            if(stopwatch.bar>5){
                lcd_gotoxy(1, 1);
                lcd_putc(stopwatch.bar-5);
            }
            else if(stopwatch.bar<=5 && stopwatch.bar>0){
                lcd_gotoxy(0, 1);
                lcd_putc(stopwatch.bar);
            }
            else lcd_puts("    ");

            update_lcd_stopwatch = 0;
        }
    }

    // Will never reach this
    return 0;
}


/* Interrupt service routines ----------------------------------------*/
/**********************************************************************
 * Function: Timer/Counter2 overflow interrupt
 * Purpose:  Update the stopwatch on LCD screen every sixth overflow,
 *           ie approximately every 100 ms (6 x 16 ms = 100 ms).
 **********************************************************************/
ISR(TIMER1_OVF_vect)
{
    static uint8_t n_ovfs = 0;

    n_ovfs++;
    if (n_ovfs >= 24) {
        // Do this every 6 x 16 ms = 100 ms
        n_ovfs = 0;

        //Count tenth of seconds 0, 1, ..., 9, 0, 1, ...
        stopwatch.tenths++;
        if(stopwatch.tenths>9){
           stopwatch.secs++;
           stopwatch.tenths=0;  
        }
        if(stopwatch.secs>59){
           stopwatch.mins++;
           stopwatch.secs=0;  
        }
        
       stopwatch.bar++;
       if(stopwatch.bar>10)stopwatch.bar=0;


        update_lcd_stopwatch = 1;
    }
    // Else do nothing and exit the ISR
}
