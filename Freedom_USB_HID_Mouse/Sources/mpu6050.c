/*
 * mpu6050.c
 *
 *  Created on: Dec 7, 2015
 *      Author: Leslie
 */
#include "mpu6050.h"

uint8_t MPU6050_Init(void)
{
	return GI2C1_WriteByteAddress8(MPU_addr, WUP_addr, 0x00); /* enable device */
}

int16_t MPU6050_GetGyX(void)
{
  int16_t value;

  value = (int16_t)MPU6050_MeasureGetRawGyX(); /* remove 2 bits (16bit to 14bit) */
  //value -= CalNxOff; /* adjust with calibration offset */
  return value;
}

uint16_t MPU6050_MeasureGetRawGyX(void)
{
  union {
    uint8_t buf[2]; /* value from device is in big endian */
    uint16_t be;
  } val;
  static const uint8_t addr = MPU6050_GYRO_XOUT_H;

  if(GI2C1_ReadAddress(MPU_addr, (uint8_t*)&addr, sizeof(addr), &val.buf[0], sizeof(val.buf))!=ERR_OK) {
    return 0; /* failure */
  }
  return val.be; /* already in BE */
}


