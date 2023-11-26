/*
 * display_screen.c
 *
 *  Created on: Nov 26, 2023
 *      Author: huaho
 */

#include "display_screen.h"

enum{
	MOVEUP, MOVEDOWN, MOVELEFT, MOVERIGHT
}direct = MOVEUP;

struct{
	uint16_t head_i;
	uint16_t head_j;
	uint16_t tail_i;
	uint16_t tail_j;
	uint16_t color;
}snake;

struct{
	uint16_t i;
	uint16_t j;
	uint16_t color;
}apple;

uint8_t play_screen[SIZE_SCREEN/SIZE_FAT][SIZE_SCREEN/SIZE_FAT];

void Fat_fill(uint8_t i, uint8_t j, uint16_t color);

void random_kudamono(){

	do{
		apple.i = rand()%14;
		apple.j = rand()%14;
	}
	while(play_screen[apple.i][apple.j] != 0);
	play_screen[apple.i][apple.j] = 2;
}

void init_screen(){
for(uint8_t i = 0; i < SIZE_SCREEN/SIZE_FAT; i++){
	for(uint8_t j = 0; j < SIZE_SCREEN/SIZE_FAT; j++){
		play_screen[i][j] = 0;
	}
}
	lcd_Fill(X_SCREEN, Y_SCREEN, X_SCREEN + SIZE_SCREEN, Y_SCREEN + SIZE_SCREEN, WHITE);

	snake.color = BLUE;
	snake.head_i = 8;
	snake.head_j = 7;
	snake.tail_i = 8;
	snake.tail_j = 8;
	play_screen[snake.head_i][snake.head_j] = 1;
	play_screen[snake.tail_i][snake.tail_j] = 1;
	snake.color = BLUE;

	random_kudamono();
	apple.color = RED;

	direct = MOVELEFT;
}

void Fat_fill(uint8_t i, uint8_t j, uint16_t color){
	lcd_Fill(X_SCREEN + i * SIZE_FAT, Y_SCREEN + j * SIZE_FAT, X_SCREEN + i * SIZE_FAT + SIZE_FAT, Y_SCREEN + j * SIZE_FAT + SIZE_FAT, color);
}

void screen_display(){
	for(uint16_t i = 0; i < SIZE_SCREEN/SIZE_FAT;i++){
		for(uint16_t j = 0; j < SIZE_SCREEN/SIZE_FAT;j++){
			if(play_screen[i][j] == 1){
				Fat_fill(i, j, BLUE);
			}
			else if(play_screen[i][j] == 2){
				Fat_fill(i, j, RED);
			}
			else{
				Fat_fill(i, j, WHITE);
			}
		}
	}
}

uint8_t run_snake(){
	switch (direct) {
		case MOVEUP:
			if(snake.head_j == 0 || play_screen[snake.head_i][snake.head_j - 1] == 1){
				return 0;
			}
			else if(play_screen[snake.head_i][snake.head_j - 1] == 2){
				return 2;
			}

			break;
		case MOVEDOWN:
			if(snake.head_j + 1 > SIZE_SCREEN/SIZE_FAT - 1 || play_screen[snake.head_i][snake.head_j + 1] == 1){
				return 0;
			}
			else if(play_screen[snake.head_i][snake.head_j + 1] == 2){
				return 2;
			}

			break;
		case MOVELEFT:
			if(snake.head_i == 0 || play_screen[snake.head_i - 1][snake.head_j] == 1){
				return 0;
			}
			else if(play_screen[snake.head_i - 1][snake.head_j] == 2){
				return 2;
			}

			break;
		case MOVERIGHT:
			if(snake.head_i + 1 > SIZE_SCREEN/SIZE_FAT - 1 || play_screen[snake.head_i + 1][snake.head_j] == 1){
				return 0;
			}
			else if(play_screen[snake.head_i + 1][snake.head_j] == 2){
				return 2;
			}

			break;
		default:
			break;
	}
	return 1;
}

void update_head(){
	switch (direct) {
		case MOVEUP:
			play_screen[snake.head_i][snake.head_j-1] = 1;
			snake.head_j--;
			break;
		case MOVEDOWN:
			play_screen[snake.head_i][snake.head_j+1] = 1;
			snake.head_j++;
			break;
		case MOVELEFT:
			play_screen[snake.head_i-1][snake.head_j] = 1;
			snake.head_i--;
			break;
		case MOVERIGHT:
			play_screen[snake.head_i+1][snake.head_j] = 1;
			snake.head_i++;
			break;
		default:
			break;
	}
}

void update_tail(){
	uint16_t tail_i_next, tail_j_next;
	if(play_screen[snake.tail_i-1][snake.tail_j] == 1){
		tail_i_next = snake.tail_i - 1;
		tail_j_next = snake.tail_j;
	}
	else if(play_screen[snake.tail_i+1][snake.tail_j] == 1){
		tail_i_next = snake.tail_i+1;
		tail_j_next = snake.tail_j;
	}
	else if(play_screen[snake.tail_i][snake.tail_j - 1] == 1){
		tail_i_next = snake.tail_i;
		tail_j_next = snake.tail_j - 1;
	}
	else if(play_screen[snake.tail_i][snake.tail_j + 1] == 1){
		tail_i_next = snake.tail_i;
		tail_j_next = snake.tail_j + 1;
	}
	play_screen[snake.tail_i][snake.tail_j] = 0;
	snake.tail_i = tail_i_next;
	snake.tail_j = tail_j_next;
}
void update_snake(uint8_t isSnakeEating){
	if(isSnakeEating == 1){
		HAL_GPIO_TogglePin(DEBUG_LED_GPIO_Port, DEBUG_LED_Pin);
		update_head();
		update_tail();
	}
	else if(isSnakeEating == 2){
		HAL_GPIO_TogglePin(DEBUG_LED_GPIO_Port, DEBUG_LED_Pin);
		update_head();
	}
}


void touchProcess() {
	switch (direct) {
	case MOVEUP:
		if(isButtonLeft()){
			direct = MOVELEFT;
		}
		else if(isButtonRight()){
			direct = MOVERIGHT;
		}
		break;
	case MOVEDOWN:
		if(isButtonLeft()){
			direct = MOVELEFT;
		}
		else if(isButtonRight()){
			direct = MOVERIGHT;
		}
		break;
	case MOVELEFT:
		if(isButtonUp()){
			direct = MOVEUP;
		}
		else if(isButtonDown()){
			direct = MOVEDOWN;
		}
		break;
	case MOVERIGHT:
		if(isButtonUp()){
			direct = MOVEUP;
		}
		else if(isButtonDown()){
			direct = MOVEDOWN;
		}
		break;
	default:
		break;
	}
}
