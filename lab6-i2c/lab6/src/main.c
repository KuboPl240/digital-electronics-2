/***********************************************************************
 * 
 * The I2C (TWI) bus scanner tests all addresses and detects devices
 * that are connected to the SDA and SCL signals.
 * 
 * ATmega328P (Arduino Uno), 16 MHz, PlatformIO
 *
 * Copyright (c) 2023 Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
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
#include <twi.h>            // I2C/TWI library for AVR-GCC
#include <uart.h>           // Peter Fleury's UART library
#include <stdlib.h>         // C library. Needed for number conversions
#include <oled.h> 


/* Global variables --------------------------------------------------*/
#define SENSOR_ADR 0x68
#define SEC 0
#define MINUTE 1
#define HOUR 2

struct RTC_values_structure {
   uint8_t sec;
   uint8_t minute;
   uint8_t temp_int;
   uint8_t hour;
   uint8_t checksum;
} rtc;
/* Function definitions ----------------------------------------------*/
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Call function to test all I2C (TWI) combinations and send
 *           detected devices to UART.
 * Returns:  none
 * 
 * Some known devices:
 *     0x3c - OLED display
 *     0x57 - EEPROM
 *     0x5c - Temp+Humid
 *     0x68 - RTC
 *     0x68 - GY521
 *     0x76 - BME280
 *
 **********************************************************************/
volatile uint8_t new_sensor_data = 0;

int main(void)
{
    char string[2];  // For converting numbers by itoa()

    twi_init();

    // Initialize USART to asynchronous, 8-N-1, 115200 Bd
    uart_init(UART_BAUD_SELECT(115200, F_CPU));
    sei();  // Needed for UART
    TIM1_ovf_1sec();
    TIM1_ovf_enable();
    oled_init(OLED_DISP_ON);
    oled_clrscr(); 
    uart_puts("Scanning I2C... ");
    oled_gotoxy(0,0);
    oled_puts("Scanning I2C... ");
    oled_drawLine(0, 32, 127, 32, WHITE);
    for (uint8_t sla = 8; sla < 120; sla++) {
        if (twi_test_address(sla) == 0) {  // ACK from Slave
            oled_puts("\r\n0x");
            uart_puts("\r\n0x");
            itoa(sla, string, 16);
            oled_puts(string);
            uart_puts(string);
        }
    }
    oled_display(); 

    while (1) {
        if (new_sensor_data == 1) {
            new_sensor_data=0;
            char buffer[10];
            sprintf(buffer, "%02X:%02X:%02X", rtc.hour,rtc.minute,rtc.sec);
            oled_charMode(DOUBLESIZE);
            oled_gotoxy(2,5);
            oled_puts(buffer);
            oled_display(); 
        }
    }

    return 0;
}

ISR(TIMER1_OVF_vect)
{
    twi_start();
    if (twi_write((SENSOR_ADR<<1) | TWI_WRITE) == 0) {
        // Set internal memory location
        twi_write(SEC);
        twi_stop();

        // Read data from internal memory
        twi_start();
        twi_write((SENSOR_ADR<<1) | TWI_READ);
        rtc.sec = twi_read(TWI_ACK);
        rtc.minute = twi_read(TWI_ACK);
        rtc.hour = twi_read(TWI_NACK);
        twi_stop();

        new_sensor_data = 1;
    }
    else {
        twi_stop();
    }

}
