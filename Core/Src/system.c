/*
 * system.c
 *
 *  Created on: Nov 18, 2023
 *      Author: HP
 */

#include "system.h"

extern TIM_HandleTypeDef htim2;
uint16_t idx = 0;
float sum_15s = 0;
void system_init(void) {
	HAL_TIM_Base_Start_IT(&htim2);
	set_timer(0, READ_BUTTON_TIME);
	set_timer(1, READ_SENSOR_TIME);
	set_timer(2, SEND_DATA_TIME);
	button_init();
	lcd_init();
	sensor_init();
	uart_init_esp();
	lcd_Clear(BLACK);
	lcd_ShowStr(0, 175, "TEMPERATURE", WHITE, BLACK, 24, 0);
}

void system_loop(void) {
	if (!is_timer_on(0)) {
		button_Scan();
		test_Esp();
		lightProcess();
		set_timer(0, READ_BUTTON_TIME);
		sensor_Read();
	}
	if (!is_timer_on(1)) {
		sensor_CalTemperature(idx++);
		if (idx >= BUFFER_SIZE) {
			float value = sensor_result();
			sum_15s += value;
			lcd_ShowFloatNum(0, 200, value, 4, WHITE, BLACK, 24);
			idx = 0;
		}
		set_timer(1, READ_SENSOR_TIME);
	}
	if (!is_timer_on(2)) {
		ftoa((float)sum_15s/15);
		sum_15s = 0;
		send_data();
		set_timer(2, SEND_DATA_TIME);
	}
}
