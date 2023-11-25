/*
 * system.h
 *
 *  Created on: Nov 18, 2023
 *      Author: HP
 */

#ifndef INC_SYSTEM_H_
#define INC_SYSTEM_H_


#include "software_timer.h"
#include "button.h"
#include "picture.h"
#include "my_clock.h"
#include "my_display.h"
#include "uart.h"
void system_init(void);
void system_loop(void);
#endif /* INC_SYSTEM_H_ */
