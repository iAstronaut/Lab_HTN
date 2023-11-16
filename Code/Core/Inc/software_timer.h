/*
 * software_timer.h
 *
 *  Created on: Nov 16, 2023
 *      Author: huaho
 */

#ifndef INC_SOFTWARE_TIMER_H_
#define INC_SOFTWARE_TIMER_H_

#include<stdint.h>
#include "tim.h"
#define FREQUENCY_OF_TIM	1000 //Hz
#define READ_BUTTON_TIME	10 //10ms
#define SCAN_7SEGLED_TIME	50 //50ms
#define ONE_SECOND			1000//1s
#define TOGGLE_TIME			250
#define INCREASE_TIME		250

typedef uint8_t bool;
void set_timer(unsigned i, unsigned int time);
void run_timer(void);
bool is_timer_on(unsigned i);

#endif /* INC_SOFTWARE_TIMER_H_ */
