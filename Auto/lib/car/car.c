#include "car.h"
#include <stdbool.h>

void PWMInit(void)
{
  sei();
  DDRB |= (1 << PB1); // pin 9
  DDRB |= (1 << PB2); // pin 10
  TCCR1A |= (1 << COM1B1) | (1 << COM1A1) | (1 << WGM11);
  TCCR1B |= (1 << WGM13) | (1 << WGM12) | (1 << CS11);
  ICR1 = 39999;
  OCR1A = 2950;
  _delay_ms(6000);
  OCR1A = 1950;
  _delay_ms(1000);
  OCR1A = 2950;
}

void drive(bool direction, uint8_t speed)
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

bool left_sensor(void){
  return (PINC & (1 << PC1));
}

bool right_sensor(void){
  return (PINC & (1 << PC0));
}