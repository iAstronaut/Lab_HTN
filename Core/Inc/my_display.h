/*
 * my_display.h
 *
 *  Created on: Nov 23, 2023
 *      Author: HP
 */

#ifndef INC_MY_DISPLAY_H_
#define INC_MY_DISPLAY_H_

#include "lcd.h"
#include "ds3231.h"
#include "utils.h"
#include "define.h"

void dis_day(uint8_t number, bool i);
void dis_date(uint8_t number, bool i);
void dis_month(uint8_t number, bool i);
void dis_year(uint8_t number, bool i);
void dis_hour(uint8_t number, bool i);
void dis_min(uint8_t number, bool i);
void dis_sec(uint8_t number, bool i);
void init_box();
void display_text();
#endif /* INC_MY_DISPLAY_H_ */
