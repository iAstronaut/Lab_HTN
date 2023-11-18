/*
 * fsm.c
 *
 *  Created on: Nov 17, 2023
 *      Author: huaho
 */
#include "fsm.h"

enum {
	TRAFFIC_LIGHT,
	RED_ADJUSTMENT,
	YELLOW_ADJUSTMENT,
	GREEN_ADJUSTMENT,
	SET_VALUE,
	INCREASE_BY_1,
	INCREASE_BY_1_OVER_TIME
}/*state variable of system*/light_st = TRAFFIC_LIGHT,
/* previous state variable of system*/light_pre_st = TRAFFIC_LIGHT;

enum {
	RED_GREEN, RED_YELLOW, GREEN_RED, YELLOW_RED, UNEQUAL
}/* state variable of traffic light*/tl_st = RED_GREEN;
enum {
	ON, OFF
} /* state variable of single led*/led_st;
enum {
	release, pressed, long_pressed
} /*state variable of button*/button_st[3];

#define RED_TIME_INIT 		5
#define GREEN_TIME_INIT		3
#define YELLOW_TIME_INIT 	2

unsigned red_time = RED_TIME_INIT;
unsigned green_time = GREEN_TIME_INIT;
unsigned yellow_time = YELLOW_TIME_INIT;

unsigned red_time_buffer = RED_TIME_INIT;
unsigned green_time_buffer = GREEN_TIME_INIT;
unsigned yellow_time_buffer = YELLOW_TIME_INIT;

unsigned traffic_light_timer1 = RED_TIME_INIT;
unsigned traffic_light_timer2 = GREEN_TIME_INIT;


/*@brief:	state machine to blink led in 2Hz
 * @para:	none
 * @retval:	none*/
void fsm_led(void) {
	switch (led_st) {
	case ON:
		switch (light_st) {
		case RED_ADJUSTMENT:
			//turn red led on
			control_traffic_light(0, 1, 0, 0);
			control_traffic_light(1, 1, 0, 0);
			break;
		case YELLOW_ADJUSTMENT:
			//turn yellow led on
			control_traffic_light(0, 0, 1, 0);
			control_traffic_light(1, 0, 1, 0);
			break;
		case GREEN_ADJUSTMENT:
			//turn green led on
			control_traffic_light(0, 0, 0, 1);
			control_traffic_light(1, 0, 0, 1);
			break;
		default:
			break;
		}
		//transition state in 0.25s
		if (!is_timer_on(3)) {
			led_st = OFF;
			set_timer(3, TOGGLE_TIME);
		}
		break;
	case OFF:
		control_traffic_light(0, 0, 0, 0);
		control_traffic_light(1, 0, 0, 0);
		//transition state in 0.25s
		if (!is_timer_on(3)) {
			led_st = ON;
			set_timer(3, TOGGLE_TIME);
		}
	}
}
/**
 * @brief  increase the time value based-on previous state (short-pressed)
 * @param  None
 * @retval None
 */
void increase_value(void) {
	if (light_pre_st == RED_ADJUSTMENT) {
		red_time_buffer++;
		if (red_time_buffer >= 100) {
			red_time_buffer = 0;
		}
	} else if (light_pre_st == YELLOW_ADJUSTMENT) {
		yellow_time_buffer++;
		if (yellow_time_buffer >= 100) {
			yellow_time_buffer = 0;
		}
	} else if (light_pre_st == GREEN_ADJUSTMENT) {
		green_time_buffer++;
		if (green_time_buffer >= 100) {
			green_time_buffer = 0;
		}
	}
}

bool button0_fsm(void) {
	switch (button_st[0]) {
	case release:
		if (is_button_pressed(0) == 1) {
			// to do
			light_pre_st = light_st;
			switch (light_st) {
			case TRAFFIC_LIGHT:
				light_st = RED_ADJUSTMENT;
				break;
			case RED_ADJUSTMENT:
				light_st = YELLOW_ADJUSTMENT;
				break;
			case YELLOW_ADJUSTMENT:
				light_st = GREEN_ADJUSTMENT;
				break;
			case GREEN_ADJUSTMENT:
				light_st = TRAFFIC_LIGHT;
				break;
			default:
				break;
			}
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
 * @brief:	setting-value button fsm - 2 states
 * @para:	none
 * @retval:	1 - successful
 * 			0 - fail
 * */
bool button2_fsm(void) {
	switch (button_st[2]) {
	case release:
		if (is_button_pressed(2) == 1) {
			// to do
			light_pre_st = light_st;
			light_st = SET_VALUE;
			button_st[2] = pressed;
		} else if (is_button_pressed(2) == ERROR)
			return 0;
		break;
	case pressed:
		if (!is_button_pressed(2)) {
			button_st[2] = release;
		} else {
			return 0;
		}
		break;
	default:
		return 0;
		break;
	}
	return 1;
}
/*
 * @brief:	increasing-value button fsm - 3 states
 * @para:	none
 * @retval:	1 - successful
 * 			0 - fail
 * */
bool button1_fsm(void) {
	switch (button_st[1]) {
	case release:
		if (is_button_pressed(1) == 1) {
			// to do
			light_pre_st = light_st;
			light_st = INCREASE_BY_1;
			button_st[1] = pressed;
		} else if (is_button_pressed(1) == ERROR)
			return 0;
		break;
	case pressed:
		if (!is_button_pressed(1)) {
			button_st[1] = release;
		} else if (is_button_long_pressed(1) == 1) {
			button_st[1] = long_pressed;
		} else {
			return 0;
		}
		break;
	case long_pressed:
		// to do
		if (light_st != INCREASE_BY_1_OVER_TIME) {
			light_pre_st = light_st;
			light_st = INCREASE_BY_1_OVER_TIME;
		}
		if (!is_button_pressed(1)) {
			light_st = light_pre_st;
			light_pre_st = INCREASE_BY_1_OVER_TIME;
			button_st[1] = release;
		}
		break;
	default:
		return 0;
		break;
	}
	return 1;
}
void traffic_light_fsm(void) {
	update_led_buf(traffic_light_timer1, traffic_light_timer2, 1);
	switch (tl_st) {
	case RED_GREEN:
		control_traffic_light(0, 1, 0, 0);
		control_traffic_light(1, 0, 0, 1);
		if (traffic_light_timer2 <= 0) {
			traffic_light_timer2 = yellow_time;
			tl_st = RED_YELLOW;
		}
		break;
	case RED_YELLOW:
		control_traffic_light(0, 1, 0, 0);
		control_traffic_light(1, 0, 1, 0);
		if (traffic_light_timer2 <= 0) {
			traffic_light_timer1 = green_time;
			traffic_light_timer2 = red_time;
			tl_st = GREEN_RED;
		}
		break;
	case GREEN_RED:
		control_traffic_light(0, 0, 0, 1);
		control_traffic_light(1, 1, 0, 0);
		if (traffic_light_timer1 <= 0) {
			traffic_light_timer1 = yellow_time;
			tl_st = YELLOW_RED;
		}
		break;
	case YELLOW_RED:
		control_traffic_light(0, 0, 1, 0);
		control_traffic_light(1, 1, 0, 0);
		if (traffic_light_timer1 <= 0) {
			traffic_light_timer1 = red_time;
			traffic_light_timer2 = green_time;
			tl_st = RED_GREEN;
		}
		break;
	default:
		break;
	}
}

void traffic_run(void) {
	switch (light_st) {
	case TRAFFIC_LIGHT:
		if (red_time != green_time + yellow_time) {
			//off all leds
			control_traffic_light(0, 0, 0, 0);
			control_traffic_light(1, 0, 0, 0);
			lcd_ShowStr(40, 130, "SYSTEM", RED, WHITE, 32, 1);
			lcd_ShowStr(40, 162, "ERROR!!!", RED, WHITE, 32, 1);
		} else {
			// decrease timer every 1s
			lcd_ShowStr(40, 130, "SYSTEM", WHITE, WHITE, 32, 1);
			lcd_ShowStr(40, 162, "ERROR!!!", WHITE, WHITE, 32, 1);
			if (!is_timer_on(1)) {
				if (traffic_light_timer1 > 0)
					traffic_light_timer1--;
				if (traffic_light_timer2 > 0)
					traffic_light_timer2--;
				set_timer(1, ONE_SECOND);
			}
			traffic_light_fsm();
		}
		//button0;
		button0_fsm();
		break;

	case RED_ADJUSTMENT:
		// update buffer of red with the condition that previous state has to be different from changing-value states
		if (light_pre_st != INCREASE_BY_1 && light_pre_st != SET_VALUE
				&& light_pre_st != INCREASE_BY_1_OVER_TIME) {
			red_time_buffer = red_time;
		}
		// update buffer of four 7-seg leds: value of red buffer and the mode (2)
		update_led_buf(red_time_buffer, red_time_buffer, 2);
		fsm_led();
		// transition mode function
		button0_fsm();
		button1_fsm();
		button2_fsm();
		break;
	case YELLOW_ADJUSTMENT:
		// update buffer of yellow with the condition that previous state has to be different from changing-value states
		if (light_pre_st != INCREASE_BY_1 && light_pre_st != SET_VALUE
				&& light_pre_st != INCREASE_BY_1_OVER_TIME)
			yellow_time_buffer = yellow_time;
		// update buffer of four 7-seg leds: value of yellow buffer and the mode (3)
		update_led_buf(yellow_time_buffer, yellow_time_buffer, 3);
		fsm_led();
		// transition mode function
		button0_fsm();
		button1_fsm();
		button2_fsm();

		break;
	case GREEN_ADJUSTMENT:
		// update buffer of green with the condition that previous state has to be different from changing-value states
		if (light_pre_st != INCREASE_BY_1 && light_pre_st != SET_VALUE
				&& light_pre_st != INCREASE_BY_1_OVER_TIME)
			green_time_buffer = green_time;
		// update buffer of four 7-seg leds: value of yellow buffer and the mode (4)
		update_led_buf(green_time_buffer, green_time_buffer, 4);
		fsm_led();
		// transition mode function
		button0_fsm();
		button1_fsm();
		button2_fsm();
		break;

	case SET_VALUE:
		// update the time value based-on previous state
		if (light_pre_st == RED_ADJUSTMENT) {
			red_time = red_time_buffer;
		} else if (light_pre_st == YELLOW_ADJUSTMENT) {
			yellow_time = yellow_time_buffer;
		} else if (light_pre_st == GREEN_ADJUSTMENT) {
			green_time = green_time_buffer;
		}

		light_st = light_pre_st;
		light_pre_st = SET_VALUE;
		break;
	case INCREASE_BY_1:
		// increase the time value based-on previous state (short-pressed)
		increase_value();
		light_st = light_pre_st;
		light_pre_st = INCREASE_BY_1;
		break;
	case INCREASE_BY_1_OVER_TIME:
		// increase the time value every 0.25s based-on previous state (short-pressed)
		if (light_pre_st == RED_ADJUSTMENT) {
			update_led_buf(red_time_buffer, red_time_buffer, 2);
		} else if (light_pre_st == YELLOW_ADJUSTMENT) {
			update_led_buf(yellow_time_buffer, yellow_time_buffer, 3);
		} else if (light_pre_st == GREEN_ADJUSTMENT) {
			update_led_buf(green_time_buffer, green_time_buffer, 4);
		}
		if (!is_timer_on(4)) {
			increase_value();
			set_timer(4, INCREASE_TIME);
		}
		button1_fsm();
		break;
	default:
		break;
	}
	LCD_DisplayNum();
}
