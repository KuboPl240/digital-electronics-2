#include <avr/io.h>
#include "uart.h"
#include <ADC_avr.h>
#include <util/delay.h>
#include <stdbool.h>
#ifndef F_CPU
#define F_CPU 16000000 // CPU frequency in Hz required for UART_BAUD_SELECT
#endif

void PWMInit()
{
  DDRB |= (1 << PB1);
  DDRB |= (1 << PB2);
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

int pos = 90;
int main(void)
{
  initADC(AVCC);

  uart_init(UART_BAUD_SELECT(9600, F_CPU));
  PWMInit();
  sei();
  
  drive(true, 80);
  _delay_ms(100);
  drive(true, 60);
  
  while (1)
  {

    if (!(PINC & (1 << PC1)))
    {
      pos++;
      steering(0);
    }

    if (!(PINC & (1 << PC0)))
    {
      pos--;
       steering(180);
    }

    if ((PINC & (1 << PC0)) && (PINC & (1 << PC1)))
    {
      steering(90);
    }
    

    if (pos < 0)
    {
      pos = 0;
    }


    if (pos > 180)
    {
      pos = 180;
    }
    
    //steering(pos);


   
    uart_putc((PINC & (1 << PC0)) ? 'W' : 'B');
    uart_puts(" ");
    uart_putc((PINC & (1 << PC1)) ? 'W' : 'B');
    uart_puts("\n");
    

    _delay_ms(20);
  }
}
