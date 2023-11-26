/*
 * control.c
 *
 *  Created on: Nov 25, 2023
 *      Author: huaho
 */

#include "control.h"

#define X_BUTTON 	60
#define Y_BUTTON 	200
#define SIZE_BUTTON 40

enum {
	INIT, START, GAME, OVER
} st_game;

/*
 * 0 is up, 1 is down, 2 is left, 3 is right
 */
struct {
	uint16_t x_start;
	uint16_t y_start;
	uint16_t x_end;
	uint16_t y_end;
	uint8_t is_pressed;
} button_lcd[4];

uint16_t score = 0;

void bt_start();
uint8_t is_bt_start_pressed();
void bt_retry();
uint8_t is_bt_retry_pressed();

void trasition_to_game();
void snake_direct();

void fsm_game() {
	lcd_ShowIntNum(108, 10, score, 3, YELLOW, BLACK, 16);
	switch (st_game) {
	case INIT:
		bt_start();
		st_game = START;
		break;
	case START:
		if (is_bt_start_pressed()) {
			trasition_to_game();
		}
		break;
	case GAME:
		snake_direct();
		if (!is_timer_on(3)) {
			if (run_snake() == 0) {
				bt_retry();
				st_game = OVER;
				break;
			} else if (run_snake() == 1) {
				update_snake(1);
			} else {
				score++;
				update_snake(2);
				random_kudamono();
			}
			screen_display();
			set_timer(3, SNAKE_MOVE_TIME);
		}
		break;
	case OVER:
		if(is_bt_retry_pressed()) {
			score = 0;
			trasition_to_game();
		}
		break;
	}
}

void bt_start() {
	lcd_Fill(X_SCREEN, Y_SCREEN, X_SCREEN + SIZE_SCREEN, Y_SCREEN + SIZE_SCREEN,
			GREEN);
	lcd_ShowStr(X_SCREEN + 20, Y_SCREEN + 20, "START", BLACK, GREEN, 32, 1);
}

uint8_t is_bt_start_pressed() {
	if (!touch_IsTouched())
		return 0;
	return touch_GetX() > X_SCREEN && touch_GetX() < X_SCREEN + SIZE_SCREEN
			&& touch_GetY() > Y_SCREEN && touch_GetY() < Y_SCREEN + SIZE_SCREEN;
}

void bt_retry() {
	lcd_Fill(X_SCREEN, Y_SCREEN, X_SCREEN + SIZE_SCREEN, Y_SCREEN + SIZE_SCREEN,
			GREEN);
	lcd_ShowStr(X_SCREEN + 20, Y_SCREEN + 20, "RETRY", BLACK, GREEN, 32, 1);
}

uint8_t is_bt_retry_pressed(){
	if (!touch_IsTouched())
		return 0;
	return touch_GetX() > X_SCREEN && touch_GetX() < X_SCREEN + SIZE_SCREEN
			&& touch_GetY() > Y_SCREEN && touch_GetY() < Y_SCREEN + SIZE_SCREEN;
}

void trasition_to_game() {
	st_game = GAME;
	init_screen();
	screen_display();
}

void init_control_button() {
	button_lcd[0].x_start = X_BUTTON + SIZE_BUTTON;
	button_lcd[0].y_start = Y_BUTTON;
	button_lcd[0].x_end = button_lcd[0].x_start + SIZE_BUTTON;
	button_lcd[0].y_end = button_lcd[0].y_start + SIZE_BUTTON;
	button_lcd[0].is_pressed = 0;

	button_lcd[1].x_start = button_lcd[0].x_start;
	button_lcd[1].y_start = button_lcd[0].y_end + SIZE_BUTTON;
	button_lcd[1].x_end = button_lcd[1].x_start + SIZE_BUTTON;
	button_lcd[1].y_end = button_lcd[1].y_start + SIZE_BUTTON;
	button_lcd[1].is_pressed = 0;

	button_lcd[2].x_start = X_BUTTON;
	button_lcd[2].y_start = Y_BUTTON + SIZE_BUTTON;
	button_lcd[2].x_end = button_lcd[2].x_start + SIZE_BUTTON;
	button_lcd[2].y_end = button_lcd[2].y_start + SIZE_BUTTON;
	button_lcd[2].is_pressed = 0;

	button_lcd[3].x_start = button_lcd[0].x_end;
	button_lcd[3].y_start = button_lcd[0].y_end;
	button_lcd[3].x_end = button_lcd[3].x_start + SIZE_BUTTON;
	button_lcd[3].y_end = button_lcd[3].y_start + SIZE_BUTTON;
	button_lcd[3].is_pressed = 0;

	lcd_Fill(button_lcd[0].x_start, button_lcd[0].y_start, button_lcd[0].x_end,
			button_lcd[0].y_end, BLUE);
	//lcd_ShowStr(button_lcd[0].x_start + 19, button_lcd[0].y_start + 13, "U", RED, BLUE, 24, 1);

	lcd_Fill(button_lcd[1].x_start, button_lcd[1].y_start, button_lcd[1].x_end,
			button_lcd[1].y_end, BLUE);
	//lcd_ShowStr(button_lcd[1].x_start + 19, button_lcd[1].y_start + 13, "D", RED, BLUE, 24, 1);

	lcd_Fill(button_lcd[2].x_start, button_lcd[2].y_start, button_lcd[2].x_end,
			button_lcd[2].y_end, BLUE);
	//lcd_ShowStr(button_lcd[2].x_start + 19, button_lcd[2].y_start + 13, "L", RED, BLUE, 24, 1);

	lcd_Fill(button_lcd[3].x_start, button_lcd[3].y_start, button_lcd[3].x_end,
			button_lcd[3].y_end, BLUE);
	//lcd_ShowStr(button_lcd[3].x_start + 19, button_lcd[3].y_start + 13, "R", RED, BLUE, 24, 1);
//	lcd_DrawRectangle(button_lcd[0].x_start, button_lcd[0].y_start,
//			button_lcd[0].x_end, button_lcd[0].y_end, BLACK);
//	lcd_DrawRectangle(button_lcd[1].x_start, button_lcd[1].y_start,
//			button_lcd[1].x_end, button_lcd[1].y_end, BLACK);
//	lcd_DrawRectangle(button_lcd[2].x_start, button_lcd[2].y_start,
//			button_lcd[2].x_end, button_lcd[2].y_end, BLACK);
//	lcd_DrawRectangle(button_lcd[3].x_start, button_lcd[3].y_start,
//			button_lcd[3].x_end, button_lcd[3].y_end, BLACK);
}
void button_lcd_init() {
	init_control_button();
	lcd_ShowStr(60, 10, "SCORE:", YELLOW, BLACK, 16, 0);
}

void snake_direct() {
	// 50ms task
	if (!is_timer_on(0)) {
		set_timer(0, READ_SCREEN_TIME);
		touchProcess();
	}
}

uint8_t isButtonUp() {
if(touch_GetX() > button_lcd[0].x_start
		&& touch_GetX() < button_lcd[0].x_end
		&& touch_GetY() > button_lcd[0].y_start
		&& touch_GetY() < button_lcd[0].y_end){
	lcd_ShowStr(X_BUTTON + 10, Y_BUTTON + 20, "U", WHITE, BLACK, 16, 0);
	return 1;
}
	return 0;
}

uint8_t isButtonDown() {
	if (!touch_IsTouched())
		return 0;
	if( touch_GetX() > button_lcd[1].x_start
			&& touch_GetX() < button_lcd[1].x_end
			&& touch_GetY() > button_lcd[1].y_start
			&& touch_GetY() < button_lcd[1].y_end){
		lcd_ShowStr(X_BUTTON + 10, Y_BUTTON + 20, "D", WHITE, BLACK, 16, 0);
		return 1;
	}
	return 0;
}

uint8_t isButtonLeft() {
	if (!touch_IsTouched())
		return 0;
	if( touch_GetX() > button_lcd[2].x_start
			&& touch_GetX() < button_lcd[2].x_end
			&& touch_GetY() > button_lcd[2].y_start
			&& touch_GetY() < button_lcd[2].y_end){
		lcd_ShowStr(X_BUTTON + 10, Y_BUTTON + 20, "L", WHITE, BLACK, 16, 0);
		return 1;
	}
	return 0;
}

uint8_t isButtonRight() {
	if (!touch_IsTouched())
		return 0;
	if( touch_GetX() > button_lcd[3].x_start
			&& touch_GetX() < button_lcd[3].x_end
			&& touch_GetY() > button_lcd[3].y_start
			&& touch_GetY() < button_lcd[3].y_end){
		lcd_ShowStr(X_BUTTON + 10, Y_BUTTON + 20, "R", WHITE, BLACK, 16, 0);
		return 1;
	}
	return 0;
}

