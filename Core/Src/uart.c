#include "uart.h"
struct {
	uint8_t buffer[MAX_SIZE_RING_BUFFER];
	uint32_t head;
	uint32_t tail;
	bool full;
	bool flag;
} ring_buffer = { .flag = 0, .head = 0, .tail = 0, .full = 0 };
enum {
	TYPING, DONE
} st_handle_flag;
uint8_t receive_buffer1 = 0;
uint8_t msg[100];

extern enum enum_st_clock st_clock;
extern enum enum_st_changing st_changing_uart;

bool rb_take_data(uint8_t *data);

void fsm_handle_uart_flag(void) {
	if (ring_buffer.flag) {
		switch (st_handle_flag) {
		case TYPING:
			uint8_t last_char;
			if (ring_buffer.tail == 0) {
				last_char = ring_buffer.buffer[MAX_SIZE_RING_BUFFER - 1];
			} else {
				last_char = ring_buffer.buffer[ring_buffer.tail - 1];
			}
			if (last_char == '%') {
				st_handle_flag = DONE;
			}
			break;
		case DONE:
			change_st_uart_respone_to_check();
			st_handle_flag = TYPING;
			break;
		}
		ring_buffer.flag = 0;
	}
}
void invalid_respone(void) {
	char *str = "Invalid input\n";
	uart_Rs232SendString((uint8_t*) str);
}

void resquest_second(void){
	char *str = "second\n";
	uart_Rs232SendString((uint8_t*) str);
}
void resquest_minute(void){
	char *str = "minutes\n";
	uart_Rs232SendString((uint8_t*) str);
}
void resquest_hour(void){
	char *str = "hours\n";
	uart_Rs232SendString((uint8_t*) str);
}
void resquest_day(void){
	char *str = "day\n";
	uart_Rs232SendString((uint8_t*) str);
}
void resquest_date(void){
	char *str = "date\n";
	uart_Rs232SendString((uint8_t*) str);
}
void resquest_month(void){
	char *str = "month\n";
	uart_Rs232SendString((uint8_t*) str);
}
void resquest_year(void){
	char *str = "year\n";
	uart_Rs232SendString((uint8_t*) str);
}
bool take_number(uint16_t *number) {
	if (ring_buffer.buffer[ring_buffer.head] == '%') {
		uint8_t temp;
		rb_take_data(&temp);
		return 0;
	}
	uint8_t data_taken;
	while (ring_buffer.buffer[ring_buffer.head] != '%'
			&& rb_take_data(&data_taken)) {
		*number = *number * 10 + (data_taken - '0');
	}
	if (ring_buffer.buffer[ring_buffer.head] == '%') {
		uint8_t temp;
		rb_take_data(&temp);
	}
	return 1;

}
void rb_add_data(uint8_t data) {
	uint32_t next = ring_buffer.tail + 1;
	if (next >= MAX_SIZE_RING_BUFFER) {
		next = 0;
	}
	if (next == ring_buffer.head) {
		//buffer is full
		ring_buffer.full = 1;
	}
	ring_buffer.buffer[ring_buffer.tail] = data;
	ring_buffer.tail = next;
}
bool rb_take_data(uint8_t *data) {
	if (ring_buffer.head == ring_buffer.tail)
		return 0; // buffer is empty
	uint32_t next = ring_buffer.head + 1;
	if (next >= MAX_SIZE_RING_BUFFER) {
		next = 0;
	}
	*data = ring_buffer.buffer[ring_buffer.head];
	ring_buffer.head = next;
	ring_buffer.full = 0;
	return 1;
}
void uart_init_rs232() {
	HAL_UART_Receive_IT(&huart1, &receive_buffer1, 1);
}

void uart_Rs232SendString(uint8_t *str) {
	HAL_UART_Transmit(&huart1, (void*) msg, sprintf((void*) msg, "%s", str),
			10);
}

void uart_Rs232SendBytes(uint8_t *bytes, uint16_t size) {
	HAL_UART_Transmit(&huart1, bytes, size, 10);
}

void uart_Rs232SendNum(uint32_t num) {
	if (num == 0) {
		char *send = "0";
		uart_Rs232SendString((uint8_t*) send);
		return;
	}
	uint8_t num_flag = 0;
	int i;
	if (num < 0) {
		char *send = "-";
		uart_Rs232SendString((uint8_t*) send);
	}
	for (i = 10; i > 0; i--) {
		if ((num / mypow(10, i - 1)) != 0) {
			num_flag = 1;
			sprintf((void*) msg, "%d", (int) (num / mypow(10, i - 1)));
			uart_Rs232SendString(msg);
		} else {
			if (num_flag != 0) {
				char *send = "0";
				uart_Rs232SendString((uint8_t*) send);
			}

		}
		num %= mypow(10, i - 1);
	}
}

void uart_Rs232SendNumPercent(uint32_t num) {
	sprintf((void*) msg, "%ld", num / 100);
	uart_Rs232SendString(msg);
	char *send = ".";
	uart_Rs232SendString((uint8_t*) send);
	sprintf((void*) msg, "%ld", num % 100);
	uart_Rs232SendString(msg);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == USART1) {
		// rs232 isr
		// can be modified
		if (st_clock == CHANGE_TIME_UART) {
			//HAL_UART_Transmit(&huart1, &receive_buffer1, 1, 10);
			rb_add_data(receive_buffer1);
			ring_buffer.flag = 1;
		}

		// turn on the receice interrupt
		HAL_UART_Receive_IT(&huart1, &receive_buffer1, 1);
	}
}

