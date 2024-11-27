#include <avr/io.h>
#include "uart.h"
#include <ADC_avr.h>
#include <util/delay.h>
#include <stdbool.h>
#include <oled.h>


#include<stdio.h>
#include<stdlib.h>

#ifndef F_CPU
#define F_CPU 16000000 // CPU frequency in Hz required for UART_BAUD_SELECT
#endif

void PWMInit()
{
  DDRB |= (1 << PB1); // pin 9
  DDRB |= (1 << PB2); // pin 10
  TCCR1A |= (1 << COM1B1) | (1 << COM1A1) | (1 << WGM11);
  TCCR1B |= (1 << WGM13) | (1 << WGM12) | (1 << CS11);
  ICR1 = 39999;
  OCR1A = 2950;
  _delay_ms(6000);
}

void drive(bool direction, char speed)
{
  if (!direction)
    OCR1A = 2950 + speed;
  if (direction)
    OCR1A = 2950 - speed;
}

void steering(uint8_t degree)
{
  OCR1B = 2700 + (1600UL * degree) / 180;
}

int pos = 0;
int main(void)
{
  oled_init(OLED_DISP_ON);
  oled_clrscr(); 
  initADC(AVCC);
  oled_gotoxy(0,0);
  oled_puts("KIT V-1.0 ");
  oled_display(); 
  uart_init(UART_BAUD_SELECT(9600, F_CPU));
  PWMInit();
  sei();

  drive(true, 90);
  _delay_ms(100);
  drive(true, 70);

  while (1)
  {

    if (!(PINC & (1 << PC1)))
    {
      pos = 90;
      drive(true, 50);
      //steering(180);
      oled_gotoxy(0,1);
      oled_puts("--->");
      uart_puts("--->");
      oled_display(); 
    }

    if (!(PINC & (1 << PC0)))
    {
      pos = -90;
      
      // drive(true, 50);
      //steering(0);
      oled_gotoxy(0,1);
      oled_puts("<---"); 
      uart_puts("<---");
      oled_display(); 
    }

    if ((PINC & (1 << PC0)) && (PINC & (1 << PC1)))
    {
      // drive(true, 70);
      pos = pos * 0.9;
      oled_gotoxy(0,1);
      oled_puts(" ^^ "); 
      oled_display(); 
    }

    if ((PINC & (0 << PC0)) && (PINC & (0 << PC1)))
    {
      //drive(true, 0);
    }

    
    steering(90 + pos);
    drive(true, (80UL * abs(pos))/90UL);

    /*uart_putc((PINC & (1 << PC0)) ? 'W' : 'B');
    uart_puts(" ");
    uart_putc((PINC & (1 << PC1)) ? 'W' : 'B');
    uart_puts("\n");*/

    _delay_ms(50);
  }
}
