#include "sensor.h"

uint16_t adc_receive[5];
uint16_t heat_buffer[BUFFER_SIZE];
float sum = 0;
static float sensor_GetTemperature() {
	return ((float) adc_receive[4] * 330) / (4095);
}

void sensor_init() {
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*) adc_receive, 5);
}

void sensor_Read() {
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*) adc_receive, 5);
}

void sensor_CalTemperature(uint16_t idx) {
	heat_buffer[idx] = sensor_GetTemperature();
	sum += heat_buffer[idx];
}
float sensor_result(void) {
	float mean = sum / BUFFER_SIZE;
	sum = 0;
	return mean;
}
