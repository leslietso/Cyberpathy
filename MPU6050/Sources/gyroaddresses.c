/*
 * gyroaddresses.c
 *
 *  Created on: Nov 30, 2015
 *      Author: Leslie
 */
#include "gyroaddresses.h"

typedef union accel_t_gyro_union
{
  struct
  {
    uint8_t x_accel_h;
    uint8_t x_accel_l;
    uint8_t y_accel_h;
    uint8_t y_accel_l;
    uint8_t z_accel_h;
    uint8_t z_accel_l;
    uint8_t t_h;
    uint8_t t_l;
    uint8_t x_gyro_h;
    uint8_t x_gyro_l;
    uint8_t y_gyro_h;
    uint8_t y_gyro_l;
    uint8_t z_gyro_h;
    uint8_t z_gyro_l;
  } reg;
  struct
  {
    int16_t x_accel;
    int16_t y_accel;
    int16_t z_accel;
    int16_t temperature;
    int16_t x_gyro;
    int16_t y_gyro;
    int16_t z_gyro;
  } value;
};

int MPU6050_read(int start, uint8_t *buffer, int size)
{
  int i, n, error;
 
  GI2C1_RequestBus();
  //Wire.beginTransmission(MPU6050_I2C_ADDRESS);
  
  error = GI2C1_ReadByteAddress8(MPU_addr, start, buffer);
 
  n = Wire.endTransmission(false);    // hold the I2C-bus
  if (n != 0)
    return (n);
 
  // Third parameter is true: relase I2C-bus after data is read.
  Wire.requestFrom(MPU6050_I2C_ADDRESS, size, true);
  i = 0;
  while(Wire.available() && i<size)
  {
    buffer[i++]=Wire.read();
  }
  if ( i != size)
    return (-11);
  
  GI2C1_ReleaseBus();
 
  return (0);  // return : no error
}


