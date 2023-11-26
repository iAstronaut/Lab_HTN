/*
 * system.c
 *
 *  Created on: Nov 24, 2023
 *      Author: huaho
 */

#include "system.h"

int volval = 99;//duty cycle of volume
uint8_t isnotify = 0;//to notify to computer

void LCD_show_sensor();

/*
 * @brief:	update sensor value
 * @para:	none
 * @retval:	none
 * */
void sensor_diplay(){
	  if(!is_timer_on(0)){
		  set_timer(0, READ_BUTTON_TIME);
		  ds3231_ReadTime();
		  button_Scan();
		  sensor_Read();
		  //Check Potentiometer value and send notify
		  if(sensor_GetPotentiometer() >= 4095 * 0.7){
			  if(isnotify == 1){
				  buzzer_SetVolume(volval);
				  if(!is_timer_on(4)){
					  set_timer(4, ONE_SECOND);
					  isnotify = 0;
	  				  uart_Rs232SendString("Potentiometer > 70%\n");
	  				  uart_Rs232SendString("Please reduce Potentiometer\n\n");
				  }
			  }
			  else if(isnotify == 0){
				  buzzer_SetVolume(0);
				  if(!is_timer_on(4)){
					  set_timer(4, ONE_SECOND);
					  isnotify = 1;
				  }
			  }
	  	  }
		  else{
			  buzzer_SetVolume(0);
		  }

		  //change volume value
		  if(button_count[11] == 1){
			  volval += 10;
			  if(volval > 99){
				  volval = 0;
			  }
			  lcd_ShowIntNum(10, 300, volval, 2, WHITE, BLACK, 16);
		  }
		  if(button_count[15] == 1){
			  volval -= 10;
			  if(volval < 0){
				  volval = 99;
			  }
			  lcd_ShowIntNum(10, 300, volval, 2, WHITE, BLACK, 16);
		  }
	  }
	  //show sensor value to LCD
	  if(!is_timer_on(3)){
		  set_timer(3, LCD_SENSOR_TIME);
		  LCD_show_sensor();
	  }
}

/*
 * @brief:	show sensor value to screen
 * @para:	none
 * @retval:	none
 * */
void LCD_show_sensor(){
	lcd_ShowStr(10, 100, "Voltage(V):", WHITE, BLACK, 16, 0);
	lcd_ShowFloatNum(130, 100, sensor_GetVoltage(), 4, WHITE, BLACK, 16);

	lcd_ShowStr(10, 120, "Current(mA):", WHITE, BLACK, 16, 0);
	lcd_ShowFloatNum(130, 120, sensor_GetCurrent(), 4, WHITE, BLACK, 16);

	lcd_ShowStr(10, 140, "Power(mW):", WHITE, BLACK, 16, 0);
	lcd_ShowFloatNum(130, 140, sensor_GetCurrent() * sensor_GetVoltage(), 4, WHITE, BLACK, 16);

	lcd_ShowStr(10, 160, "Light:", WHITE, BLACK, 16, 0);
	if(sensor_GetLight() <= 4095*0.75){
			lcd_ShowStr(60, 160, "(Strong)", WHITE, BLACK, 16, 0);
		}
		else{
			lcd_ShowStr(60, 160, "(Weak)  ", WHITE, BLACK, 16, 0);
		}
	lcd_ShowIntNum(130, 160, sensor_GetLight(), 4, WHITE, BLACK, 16);


	lcd_ShowStr(10, 180, "Poten(Ohm):", WHITE, BLACK, 16, 0);
	lcd_ShowIntNum(130, 180, sensor_GetPotentiometer()*100/4095, 2, WHITE, BLACK, 16);
	lcd_ShowStr(180, 180, "%", WHITE, BLACK, 16, 0);

	lcd_ShowStr (10, 200, "Temp(C):", WHITE, BLACK, 16, 0);
	lcd_ShowFloatNum (130, 200, sensor_GetTemperature(), 4, WHITE, BLACK, 16);
}

void system_loop(void) {
	sensor_diplay();
	fsm_clock();
}

