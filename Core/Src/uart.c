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
volatile uint8_t receive_buffer1 = 0;
uint8_t msg[100];
volatile bool ring_buffer_flag = 0;

extern enum enum_st_clock st_clock;
extern enum enum_st_changing st_changing_uart;

bool rb_take_data(uint8_t *data);

void set_state(enum UARTState new_state) {
    st_handle_flag = new_state;
}

void fsm_handle_uart_flag(void) {
    if (ring_buffer.flag) {
        switch (st_handle_flag) {
        case TYPING:
            if (ring_buffer.tail == 0) {
                last_char = ring_buffer.buffer[MAX_SIZE_RING_BUFFER - 1];
            } else {
                last_char = ring_buffer.buffer[ring_buffer.tail - 1];
            }
            if (last_char == '%') {
                set_state(DONE);
            }
            break;
        case DONE:
            change_st_uart_respone_to_check();
            set_state(TYPING);
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
    uint8_t data_taken;

    while (rb_take_data(&data_taken) && data_taken != '%') {
        if (data_taken < '0' || data_taken > '9') {
            invalid_respone();
            return false; // Giá trị không hợp lệ
        }
        *number = *number * 10 + (data_taken - '0');
    }

    if (data_taken == '%') {
        return true; // Số hợp lệ
    }

    return false; // Không có ký tự kết thúc hợp lệ
}

void rb_add_data(uint8_t data) {
    if (ring_buffer.full) {
        // Buffer đầy, không thể thêm dữ liệu
        return;
    }

    uint32_t next = (ring_buffer.tail + 1) % MAX_SIZE_RING_BUFFER;

    ring_buffer.buffer[ring_buffer.tail] = data;
    ring_buffer.tail = next;

    if (next == ring_buffer.head) {
        ring_buffer.full = true;
    }
}

bool rb_take_data(uint8_t *data) {
    if (ring_buffer.head == ring_buffer.tail && !ring_buffer.full) {
        return false; // Buffer rỗng
    }

    *data = ring_buffer.buffer[ring_buffer.head];
    ring_buffer.head = (ring_buffer.head + 1) % MAX_SIZE_RING_BUFFER;

    ring_buffer.full = false;
    return true;
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
        if (st_clock == CHANGE_TIME_UART) {
            rb_add_data(receive_buffer1);
            ring_buffer.flag = 1;
        }
        HAL_UART_Receive_IT(&huart1, &receive_buffer1, 1); // Bật lại ngắt nhận
    }
}

