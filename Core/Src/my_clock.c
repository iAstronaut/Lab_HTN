/*
 * my_clock.c
 *
 *  Created on: Nov 23, 2023
 *      Author: HP
 */

#include "my_clock.h"

enum {
	DISPLAY, CHANGE_TIME, ALARM
} st_clock = DISPLAY;
enum {
	release, pressed, long_pressed
} /*state variable of button*/button_st[3];
enum {
	SECOND, MINUTE, HOUR, DAY, DATE, MONTH, YEAR
} st_changing = SECOND;

enum {
	ON, OFF
} st_blinking;
uint8_t buf_sec = 23;
uint8_t buf_min = 11;
uint8_t buf_hour = 20;
uint8_t buf_day = 6;
uint8_t buf_date = 20;
uint8_t buf_mon = 10;
uint8_t buf_year = 23;

uint8_t ala_min = 20;
uint8_t ala_hour = 11;
uint8_t ala_day = 6;
uint8_t ala_date = 24;
uint8_t ala_mon = 11;
uint8_t ala_year = 23;

void update_clock(void);
void display_all_clock(void);
bool button0_fsm(void);
void fsm_changing(void);
void fsm_alarm(void);
bool button1_fsm(uint8_t *number);
bool button2_fsm(void);
void display_all_alarm(void);
void display_mode(void);

/*
 * @brief:	alarm function when value of clock is equal to value of alarm
 * @para:	none
 * @retval:	none
 * */
void alarm(void) {
//	if (ala_year == buf_year && ala_mon == buf_mon && ala_date == buf_date
//			&& ala_day == buf_day && ala_hour == buf_hour
//			&& ala_min == buf_min) {
//
//		lcd_ShowStr(10, 200, "WAKE UP", WHITE, RED, 24, 0);
//		buzzer_SetVolume(50);
//	}
//	else{
//		buzzer_SetVolume(0);
//		lcd_ShowStr(10, 200, "WAKE UP", BLACK, BLACK, 24, 0);
//	}
}
/*
 * @brief:	top-layer state machine of clock
 * @para:	none
 * @retval:	none
 * */
void fsm_clock(void) {
	switch (st_clock) {
	case DISPLAY:
		update_clock();
		display_all_clock();
		display_mode();
		alarm();
		button0_fsm();
		break;
	case CHANGE_TIME:
		fsm_changing();
		display_mode();
		button0_fsm();
		break;
	case ALARM:
		fsm_alarm();
		display_mode();
		button0_fsm();
	}
}
/*
 * @brief:	blinking number, changing buffer of alarm
 * @para:	none
 * @retval:	none
 * */
void fsm_alarm(void) {
	switch (st_changing) {
	case SECOND:
		// no set alarm on second
		dis_sec(1, 0);
		st_changing = MINUTE;
		break;
	case MINUTE:
		//blinking number
		switch (st_blinking) {
		case ON:
			dis_min(ala_min, 1);
			if (!is_timer_on(1)) {
				st_blinking = OFF;
				set_timer(1, BLINKING_TIME);
			}
			break;
		case OFF:
			dis_min(ala_min, 0);
			if (!is_timer_on(1)) {
				st_blinking = ON;
				set_timer(1, BLINKING_TIME);
			}
		}
		//set value
		button1_fsm(&ala_min);
		button2_fsm();
		break;
	case HOUR:
		switch (st_blinking) {
		case ON:
			dis_hour(ala_hour, 1);
			if (!is_timer_on(1)) {
				st_blinking = OFF;
				set_timer(1, BLINKING_TIME);
			}
			break;
		case OFF:
			dis_hour(ala_hour, 0);
			if (!is_timer_on(1)) {
				st_blinking = ON;
				set_timer(1, BLINKING_TIME);
			}
		}
		button1_fsm(&ala_hour);
		button2_fsm();
		break;
	case DAY:
		switch (st_blinking) {
		case ON:
			dis_day(ala_day, 1);
			if (!is_timer_on(1)) {
				st_blinking = OFF;
				set_timer(1, BLINKING_TIME);
			}
			break;
		case OFF:
			dis_day(ala_day, 0);
			if (!is_timer_on(1)) {
				st_blinking = ON;
				set_timer(1, BLINKING_TIME);
			}
		}
		button1_fsm(&ala_day);
		button2_fsm();
		break;
	case DATE:
		switch (st_blinking) {
		case ON:
			dis_date(ala_date, 1);
			if (!is_timer_on(1)) {
				st_blinking = OFF;
				set_timer(1, BLINKING_TIME);
			}
			break;
		case OFF:
			dis_date(ala_date, 0);
			if (!is_timer_on(1)) {
				st_blinking = ON;
				set_timer(1, BLINKING_TIME);
			}
		}
		button1_fsm(&ala_date);
		button2_fsm();
		break;
	case MONTH:
		switch (st_blinking) {
		case ON:
			dis_month(ala_mon, 1);
			if (!is_timer_on(1)) {
				st_blinking = OFF;
				set_timer(1, BLINKING_TIME);
			}
			break;
		case OFF:
			dis_month(ala_mon, 0);
			if (!is_timer_on(1)) {
				st_blinking = ON;
				set_timer(1, BLINKING_TIME);
			}
		}
		button1_fsm(&ala_mon);
		button2_fsm();
		break;
	case YEAR:
		switch (st_blinking) {
		case ON:
			dis_year(ala_year, 1);
			if (!is_timer_on(1)) {
				st_blinking = OFF;
				set_timer(1, BLINKING_TIME);
			}
			break;
		case OFF:
			dis_year(ala_year, 0);
			if (!is_timer_on(1)) {
				st_blinking = ON;
				set_timer(1, BLINKING_TIME);
			}
		}
		button1_fsm(&ala_year);
		button2_fsm();
	}
}
/*
 * @brief:	blinking number, changing buffer of clock
 * @para:	none
 * @retval:	none
 * */
void fsm_changing(void) {
	switch (st_changing) {
	case SECOND:
		//blinking number
		switch (st_blinking) {
		case ON:
			dis_sec(buf_sec, 1);
			if (!is_timer_on(1)) {
				st_blinking = OFF;
				set_timer(1, BLINKING_TIME);
			}
			break;
		case OFF:
			dis_sec(buf_sec, 0);
			if (!is_timer_on(1)) {
				st_blinking = ON;
				set_timer(1, BLINKING_TIME);
			}
		}
		button1_fsm(&buf_sec);
		button2_fsm();
		break;
	case MINUTE:
		switch (st_blinking) {
		case ON:
			dis_min(buf_min, 1);
			if (!is_timer_on(1)) {
				st_blinking = OFF;
				set_timer(1, BLINKING_TIME);
			}
			break;
		case OFF:
			dis_min(buf_min, 0);
			if (!is_timer_on(1)) {
				st_blinking = ON;
				set_timer(1, BLINKING_TIME);
			}
		}
		button1_fsm(&buf_min);
		button2_fsm();
		break;
	case HOUR:
		switch (st_blinking) {
		case ON:
			dis_hour(buf_hour, 1);
			if (!is_timer_on(1)) {
				st_blinking = OFF;
				set_timer(1, BLINKING_TIME);
			}
			break;
		case OFF:
			dis_hour(buf_hour, 0);
			if (!is_timer_on(1)) {
				st_blinking = ON;
				set_timer(1, BLINKING_TIME);
			}
		}
		button1_fsm(&buf_hour);
		button2_fsm();
		break;
	case DAY:
		switch (st_blinking) {
		case ON:
			dis_day(buf_day, 1);
			if (!is_timer_on(1)) {
				st_blinking = OFF;
				set_timer(1, BLINKING_TIME);
			}
			break;
		case OFF:
			dis_day(buf_day, 0);
			if (!is_timer_on(1)) {
				st_blinking = ON;
				set_timer(1, BLINKING_TIME);
			}
		}
		button1_fsm(&buf_day);
		button2_fsm();
		break;
	case DATE:
		switch (st_blinking) {
		case ON:
			dis_date(buf_date, 1);
			if (!is_timer_on(1)) {
				st_blinking = OFF;
				set_timer(1, BLINKING_TIME);
			}
			break;
		case OFF:
			dis_date(buf_date, 0);
			if (!is_timer_on(1)) {
				st_blinking = ON;
				set_timer(1, BLINKING_TIME);
			}
		}
		button1_fsm(&buf_date);
		button2_fsm();
		break;
	case MONTH:
		switch (st_blinking) {
		case ON:
			dis_month(buf_mon, 1);
			if (!is_timer_on(1)) {
				st_blinking = OFF;
				set_timer(1, BLINKING_TIME);
			}
			break;
		case OFF:
			dis_month(buf_mon, 0);
			if (!is_timer_on(1)) {
				st_blinking = ON;
				set_timer(1, BLINKING_TIME);
			}
		}
		button1_fsm(&buf_mon);
		button2_fsm();
		break;
	case YEAR:
		switch (st_blinking) {
		case ON:
			dis_year(buf_year, 1);
			if (!is_timer_on(1)) {
				st_blinking = OFF;
				set_timer(1, BLINKING_TIME);
			}
			break;
		case OFF:
			dis_year(buf_year, 0);
			if (!is_timer_on(1)) {
				st_blinking = ON;
				set_timer(1, BLINKING_TIME);
			}
		}
		button1_fsm(&buf_year);
		button2_fsm();
	}
}
/*
 * @brief:	reset the string when transitioning mode
 * @para:	none
 * @retval:	none
 * */
void reset_str(void){
	lcd_ShowStr(20, 30, "MODE: CHANGE TIME", BLACK, BLACK, 24, 0);
}
/*
 * @brief:	mode button fsm - 2 states
 * @para:	none
 * @retval:	1 - successful
 * 			0 - fail
 * */
bool button0_fsm(void) {
	switch (button_st[0]) {
	case release:
		if (is_button_pressed(0) == 1) {
			// to do
			switch (st_clock) {
			case DISPLAY:
				display_all_clock();
				st_clock = CHANGE_TIME;
				st_changing = SECOND;
				break;
			case CHANGE_TIME:
				display_all_alarm();
				st_changing = SECOND;
				st_clock = ALARM;
				break;
			case ALARM:
				st_clock = DISPLAY;
			}
			reset_str();
			button_st[0] = pressed;
		} else if (is_button_pressed(0) == ERROR)
			return 0;
		break;
	case pressed:
		if (!is_button_pressed(0)) {
			button_st[0] = release;
		} else {
			return 0;
		}
		break;
	default:
		return 0;
	}
	return 1;
}
/*
 * @brief:	set value button fsm - 2 states
 * @para:	none
 * @retval:	1 - successful
 * 			0 - fail
 * */
bool button2_fsm(void) {
	switch (button_st[2]) {
	case release:
		if (is_button_pressed(12) == 1) {
			// to do
			if (st_clock == CHANGE_TIME) {
				switch (st_changing) {
				case SECOND:

					ds3231_Write(ADDRESS_SEC, buf_sec);
					st_changing = MINUTE;
					break;
				case MINUTE:
					ds3231_Write(ADDRESS_MIN, buf_min);
					st_changing = HOUR;
					break;
				case HOUR:
					ds3231_Write(ADDRESS_HOUR, buf_hour);
					st_changing = DAY;
					break;
				case DAY:
					ds3231_Write(ADDRESS_DAY, buf_day);
					st_changing = YEAR;
					break;
				case YEAR:
					ds3231_Write(ADDRESS_YEAR, buf_year);
					st_changing = MONTH;
					break;
				case MONTH:
					ds3231_Write(ADDRESS_MONTH, buf_mon);
					st_changing = DATE;
					break;
				case DATE:
					ds3231_Write(ADDRESS_DATE, buf_date);
					st_changing = SECOND;
					break;
				}
				display_all_clock();
			} else {
				switch (st_changing) {
				case SECOND:
					st_changing = MINUTE;
					break;
				case MINUTE:
					st_changing = HOUR;
					break;
				case HOUR:
					st_changing = DAY;
					break;
				case DAY:
					st_changing = YEAR;
					break;
				case YEAR:
					st_changing = MONTH;
					break;
				case MONTH:
					st_changing = DATE;
					break;
				case DATE:
					st_changing = SECOND;
					break;
				}
			}
			button_st[2] = pressed;
		} else if (is_button_pressed(12) == ERROR)
			return 0;
		break;
	case pressed:
		if (!is_button_pressed(12)) {
			button_st[2] = release;
		} else {
			return 0;
		}
		break;
	default:
		return 0;
	}
	return 1;
}
/*
 * @brief:	increase the value of number based on conditions
 * @para:	number - the value need to be risen
 * @retval:	none
 * */
void increase_value(uint8_t *number) {
	switch (st_changing) {
	case SECOND:
	case MINUTE:
		(*number)++;
		if (*number > 59)
			*number = 0;
		break;
	case HOUR:
		(*number)++;
		if (*number > 23)
			*number = 0;
		break;
	case DAY:
		(*number)++;
		if (*number > 7)
			*number = 1;
		break;
	case YEAR:
		(*number)++;
		if (*number > 99)
			*number = 0;
		break;
	case MONTH:
		(*number)++;
		if (*number > 12)
			*number = 1;
		break;
	case DATE:
		(*number)++;
		uint8_t max_date = 30;
		switch (buf_mon) {
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			max_date = 31;
			break;
		case 2:
			if (buf_year % 4)
				max_date = 28;
			else
				max_date = 29;
			break;
		default:
			break;
		}
		if (*number > max_date)
			*number = 1;

	}
}
/*
 * @brief:	increasing-value button fsm - 3 states
 * @para:	none
 * @retval:	1 - successful
 * 			0 - fail
 * */
bool button1_fsm(uint8_t *number) {
	switch (button_st[1]) {
	case release:
		if (is_button_pressed(3) == 1) {
			// to do
			increase_value(number);
			button_st[1] = pressed;
		} else if (is_button_pressed(3) == ERROR)
			return 0;
		break;
	case pressed:
		if (!is_button_pressed(3)) {
			button_st[1] = release;
		} else if (is_button_long_pressed(3) == 1) {
			button_st[1] = long_pressed;
		} else {
			return 0;
		}
		break;
	case long_pressed:
		// to do
		st_blinking = ON;
		if (!is_timer_on(2)) {
			increase_value(number);
			set_timer(2, INCREASE_TIME);
		}
		if (!is_button_pressed(3)) {
			button_st[1] = release;
		}
		break;
	default:
		return 0;
		break;
	}
	return 1;
}
/*
 * @brief:	update the data of ds3231 for initial
 * @para:	none
 * @retval:	none
 * */
void update_ds3231_register(void) {
	ds3231_Write(ADDRESS_SEC, buf_sec);
	ds3231_Write(ADDRESS_YEAR, buf_year);
	ds3231_Write(ADDRESS_MONTH, buf_mon);
	ds3231_Write(ADDRESS_DATE, buf_date);
	ds3231_Write(ADDRESS_DAY, buf_day);
	ds3231_Write(ADDRESS_HOUR, buf_hour);
	ds3231_Write(ADDRESS_MIN, buf_min);

}
/*
 * @brief:	update the data of clock in display mode
 * @para:	none
 * @retval:	none
 * */
void update_clock(void) {
	buf_sec = ds3231_sec;
	buf_min = ds3231_min;
	buf_hour = ds3231_hours;
	buf_day = ds3231_day;
	buf_date = ds3231_date;
	buf_mon = ds3231_month;
	buf_year = ds3231_year;
}
/*
 * @brief:	display all data of clock
 * @para:	none
 * @retval:	none
 * */
void display_all_clock(void) {
	dis_date(buf_date, 1);
	dis_day(buf_day, 1);
	dis_hour(buf_hour, 1);
	dis_min(buf_min, 1);
	dis_month(buf_mon, 1);
	dis_year(buf_year, 1);
	dis_sec(buf_sec, 1);
}
/*
 * @brief:	display the current mode
 * @para:	none
 * @retval:	none
 * */
void display_mode(void) {
//	switch (st_clock) {
//	case DISPLAY:
//		lcd_ShowStr(20, 30, "MODE: DISPLAY", WHITE, RED, 24, 0);
//		break;
//	case CHANGE_TIME:
//		lcd_ShowStr(20, 30, "MODE: CHANGE TIME", WHITE, RED, 24, 0);
//		break;
//	case ALARM:
//		lcd_ShowStr(20, 30, "MODE: ALARM", WHITE, RED, 24, 0);
//		break;
//	}
}
/*
 * @brief:	display all data of alarm
 * @para:	none
 * @retval:	none
 * */
void display_all_alarm(void) {
//	dis_date(ala_date, 1);
//	dis_day(ala_day, 1);
//	dis_hour(ala_hour, 1);
//	dis_min(ala_min, 1);
//	dis_month(ala_mon, 1);
//	dis_year(ala_year, 1);
}
