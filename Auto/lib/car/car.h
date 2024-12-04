#include <inttypes.h>
#include <avr/io.h>
#include <util/delay.h>
#include <ADC_avr.h>
#include <stdbool.h>
#include<stdlib.h>
#include<stdio.h>

void PWMInit(void);

void drive(bool direction, uint8_t speed);

void steering(uint8_t degree);

bool left_sensor(void);

bool right_sensor(void);