/*
 * led.c
 *
 *  Created on: Nov 17, 2023
 *      Author: huaho
 */

#include "led.h"

#define NUM_BOX	5

typedef struct {
	uint16_t center_x;
	uint16_t center_y;
	uint16_t COLOR_FILL;
	uint16_t radius;
	uint8_t is_fill;
} circle;

typedef struct {
	uint16_t x;
	uint16_t y;
	uint16_t num;
	uint8_t len;
	uint16_t fc;
	uint16_t bc;
	uint8_t sizey;
} number;

struct {
	uint16_t x;
	uint16_t y;
	uint16_t height;
	uint16_t width;
	uint16_t COLOR_FILL;

	circle red;
	circle yellow;
	circle green;

	//num
	number num;
}traffic_WE;

struct {
	uint16_t x;
	uint16_t y;
	uint16_t height;
	uint16_t width;
	uint16_t COLOR_FILL;

	circle red;
	circle yellow;
	circle green;
	//num
	number num;
}traffic_NS;


struct {
	uint16_t X;
	uint16_t Y;
	uint16_t height;
	uint16_t width;
	uint16_t COLOR_EDGE;
	uint16_t COLOR_FILL;
	uint8_t is_fill;
} BOX[NUM_BOX];

//draw a rec-box
void box_rec(uint8_t box_idx, uint16_t x_start, uint16_t y_start,
		uint16_t width, uint16_t height, uint16_t color_edge,
		uint16_t color_fill, uint8_t is_fill) {
	BOX[box_idx].X = x_start;
	BOX[box_idx].Y = y_start;
	BOX[box_idx].height = height;
	BOX[box_idx].width = width;
	BOX[box_idx].COLOR_EDGE = color_edge;
	BOX[box_idx].COLOR_FILL = color_fill;
	BOX[box_idx].is_fill = is_fill;
	lcd_DrawRectangle(BOX[box_idx].X, BOX[box_idx].Y,
			BOX[box_idx].X + BOX[box_idx].width,
			BOX[box_idx].Y + BOX[box_idx].height, BOX[box_idx].COLOR_EDGE);
	if (BOX[box_idx].is_fill) {
		lcd_Fill(BOX[box_idx].X, BOX[box_idx].Y,
				BOX[box_idx].X + BOX[box_idx].width,
				BOX[box_idx].Y + BOX[box_idx].height, BOX[box_idx].COLOR_FILL);
	}
}


void init_traffic_light(void) {
	//init WE
	traffic_WE.x = 40;
	traffic_WE.y = 60;
	//init traffic light
	traffic_WE.width = 120;
	traffic_WE.height = 40;
	traffic_WE.COLOR_FILL = BLACK;
	lcd_DrawRectangle(traffic_WE.x, traffic_WE.y, traffic_WE.x + traffic_WE.width, traffic_WE.y + traffic_WE.height, traffic_WE.COLOR_FILL);
	lcd_Fill(traffic_WE.x, traffic_WE.y, traffic_WE.x + traffic_WE.width, traffic_WE.y + traffic_WE.height, traffic_WE.COLOR_FILL);

	traffic_WE.red.center_x = traffic_WE.x + traffic_WE.width / 6;
	traffic_WE.yellow.center_x = traffic_WE.x + 3 * traffic_WE.width / 6;
	traffic_WE.green.center_x = traffic_WE.x + 5 * traffic_WE.width / 6;

	traffic_WE.red.center_y = traffic_WE.y + traffic_WE.height / 2;
	traffic_WE.yellow.center_y = traffic_WE.red.center_y;
	traffic_WE.green.center_y = traffic_WE.red.center_y;

	traffic_WE.red.radius = (traffic_WE.height / 2)  * 90 / 100 ;
	traffic_WE.yellow.radius = traffic_WE.red.radius;
	traffic_WE.green.radius = traffic_WE.red.radius;

	traffic_WE.red.is_fill = 1;
	if(traffic_WE.red.is_fill == 1){
		traffic_WE.red.COLOR_FILL = RED;
	}
	else{
		traffic_WE.red.COLOR_FILL = BLACK;
	}
	traffic_WE.yellow.is_fill = 1;
	if(traffic_WE.yellow.is_fill == 1){
		traffic_WE.yellow.COLOR_FILL = YELLOW;
	}
	else{
		traffic_WE.yellow.COLOR_FILL = BLACK;
	}
	traffic_WE.green.is_fill = 1;
	if(traffic_WE.green.is_fill == 1){
		traffic_WE.green.COLOR_FILL = GREEN;
	}
	else{
		traffic_WE.green.COLOR_FILL = BLACK;
	}
	//draw red
	lcd_DrawCircle(traffic_WE.red.center_x, traffic_WE.red.center_y, traffic_WE.red.COLOR_FILL, traffic_WE.red.radius, traffic_WE.red.is_fill);
	//draw yellow
	lcd_DrawCircle(traffic_WE.yellow.center_x, traffic_WE.yellow.center_y, traffic_WE.yellow.COLOR_FILL, traffic_WE.yellow.radius, traffic_WE.yellow.is_fill);
	//draw green
	lcd_DrawCircle(traffic_WE.green.center_x, traffic_WE.green.center_y, traffic_WE.green.COLOR_FILL, traffic_WE.green.radius, traffic_WE.green.is_fill);

	//init num
	traffic_WE.num.len = 2;
	traffic_WE.num.fc = WHITE;
	traffic_WE.num.bc = BLACK;
	traffic_WE.num.sizey = 32;
	traffic_WE.num.x = traffic_WE.x - traffic_WE.num.len * traffic_WE.num.sizey / 2;
	traffic_WE.num.y = traffic_WE.y;
	traffic_WE.num.num = 10;
	lcd_ShowIntNum(traffic_WE.num.x, traffic_WE.num.y, traffic_WE.num.num, traffic_WE.num.len, traffic_WE.num.fc, traffic_WE.num.bc, traffic_WE.num.sizey);

	//init NS
	traffic_NS.x = 180;
	traffic_NS.y = 120;
	//init traffic light
	traffic_NS.width = traffic_WE.height;
	traffic_NS.height = traffic_WE.width;
	traffic_NS.COLOR_FILL = BLACK;
	lcd_DrawRectangle(traffic_NS.x, traffic_NS.y, traffic_NS.x + traffic_NS.width, traffic_NS.y + traffic_NS.height, traffic_NS.COLOR_FILL);
	lcd_Fill(traffic_NS.x, traffic_NS.y, traffic_NS.x + traffic_NS.width, traffic_NS.y + traffic_NS.height, traffic_NS.COLOR_FILL);

	traffic_NS.red.center_y = traffic_NS.y + traffic_NS.height / 6;
	traffic_NS.yellow.center_y = traffic_NS.y + 3 * traffic_NS.height / 6;
	traffic_NS.green.center_y = traffic_NS.y + 5 * traffic_NS.height / 6;

	traffic_NS.red.center_x = traffic_NS.x + traffic_NS.width / 2;
	traffic_NS.yellow.center_x = traffic_NS.red.center_x;
	traffic_NS.green.center_x = traffic_NS.red.center_x;

	traffic_NS.red.radius = (traffic_NS.width / 2)  * 90 / 100 ;
	traffic_NS.yellow.radius = traffic_NS.red.radius;
	traffic_NS.green.radius = traffic_NS.red.radius;

	traffic_NS.red.is_fill = 1;
	if(traffic_NS.red.is_fill == 1){
		traffic_NS.red.COLOR_FILL = RED;
	}
	else{
		traffic_NS.red.COLOR_FILL = BLACK;
	}
	traffic_NS.yellow.is_fill = 1;
	if(traffic_NS.yellow.is_fill == 1){
		traffic_NS.yellow.COLOR_FILL = YELLOW;
	}
	else{
		traffic_NS.yellow.COLOR_FILL = BLACK;
	}
	traffic_NS.green.is_fill = 1;
	if(traffic_NS.green.is_fill == 1){
		traffic_NS.green.COLOR_FILL = GREEN;
	}
	else{
		traffic_NS.green.COLOR_FILL = BLACK;
	}
	//draw red
	lcd_DrawCircle(traffic_NS.red.center_x, traffic_NS.red.center_y, traffic_NS.red.COLOR_FILL, traffic_NS.red.radius, traffic_NS.red.is_fill);
	//draw yellow
	lcd_DrawCircle(traffic_NS.yellow.center_x, traffic_NS.yellow.center_y, traffic_NS.yellow.COLOR_FILL, traffic_NS.yellow.radius, traffic_NS.yellow.is_fill);
	//draw green
	lcd_DrawCircle(traffic_NS.green.center_x, traffic_NS.green.center_y, traffic_NS.green.COLOR_FILL, traffic_NS.green.radius, traffic_NS.green.is_fill);

	//init num
	traffic_NS.num.len = 2;
	traffic_NS.num.fc = WHITE;
	traffic_NS.num.bc = BLACK;
	traffic_NS.num.sizey = 32;
	traffic_NS.num.x = traffic_NS.x;
	traffic_NS.num.y = traffic_NS.y - traffic_NS.num.sizey;
	traffic_NS.num.num = 99;
	lcd_ShowIntNum(traffic_NS.num.x, traffic_NS.num.y, traffic_NS.num.num, traffic_NS.num.len, traffic_NS.num.fc, traffic_NS.num.bc, traffic_NS.num.sizey);
}

void control_traffic_light(uint8_t i, uint8_t red, uint8_t yellow, uint8_t green){
	if(i == 0){
		//draw red
		if(!red){
			traffic_WE.red.COLOR_FILL = BLACK;
		}
		else{
			traffic_WE.red.COLOR_FILL = RED;
		}
		if(!yellow){
			traffic_WE.yellow.COLOR_FILL = BLACK;
		}
		else{
			traffic_WE.yellow.COLOR_FILL = YELLOW;
		}
		if(!green){
			traffic_WE.green.COLOR_FILL = BLACK;
		}
		else{
			traffic_WE.green.COLOR_FILL = GREEN;
		}
		lcd_DrawCircle(traffic_WE.red.center_x, traffic_WE.red.center_y, traffic_WE.red.COLOR_FILL, traffic_WE.red.radius, 1);
		//draw yellow
		lcd_DrawCircle(traffic_WE.yellow.center_x, traffic_WE.yellow.center_y, traffic_WE.yellow.COLOR_FILL, traffic_WE.yellow.radius, 1);
		//draw green
		lcd_DrawCircle(traffic_WE.green.center_x, traffic_WE.green.center_y, traffic_WE.green.COLOR_FILL, traffic_WE.green.radius, 1);
	}
	else{
		if(!red){
			traffic_NS.red.COLOR_FILL = BLACK;
		}
		else{
			traffic_NS.red.COLOR_FILL = RED;
		}
		if(!yellow){
			traffic_NS.yellow.COLOR_FILL = BLACK;
		}
		else{
			traffic_NS.yellow.COLOR_FILL = YELLOW;
		}
		if(!green){
			traffic_NS.green.COLOR_FILL = BLACK;
		}
		else{
			traffic_NS.green.COLOR_FILL = GREEN;
		}
		//draw red
		lcd_DrawCircle(traffic_NS.red.center_x, traffic_NS.red.center_y, traffic_NS.red.COLOR_FILL, traffic_NS.red.radius, 1);
		//draw yellow
		lcd_DrawCircle(traffic_NS.yellow.center_x, traffic_NS.yellow.center_y, traffic_NS.yellow.COLOR_FILL, traffic_NS.yellow.radius, 1);
		//draw green
		lcd_DrawCircle(traffic_NS.green.center_x, traffic_NS.green.center_y, traffic_NS.green.COLOR_FILL, traffic_NS.green.radius, 1);
	}
}

void update_led_buf(unsigned val1, unsigned val2, unsigned mode){
	if(mode == 4 || mode == 3 || mode == 2){
		lcd_ShowIntNum(140, 280, mode, 1, RED, GRAY, 32);
	}
	else{
		lcd_ShowIntNum(140, 280, mode, 1, RED, GRAY, 32);
	}
	traffic_WE.num.num = val1;
	traffic_NS.num.num = val2;
}

void LCD_DisplayNum(){
	lcd_ShowStr(60, 280, "MODE:", RED, GRAY, 32, 0);
	lcd_ShowIntNum(traffic_WE.num.x, traffic_WE.num.y, traffic_WE.num.num, traffic_WE.num.len, traffic_WE.num.fc, traffic_WE.num.bc, traffic_WE.num.sizey);
	lcd_ShowIntNum(traffic_NS.num.x, traffic_NS.num.y, traffic_NS.num.num, traffic_NS.num.len, traffic_NS.num.fc, traffic_NS.num.bc, traffic_NS.num.sizey);
}
