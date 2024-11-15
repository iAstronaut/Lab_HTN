/*
 * led.h
 *
 *  Created on: Nov 17, 2023
 *      Author: huaho
 */

#ifndef INC_LED_H_
#define INC_LED_H_

#include <stdint.h>
#include "lcd.h"


void init_traffic_light(void);
void control_traffic_light(uint8_t i, uint8_t red, uint8_t yellow, uint8_t green);
void box_rec(uint8_t box_idx, uint16_t x_start, uint16_t y_start, uint16_t height0, uint16_t width0, uint16_t color_edge, uint16_t color_fill, uint8_t is_fill);
void update_led_buf(unsigned val1, unsigned val2, unsigned mode);
void LCD_DisplayNum();

#endif /* INC_LED_H_ */
