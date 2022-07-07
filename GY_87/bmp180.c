
#include "bmp180.h"


// i2c timeout
#define I2C_TIMEOUT 0xFF

// bmp180 device address
#define BMP180_WRITE 0xEE
#define BMP180_READ 0xEF

// bmp180 registers address
#define OUT_XLASB 0xF8
#define OUT_LSB 0xF7
#define OUT_MSB 0xF6
#define CTRL_MEAS 0xF4
#define SOFT_RESET 0xE0
#define ID 0xD0
#define CALIB21 0xBF

// control register value
// Measurement      Register value      Max conversion time
// Temperature      0x2E                4.5
// Pressure(oss=0)  0x34                4.5
// Pressure(oss=1)  0x74                7.5
// Pressure(oss=2)  0xB4                13.5
// Pressure(oss=3)  0xF4                25.5
#define CTRL_MEAS_TEMP 0x2E
#define CTRL_MEAS_PRES 0x34

I2C_HandleTypeDef *_I2Cx;

void BMP180_init(I2C_HandleTypeDef *I2Cx) {
    _I2Cx = I2Cx;
    return;
}

// Start temperature and pressure measurement
void BMP180_start_measurement() {
    uint8_t send_data = CTRL_MEAS_PRES;
    HAL_I2C_Mem_Write(_I2Cx, BMP180_WRITE, CTRL_MEAS, I2C_MEMADD_SIZE_8BIT, &send_data, 1, I2C_TIMEOUT);
    HAL_Delay(5);
    return;
}

void BMP180_read_all(BMP180_t *data_struct) {
    uint8_t receive_data[2];
    uint8_t send_data;

    // get temperature
    send_data = CTRL_MEAS_TEMP;
    HAL_I2C_Mem_Write(_I2Cx, BMP180_WRITE, CTRL_MEAS, I2C_MEMADD_SIZE_8BIT, &send_data, 1, I2C_TIMEOUT);
    HAL_I2C_Mem_Read(_I2Cx, BMP180_READ, OUT_MSB, I2C_MEMADD_SIZE_8BIT, receive_data, 2, I2C_TIMEOUT);
    data_struct->temperature = (int16_t) (receive_data[0] << 8 | receive_data[1]);

    // get pressure
    send_data = CTRL_MEAS_PRES;
    HAL_I2C_Mem_Write(_I2Cx, BMP180_WRITE, CTRL_MEAS, I2C_MEMADD_SIZE_8BIT, &send_data, 1, I2C_TIMEOUT);
    HAL_I2C_Mem_Read(_I2Cx, BMP180_READ, OUT_MSB, I2C_MEMADD_SIZE_8BIT, receive_data, 2, I2C_TIMEOUT);
    data_struct->pressure = (int16_t) (receive_data[0] << 8 | receive_data[1]);
    return;
}


