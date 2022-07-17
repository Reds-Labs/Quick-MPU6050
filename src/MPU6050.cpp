#include "MPU6050.h"
#include <Wire.h>

MPU6050::MPU6050(){}

bool MPU6050::I2Cread(uint8_t addr,  uint8_t* data,  uint8_t len) {
	uint8_t pos = 0;

	Wire.beginTransmission(addr);
	Wire.requestFrom(addr, len);

	while(Wire.available() && pos<len) {
		data[pos] = Wire.read();
    	pos++;
  	}
	return pos == len;
}

bool MPU6050::I2Cwrite(uint8_t addr,  uint8_t* data, uint8_t len) {
	uint8_t pos = 0;
	Wire.beginTransmission(addr);
	Wire.write(data, len);
	pos = Wire.endTransmission();

    return pos == 0;
}

bool MPU6050::begin() {

	int8_t data[2];
	bool status = 1;

	I2Cwrite(MPU_ADDR, &WHO_AM_I, 1);
	status = I2Cread(MPU_ADDR, data, 1);

	data[0]=PWR_MGMT_1;
	data[1]=0x00;
	I2Cwrite(MPU_ADDR, data, 2);

	data[0]=CONFIG;
	data[1]=DEFAULT_CONFIG;
	I2Cwrite(MPU_ADDR, data, 2);

	data[0]=0x19;
	data[1]=0x07;
	I2Cwrite(MPU_ADDR, data, 2);

	data[0]=GYRO_CONFIG;
	data[1]=DEFAULT_GYR;
	I2Cwrite(MPU_ADDR, data, 2);

	data[0]=ACCEL_CONFIG;
	data[1]=DEFAULT_ACL;
	I2Cwrite(MPU_ADDR, data, 2);
	return status;
}

bool MPU6050::getData(int16_t* array, uint8_t len) {
	bool status;
	if(len<6) {return 0;}
	int8_t input[14];

	I2Cwrite(MPU_ADDR, &ACC_ADR, 1);
	status = I2Cread(MPU_ADDR, input, 14);

	array[0] = ((int16_t)input[0]) << 8 | input[1];
	array[1] = ((int16_t)input[2]) << 8 | input[3];
	array[2] = ((int16_t)input[4]) << 8 | input[5];

	array[3] = ((int16_t)input[8]) << 8 | input[9];
	array[4] = ((int16_t)input[10]) << 8 | input[11];
	array[5] = ((int16_t)input[12]) << 8 | input[13];

	return status;
}


