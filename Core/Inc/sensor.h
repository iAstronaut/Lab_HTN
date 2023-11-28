#ifndef INC_SENSOR_H_
#define INC_SENSOR_H_

#include "adc.h"
#define BUFFER_SIZE	10
void sensor_init();

void sensor_Read();
void sensor_CalTemperature(uint16_t idx);
float sensor_result(void);
#endif /* INC_SENSOR_H_ */
