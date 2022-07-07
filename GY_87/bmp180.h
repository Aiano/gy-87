
#ifndef GY_87_BMP180_H
#define GY_87_BMP180_H

#include "main.h"

typedef struct {
    int16_t temperature;
    int16_t pressure;
} BMP180_t;

void BMP180_init(I2C_HandleTypeDef *I2Cx);
void BMP180_start_measurement();
void BMP180_read_all(BMP180_t *data_struct);



#endif //GY_87_BMP180_H
