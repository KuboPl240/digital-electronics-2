#include <avr/io.h>
#include <util/delay.h>
#include <gpio.h>

#define LED_G PB5
#define WAIT 1000

int main(void)
{
    GPIO_mode_output(&DDRB,LED_G);
    GPIO_write_low(&PORTB,LED_G);
    while (1){
        PORTB ^= (1<<LED_G);
        _delay_ms(WAIT);

    }   

    return 0;
}
