/*
 * light_control.c
 *
 *  Created on: Nov 15, 2023
 *      Author: phamv
 */
#include <control.h>

uint8_t light_status = 0;

void lightProcess() {
	if (button_count[13] == 1) {
		light_status = 1 - light_status;
		if (light_status == 1) {
			uart_EspSendBytes("A", 1);
		} else {
			uart_EspSendBytes("a", 1);
		}
	}
	if (light_status == 1) {
		HAL_GPIO_WritePin(OUTPUT_Y0_GPIO_Port, OUTPUT_Y0_Pin, 1);
	} else {
		HAL_GPIO_WritePin(OUTPUT_Y0_GPIO_Port, OUTPUT_Y0_Pin, 0);
	}
}

void test_Esp() {
	if (uart_EspCheck() == 0) {
		uart_EspSendBytes("o", 1);
		lcd_ShowStr(10, 50, "ESP Connect", BLACK, BLACK, 24, 0);
	} else
		lcd_ShowStr(10, 50, "ESP Connect", GREEN, BLACK, 24, 0);
}
