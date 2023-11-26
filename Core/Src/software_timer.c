/*
 * software_timer.c
 *
 *  Created on: Oct 30, 2023
 *      Author: HP
 */

#include"software_timer.h"

#define NUMBER_OF_TIMER	5
/*
 * bief: state --> timer is on or off (1: on, 0: off)
 * */
struct {
	bool state;
	unsigned int count;
} timer[NUMBER_OF_TIMER];
/* timer[0]: to read button
 * timer[1]: to blink number
 * timer[2]: to increase number over time
 * timer[3]: to auto move snake
 * */

void timer_init(){
	HAL_TIM_Base_Start_IT(&htim2);
	HAL_TIM_Base_Start(&htim1);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	run_timer();
}
/*
 * @brief:	turn timer on and set value
 * @para:	i: id of timer
 * 			time: unit is ms
 * @retval:	none
 * */
void set_timer(unsigned i, unsigned int time) {
	timer[i].count = time * FREQUENCY_OF_TIM / 1000.0;
	timer[i].state = 1;
}
/*
 * @brief:	run all timers that is on
 * @para:	none
 * @retval:	none
 * */
void run_timer(void) {
	for (unsigned i = 0; i < NUMBER_OF_TIMER; i++) {
		if (timer[i].state) {
			timer[i].count--;
			if (timer[i].count <= 0) {
				timer[i].state = 0;
			}
		}
	}
}
bool is_timer_on(unsigned i) {
	return (timer[i].state == 1);
}


void timer_EnableDelayUs(){
	HAL_TIM_Base_Start(&htim1);
}

void delay_us(uint16_t us)
{
	__HAL_TIM_SET_COUNTER(&htim1,0);  // set the counter value a 0
	while (__HAL_TIM_GET_COUNTER(&htim1) < us);  // wait for the counter to reach the us input in the parameter
}


