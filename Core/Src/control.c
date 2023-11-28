
#include <control.h>

uint8_t light_status = 0;
char res[7];
static void reverse(char* str, int len)
{
    int i = 0, j = len - 1, temp;
    while (i < j) {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
}
// Converts a given integer x to string str[].
// d is the number of digits required in the output.
// If d is more than the number of digits in x,
// then 0s are added at the beginning.
static int intToStr(int x, char str[], int d) {
	int i = 0;
	while (x) {
		str[i++] = (x % 10) + '0';
		x = x / 10;
	}

	// If number of digits required is more, then
	// add 0s at the beginning
	while (i < d)
		str[i++] = '0';

	reverse(str, i);
	str[i] = '\0';
	return i;
}

// Converts a floating-point/double number to a string.
void ftoa(float n) {
	// Extract integer part
	int ipart = (int) n;

	// Extract floating part
	float fpart = n - (float) ipart;

	// convert integer part to string
	int i = intToStr(ipart, res, 0);

	// check for display option after point

	res[i] = '.'; // add dot

	// Get the value of fraction part upto given no.
	// of points after dot. The third parameter
	// is needed to handle cases like 233.007
	fpart = fpart * 10 * 10;

	intToStr((int) fpart, res + i + 1, 2);
}

void send_data(void){
	char str[20];
	sprintf(str, "!TEMP:%s#", res);
	uart_EspSendString((uint8_t*)str);
}

void lightProcess(){
	if(button_count[13] == 1){
		light_status = 1 - light_status;
		if(light_status == 1){
			uart_EspSendBytes("A", 1);
		} else {
			uart_EspSendBytes("a", 1);
		}
	}
	if(light_status == 1){
		HAL_GPIO_WritePin(OUTPUT_Y0_GPIO_Port, OUTPUT_Y0_Pin, 1);
	} else {
		HAL_GPIO_WritePin(OUTPUT_Y0_GPIO_Port, OUTPUT_Y0_Pin, 0);
	}
}

void test_Esp(){
	if(uart_EspCheck() == 0) uart_EspSendBytes("o", 1);
	else lcd_ShowStr(10, 50, "ESP Connect", GREEN, BLACK, 24, 0);
}
