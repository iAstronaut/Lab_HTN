#ifndef INC_CONTROL_H_
#define INC_CONTROL_H_

#include "gpio.h"
#include "uart.h"
#include "button.h"
#include "lcd.h"
#include <stdlib.h>
extern uint8_t light_status;

void lightProcess();

void test_Esp();
void ftoa(float n);
void send_data(void);
#endif /* INC_CONTROL_H_ */
