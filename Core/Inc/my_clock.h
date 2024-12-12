/*
 * my_clock.h
 *
 *  Created on: Nov 23, 2023
 *      Author: HP
 */

#ifndef INC_MY_CLOCK_H_
#define INC_MY_CLOCK_H_


#include "my_display.h"
#include "define.h"
#include "button.h"
#include "buzzer.h"
void update_ds3231_register(void);
void fsm_clock(void);
#endif /* INC_MY_CLOCK_H_ */
