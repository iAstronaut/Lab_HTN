/*
 * system.c
 *
 *  Created on: Nov 18, 2023
 *      Author: HP
 */

#include "system.h"

extern TIM_HandleTypeDef htim2;
void system_init(void){
	HAL_TIM_Base_Start_IT(&htim2);
	set_timer(0, READ_BUTTON_TIME);
	button_init();
}

void system_loop(void){
	if(!is_timer_on(0)){
		button_Scan();
		set_timer(0, READ_BUTTON_TIME);
	}
	if(button_count[0] == 1) HAL_GPIO_TogglePin(DEBUG_LED_GPIO_Port, DEBUG_LED_Pin);
}
