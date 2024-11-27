# ADC-AVR
This C library is for the Analog to Digital Converter module of ATmega328 MCU, it allows you to use the ADC module with or without Interrupts.

## Functions :
```c
void initADC(ANALOG_REF ref);
```
This function initiates the ADC module, the function's parameter ref should be one of the followings:
- `AREF` : selecte external voltage reference.
- `AVCC` : the same power supply of MCU.
- `INTERNAL_1_1` : internal 1.1V voltage reference.
```c
uint8_t setAnalogReference(ANALOG_REF ref);
```
This function changes the ADC module voltage reference.

```c
uint16_t analogRead(ANALOG_CHANNEL adcCh);
```
This function returns the ADC value (when conversion is completed) to one of the following channels:
`A0`, `A1`, `A2`, `A3`, `A4`, `A5`, `A6`, `A7`, `INT_TEMP_SENSOR`, `BANG_GAP_VOLATGE` and `ANALOG_GND`.
```c
void start_conversion();
```
We use this function to start conversion when ADC interrupts will be used.
```c
void ADC_attach_interrupt(void* f_ptr);
```
specifies a function to call when conversion is completed, this function's parameter represents the custom function name we wanna call.
```c
void ADC_IRQ_disable();
```
This function is used to disable Interrupts occurring.
## Example:
### 1- Without Interrupt : 
When using this code, make sure to comment out the line: `#define _ADC_IRQ_EN` in `ADC_avr.h` file.
```c 
#include <avr/io.h>
#include <util/delay.h>
#include "ADC/ADC328p.h"
#include "Serial/uart.h"

int main(void)
{
  int adc;
  
  initADC(AVCC);
  SerialBegin(9600);
  while (1)
  {
    adc = analogRead(A0);
    printf("ADC value: %d\r\n", adc);
    _delay_ms(100);
  }
}
```
### 2- With Interrupt : 
When using this code, make sure to uncomment the line: `#define _ADC_IRQ_EN` in `ADC_avr.h` file.
```c
#include <avr/io.h>
#include <util/delay.h>
#include "ADC/ADC328p.h"
#include "Serial/uart.h"

void print_ADC_value();

int main(void)
{
  initADC(AVCC);
  attach_interrupt(print_ADC_value);
  SerialBegin(9600);
  sei();
  while (1)
  {
    start_conversion(A0);
    _delay_ms(100);
  }
}
void print_ADC_value()
{
  printf("ADC value is: %d\r\n", ADC);
}
```
