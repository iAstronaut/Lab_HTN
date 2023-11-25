
#ifndef INC_UART_H_
#define INC_UART_H_

#include "usart.h"
#include <stdio.h>
#include "utils.h"
#include "lcd.h"
#include"define.h"
#include"my_clock.h"
#include"string.h"

#define MAX_SIZE_RING_BUFFER	100

void uart_init_rs232();

void uart_Rs232SendString(uint8_t* str);

void uart_Rs232SendBytes(uint8_t* bytes, uint16_t size);

void uart_Rs232SendNum(uint32_t num);

void uart_Rs232SendNumPercent(uint32_t num);
bool take_number(uint16_t *number);
void invalid_respone(void);
void resquest_year(void);
void resquest_month(void);
void resquest_date(void);
void resquest_day(void);
void resquest_hour(void);
void resquest_minute(void);
void resquest_second(void);
void fsm_handle_uart_flag(void);
#endif /* INC_UART_H_ */

