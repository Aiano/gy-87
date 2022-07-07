

#include "hmc5883l.h"
#include <math.h>

#define HMC5883L_ADDR 0x1E // 7-bit address
#define HMC5883L_READ 0x3D // 8-bit read address
#define HMC5883L_WRITE 0x3C // 8-bit write address

// Read/Write
#define Configuration_Register_A 0x00
#define Configuration_Register_B 0x01
#define Mode_Register 0x02
// Read
#define Data_Output_X_MSB_Register 0x03
#define Status_Register 0x09
#define Identification_Register_A 0x0A
#define Identification_Register_B 0x0B
#define Identification_Register_C 0x0C

#define I2C_TIMEOUT 0xFF

void HMC5883L_Init(I2C_HandleTypeDef *I2Cx) {
    uint8_t Data;

    // Set number of samples averaged per measurement output to 8
    // Set data output rate to 15Hz
    // Set normal measurement configuration
    // 0111 0000
    Data = 0x70;
    HAL_I2C_Mem_Write(I2Cx, HMC5883L_WRITE, Configuration_Register_A, I2C_MEMADD_SIZE_8BIT, &Data, 1, I2C_TIMEOUT);

    // Set gain to 1090 LSB/Gauss -> +- 1.3 Ga
    // Output Range: -2048 ~ 2047
    // 0010 0000
    Data = 0x20;
    HAL_I2C_Mem_Write(I2Cx, HMC5883L_WRITE, Configuration_Register_B, I2C_MEMADD_SIZE_8BIT, &Data, 1, I2C_TIMEOUT);

    // Select Continuous-Measurement Mode
    // 0000 0000
    Data = 0x00;
    HAL_I2C_Mem_Write(I2Cx, HMC5883L_WRITE, Mode_Register, I2C_MEMADD_SIZE_8BIT, &Data, 1, I2C_TIMEOUT);

    return;
}

void HMC5883L_Read_All(I2C_HandleTypeDef *I2Cx, HMC5883L_t *DataStruct) {
    uint8_t receive_data[6];

    // Read output data
    HAL_I2C_Mem_Read(I2Cx, HMC5883L_READ, Data_Output_X_MSB_Register, I2C_MEMADD_SIZE_8BIT, receive_data, 6,
                     I2C_TIMEOUT);
    DataStruct->Magnet_X_RAW = (int16_t) (receive_data[0] << 8 | receive_data[1]);
    DataStruct->Magnet_Z_RAW = (int16_t) (receive_data[2] << 8 | receive_data[3]);
    DataStruct->Magnet_Y_RAW = (int16_t) (receive_data[4] << 8 | receive_data[5]);

    // Calibration
    DataStruct->Magnet_X_Calib = DataStruct->Magnet_X_RAW - DataStruct->Magnet_X_offset;
    DataStruct->Magnet_Y_Calib = DataStruct->Magnet_Y_RAW - DataStruct->Magnet_Y_offset;
    DataStruct->Magnet_Z_Calib = DataStruct->Magnet_Z_RAW - DataStruct->Magnet_Z_offset;

    // Calculate radian and degree
    DataStruct->radian = atan2((double) DataStruct->Magnet_Y_Calib, (double) DataStruct->Magnet_X_Calib);
    DataStruct->degree = DataStruct->radian / 3.1415926 * 180.0 + 180.0;
}


