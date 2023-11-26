/*
 * display_screen.h
 *
 *  Created on: Nov 26, 2023
 *      Author: huaho
 */

#ifndef INC_DISPLAY_SCREEN_H_
#define INC_DISPLAY_SCREEN_H_

#include "software_timer.h"
#include "lcd.h"
#include "touch.h"
#include "stdlib.h"
#include "control.h"


#define X_SCREEN 	45
#define Y_SCREEN 	40
#define SIZE_SCREEN 150
#define SIZE_FAT	10


void init_screen();
void screen_display();
void touchProcess();
void update_snake(uint8_t isSnakeEating);
uint8_t run_snake();
void random_kudamono();

#endif /* INC_DISPLAY_SCREEN_H_ */
