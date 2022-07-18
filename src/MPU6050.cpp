#include "MPU6050.h"
#include <Wire.h>
#include <Arduino.h>
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
	pos = Wire.endTransmission(false);

    return pos == 0;
}

bool MPU6050::begin() {
	int8_t data[2];
	bool status = 1;

	I2Cwrite(MPU_ADDR, &WHO_AM_I, 1);
	status = I2Cread(MPU_ADDR, data, 1);

	data[0]=PWR_MGMT_1;
	data[1]=DEFAULT_POWER_SETTINGS;
	I2Cwrite(MPU_ADDR, data, 2);

	data[0]=CONFIG;
	data[1]=DEFAULT_CONFIG;
	I2Cwrite(MPU_ADDR, data, 2);

	data[0]=SMPRT_DIV;
	data[1]=GYRO_ACL_SYNC;
	I2Cwrite(MPU_ADDR, data, 2);

	data[0]=GYRO_CONFIG;
	data[1]=DEFAULT_GYR;
	I2Cwrite(MPU_ADDR, data, 2);

	data[0]=ACCEL_CONFIG;
	data[1]=DEFAULT_ACL;
	I2Cwrite(MPU_ADDR, data, 2);
	return status;
}

void MPU6050::setSettings(uint8_t powerMode, uint8_t mpuConf, uint8_t gyroSmplRt) {
	uint8_t data[2];
	
	data[0]=PWR_MGMT_1;
	data[1]=powerMode;
	I2Cwrite(MPU_ADDR, data, 2);

	data[0]=CONFIG;
	data[1]=mpuConfig;
	I2Cwrite(MPU_ADDR, data, 2);

	data[0]=SMPRT_DIV;
	data[1]=gyroSmplRt;
	I2Cwrite(MPU_ADDR, data, 2);

	data[0]=GYRO_CONFIG;
	data[1]=0x00;
	I2Cwrite(MPU_ADDR, data, 2);

	data[0]=ACCEL_CONFIG;
	data[1]=0x00;
	I2Cwrite(MPU_ADDR, data, 2);
}

void MPU6050::setPrescaler(uint8_t aclPrescaler, uint8_t gyroPrescaler) {
	uint8_t data[2];
	data[0]=GYRO_CONFIG;
	data[1]=gyroPrescaler<<3;
	I2Cwrite(MPU_ADDR, data, 2);

	data[0]=ACCEL_CONFIG;
	data[1]=aclPrescaler<<3;
	I2Cwrite(MPU_ADDR, data, 2);

	//aclScale = (float)(CONVERSION_CONSTANT_ACL>>aclPrescaler);
	switch(gyroPrescaler) {
		case 0:
			gyroScale = 131.0;
			break;
		case 1:
			gyroScale = 65.5;
			break;
		case 2:
			gyroScale = 32.8;
			break;
		case 3:
			gyroScale = 16.4;
			break;
	}
}

bool MPU6050::getRaw(int16_t* array, uint8_t len) {
	bool status;
	if(len<6) {return 0;}
	uint8_t input[14];

	I2Cwrite(MPU_ADDR, &ACC_ADR, 1);
	status = I2Cread(MPU_ADDR, input, 14);

	array[0] = (int16_t)(((uint16_t)input[0]) << 8 | input[1]);
	array[1] = (int16_t)(((uint16_t)input[2]) << 8 | input[3]);
	array[2] = (int16_t)(((uint16_t)input[4]) << 8 | input[5]);

	array[3] = (int16_t)(((uint16_t)input[8]) << 8 | input[9]);
	array[4] = (int16_t)(((uint16_t)input[10]) << 8 | input[11]);
	array[5] = (int16_t)(((uint16_t)input[12]) << 8 | input[13]);

	return status;
}

void MPU6050::calibrate(uint8_t samples) {
	float data[6], newOffset[6] = {0,0,0,0,0,0};
	offsets[0] = 0.0;
	offsets[1] = 0.0;
	offsets[2] = 0.0;
	offsets[3] = 0.0;
	offsets[4] = 0.0;
	offsets[5] = 0.0;
	for(uint8_t i = 0; i <samples; i++) {
		getData(data, 6);

		newOffset[0] += data[0];
		newOffset[1] += data[1];
		newOffset[2] += data[2]-1.0;
		newOffset[3] += data[3];
		newOffset[4] += data[4];
		newOffset[5] += data[5];
		delay(2);
	}

	offsets[0] = newOffset[0]/(float)samples;
	offsets[1] = newOffset[1]/(float)samples;
	offsets[2] = newOffset[2]/(float)samples;
	offsets[3] = newOffset[3]/(float)samples;
	offsets[4] = newOffset[4]/(float)samples;
	offsets[5] = newOffset[5]/(float)samples;


}

bool MPU6050::getData(float* array, uint8_t len) {
	
	if(len<6) return 0;
	
	int16_t data[6];
	bool status = getRaw(data, 6);

	array[0] = ((float)data[0])/aclScale - offsets[0];
	array[1] = ((float)data[1])/aclScale - offsets[1];
	array[2] = ((float)data[2])/aclScale - offsets[2];
	array[3] = ((float)data[3])/gyroScale - offsets[3];
	array[4] = ((float)data[4])/gyroScale - offsets[4];
	array[5] = ((float)data[5])/gyroScale - offsets[5];

	return status;
}