#include "uart.h"
#include "car.h"
#include <oled.h>

#ifndef F_CPU
#define F_CPU 16000000 // CPU frequency in Hz required for UART_BAUD_SELECT
#endif


int pos = 0;
int main(void)
{
  oled_init(OLED_DISP_ON);
  oled_clrscr(); 
  oled_gotoxy(0,0);
  oled_puts("KITT V-1.0 ");
  oled_display(); 
  uart_init(UART_BAUD_SELECT(9600, F_CPU));
  PWMInit();

  drive(true, 90);
  _delay_ms(100);
  drive(true, 70);

  while (1)
  {

    if (!left_sensor)
    {
      pos = 90;
      drive(true, 50);
      //steering(180);
      oled_gotoxy(0,1);
      oled_puts("--->");
      uart_puts("--->");
      oled_display(); 
    }

    if (!right_sensor)
    {
      pos = -90;
      
      // drive(true, 50);
      //steering(0);
      oled_gotoxy(0,1);
      oled_puts("<---"); 
      uart_puts("<---");
      oled_display(); 
    }

    if (right_sensor && left_sensor)
    {
      // drive(true, 70);
      pos = pos * 0.9;
      oled_gotoxy(0,1);
      oled_puts(" ^^ "); 
      oled_display(); 
    }

    if (!right_sensor && !left_sensor)
    {
      drive(true, 0);
    }

    
    steering(90 + pos);
    drive(true, (80UL * abs(pos))/90UL);

    uart_putc((PINC & (1 << PC0)) ? 'W' : 'B');
    uart_puts(" ");
    uart_putc((PINC & (1 << PC1)) ? 'W' : 'B');
    uart_puts("\n");

    _delay_ms(50);
  }
}
