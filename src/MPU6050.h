#ifndef MPU6050_H_
#define MPU6050_H_
#include <stdint.h>

class MPU6050 {
    
    private:

        inline static const uint8_t MPU_ADDR			= 0x68;

        inline static const uint8_t WHO_AM_I			= 0x75;
        inline static const uint8_t CONFIG				= 0x1A;
        inline static const uint8_t GYRO_CONFIG		= 0x1B;
        inline static const uint8_t ACCEL_CONFIG		= 0x1C;

        inline static const uint8_t ACC_ADR			= 0x3B;
        inline static const uint8_t GYR_ADR			= 0x43;
        inline static const uint8_t PWR_MGMT_1			= 0x6B;

        inline static const uint8_t DEFAULT_CONFIG 	= 0b00000110;
        inline static const uint8_t DEFAULT_ACL		= 0b00000000;
        inline static const uint8_t DEFAULT_GYR		= 0b00000000;

        bool I2Cwrite(uint8_t addr, uint8_t* data, uint8_t len);

        bool I2Cread(uint8_t addr, uint8_t* data, uint8_t len);

    public:

        MPU6050();

        bool begin();

        bool getData(int16_t* array, uint8_t len);
};
#endif /* MPU6050_H_ */