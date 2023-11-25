/*
 * my_display.c
 *
 *  Created on: Nov 23, 2023
 *      Author: HP
 */

#include "my_display.h"

struct {
	uint32_t text_color;
	uint32_t background_color;
	uint8_t x_second;
	uint8_t y_second;
	uint8_t x_minute;
	uint8_t y_minute;
	uint8_t x_hour;
	uint8_t y_hour;
	uint8_t x_day;
	uint8_t y_day;
	uint8_t x_date;
	uint8_t y_date;
	uint8_t x_month;
	uint8_t y_month;
	uint8_t x_year;
	uint8_t y_year;
} box;
void init_box() {
	box.text_color = GREEN;
	box.background_color = BLACK;
	box.x_second = 150;
	box.y_second = 6;
	box.x_minute = 110;
	box.y_minute = 6;
	box.x_hour = 70;
	box.y_hour = 6;
	box.x_day = 20;
	box.y_day = 60;
	box.x_date = 70;
	box.y_date = 60;
	box.x_month = 110;
	box.y_month = 60;
	box.x_year = 150;
	box.y_year = 60;
}
/**
 * @brief  display second in lcd or not
 * @param	i - yes or no
 * 			number - value is displayed in seconds area
 * @retval none
 */
void dis_sec(uint8_t number, bool i) {
	if (i)
		lcd_ShowIntNum(box.x_second, box.y_second, number, 2, box.text_color,
				box.background_color, 24);
	else
		lcd_ShowIntNum(box.x_second, box.y_second, number, 2,
				box.background_color, box.background_color, 24);
}
/**
 * @brief  display minutes in lcd or not
 * @param	i - yes or no
 * 			number - value is displayed in minutes area
 * @retval none
 */
void dis_min(uint8_t number, bool i) {
	if (i)
		lcd_ShowIntNum(box.x_minute, box.y_minute, number, 2, box.text_color,
				box.background_color, 24);
	else
		lcd_ShowIntNum(box.x_minute, box.y_minute, number, 2,
				box.background_color, box.background_color, 24);

}
/**
 * @brief  display hours in lcd or not
 * @param	i - yes or no
 * 			number - value is displayed in hours area
 * @retval none
 */
void dis_hour(uint8_t number, bool i) {
	if (i)
		lcd_ShowIntNum(box.x_hour, box.y_hour, number, 2, box.text_color,
				box.background_color, 24);
	else
		lcd_ShowIntNum(box.x_hour, box.y_hour, number, 2, box.background_color,
				box.background_color, 24);
}
/**
 * @brief  display day in lcd or not
 * @param	i - yes or no
 * 			number - value is displayed in day area
 * @retval none
 */
void dis_day(uint8_t number, bool i) {
	//lcd_ShowStr(box.x_day, box.y_day - 5, ": Day", box.text_color, box.background_color, 24, 0);
	if (i){
		if(number != 1)
		lcd_ShowIntNum(box.x_day, box.y_day, number, 2, box.text_color,
						box.background_color, 24);
		else
			lcd_ShowStr(box.x_day, box.y_day, "CN", box.text_color,
					box.background_color, 24, 0);
	}
	else
		lcd_ShowIntNum(box.x_day, box.y_day, number, 2, box.background_color,
				box.background_color, 24);
}
/**
 * @brief  display dates in lcd or not
 * @param	i - yes or no
 * 			number - value is displayed in dates area
 * @retval none
 */
void dis_date(uint8_t number, bool i) {
	//lcd_ShowStr(box.x_date, box.y_date - 5, ": Date", box.text_color, box.background_color, 24, 0);
	if (i)
		lcd_ShowIntNum(box.x_date, box.y_date, number, 2, box.text_color,
				box.background_color, 24);
	else
		lcd_ShowIntNum(box.x_date, box.y_date, number, 2, box.background_color,
				box.background_color, 24);
}
/**
 * @brief  display months in lcd or not
 * @param	i - yes or no
 * 			number - value is displayed in months area
 * @retval none
 */
void dis_month(uint8_t number, bool i) {
	//lcd_ShowStr(box.x_month, box.y_month - 5, ": Month", box.text_color, box.background_color, 24, 0);
	if (i)
		lcd_ShowIntNum(box.x_month, box.y_month, number, 2, box.text_color,
				box.background_color, 24);
	else
		lcd_ShowIntNum(box.x_month, box.y_month, number, 2, box.background_color,
				box.background_color, 24);
}
/**
 * @brief  display years in lcd or not
 * @param	i - yes or no
 * 			number - value is displayed in years area
 * @retval none
 */
void dis_year(uint8_t number, bool i) {
	if (i)
		lcd_ShowIntNum(box.x_year, box.y_year, number, 2, box.text_color,
				box.background_color, 24);
	else
		lcd_ShowIntNum(box.x_year, box.y_year, number, 2, box.background_color,
				box.background_color, 24);
}

void display_text(){
	lcd_Clear(BLACK);

	lcd_ShowStr(box.x_day, box.y_day - 24, "Day", box.text_color, box.background_color, 24, 0);
	lcd_ShowStr(box.x_second, box.y_second - 24, "sec", box.text_color, box.background_color, 24, 0);
	lcd_ShowStr(box.x_minute, box.y_minute - 24, "min", box.text_color, box.background_color, 24, 0);
	lcd_ShowStr(box.x_hour - 24, box.y_hour - 24, "hour", box.text_color, box.background_color, 24, 0);

	lcd_ShowStr(box.x_date, box.y_date - 24, "DD", box.text_color, box.background_color, 24, 0);
	lcd_ShowStr(box.x_month, box.y_month - 24, "MM", box.text_color, box.background_color, 24, 0);
	lcd_ShowStr(box.x_year, box.y_year - 24, "YY", box.text_color, box.background_color, 24, 0);
}
