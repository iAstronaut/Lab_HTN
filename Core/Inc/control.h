/*
 * light_control.h
 *
 *  Created on: Nov 15, 2023
 *      Author: phamv
 */

#ifndef INC_CONTROL_H_
#define INC_CONTROL_H_

#include "gpio.h"
#include "uart.h"
#include "button.h"
#include "lcd.h"

extern uint8_t light_status;

void lightProcess();

void test_Esp();

#endif /* INC_CONTROL_H_ */
