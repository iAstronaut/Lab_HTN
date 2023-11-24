/*
 * system.c
 *
 *  Created on: Nov 18, 2023
 *      Author: HP
 */

#include "system.h"
#include "picture.h"
#include "my_clock.h"
extern TIM_HandleTypeDef htim2;

void test_lcd() {
	lcd_Fill(0, 0, 240, 20, BLUE);
	lcd_StrCenter(0, 2, "Hello World !!!", RED, BLUE, 16, 1);
	lcd_ShowStr(20, 30, "Test lcd screen", WHITE, RED, 24, 0);
	lcd_DrawCircle(60, 120, GREEN, 40, 1);
	lcd_DrawCircle(160, 120, BRED, 40, 0);
	lcd_ShowPicture(80, 200, 90, 90, gImage_logo);
}

void displayTime() {
	lcd_ShowIntNum(70, 100, ds3231_hours, 2, GREEN, BLACK, 24);
	lcd_ShowIntNum(110, 100, ds3231_min, 2, GREEN, BLACK, 24);
	lcd_ShowIntNum(150, 100, ds3231_sec, 2, GREEN, BLACK, 24);
	lcd_ShowIntNum(20, 130, ds3231_day, 2, YELLOW, BLACK, 24);
	lcd_ShowIntNum(70, 130, ds3231_date, 2, YELLOW, BLACK, 24);
	lcd_ShowIntNum(110, 130, ds3231_month, 2, YELLOW, BLACK, 24);
	lcd_ShowIntNum(150, 130, ds3231_year, 2, YELLOW, BLACK, 24);
}

void system_init(void) {
	HAL_TIM_Base_Start_IT(&htim2);
	set_timer(0, READ_BUTTON_TIME);
	set_timer(1, BLINKING_TIME);
	button_init();
	lcd_init();
	lcd_Clear(BLACK);
	ds3231_init();
	init_box();
	update_ds3231_register();
	display_text();
}

void system_loop(void) {
	if (!is_timer_on(0)) {
		ds3231_ReadTime();
		button_Scan();
		set_timer(0, READ_BUTTON_TIME);
	}
	fsm_clock();

}
