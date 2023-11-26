/*
 * software_timer.h
 *
 *  Created on: Oct 30, 2023
 *      Author: HP
 */

#ifndef INC_SOFTWARE_TIMER_H_
#define INC_SOFTWARE_TIMER_H_

#include "tim.h"

typedef uint8_t bool;

#define FREQUENCY_OF_TIM	1000 //Hz
#define READ_BUTTON_TIME	10 //50ms
#define BLINKING_TIME		1000 // blink in 2Hz
#define INCREASE_TIME		200//200ms
#define READ_SCREEN_TIME	10
#define SNAKE_MOVE_TIME		500

void set_timer(unsigned i, unsigned int time);
void run_timer(void);
bool is_timer_on(unsigned i);
void timer_init();
void timer_EnableDelayUs();
void delay_us (uint16_t us);
#endif /* INC_SOFTWARE_TIMER_H_ */
