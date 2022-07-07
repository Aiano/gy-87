#ifndef GY_87_HMC5883L_H
#define GY_87_HMC5883L_H


#include "i2c.h"

typedef struct {
    int16_t Magnet_X_RAW;
    int16_t Magnet_Y_RAW;
    int16_t Magnet_Z_RAW;
    int16_t Magnet_X_offset;
    int16_t Magnet_Y_offset;
    int16_t Magnet_Z_offset;
    int16_t Magnet_X_Calib;
    int16_t Magnet_Y_Calib;
    int16_t Magnet_Z_Calib;

    double radian;
    double degree;
} HMC5883L_t;

void HMC5883L_Init(I2C_HandleTypeDef *I2Cx);
void HMC5883L_Read_All(I2C_HandleTypeDef *I2Cx, HMC5883L_t *DataStruct);
#endif //GY_87_HMC5883L_H
