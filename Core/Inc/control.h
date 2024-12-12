/*
 * control.h
 *
 *  Created on: Nov 25, 2023
 *      Author: huaho
 */

#ifndef INC_CONTROL_H_
#define INC_CONTROL_H_

#include "software_timer.h"
#include "lcd.h"
#include "touch.h"
#include "display_screen.h"

void button_lcd_init();
void fsm_game();
uint8_t isMoveUp();
uint8_t isMoveDown();
uint8_t isMoveLeft();
uint8_t isMoveRight();

#endif /* INC_CONTROL_H_ */
