#ifndef MPU6050_H_
#define MPU6050_H_
#include <stdint.h>

class MPU6050 {
    
    private:

        inline static const uint8_t MPU_ADDR			     = 0x68;

        inline static const uint8_t WHO_AM_I			     = 0x75;
        inline static const uint8_t CONFIG				     = 0x1A;
        inline static const uint8_t GYRO_CONFIG		         = 0x1B;
        inline static const uint8_t ACCEL_CONFIG		     = 0x1C;
        inline static const uint8_t SMPRT_DIV                = 0x19;

        inline static const uint8_t ACC_ADR			         = 0x3B;
        inline static const uint8_t GYR_ADR			         = 0x43;
        inline static const uint8_t PWR_MGMT_1			     = 0x6B;
        
        inline static const uint8_t DEFAULT_POWER_SETTINGS   = 0x00; 
        inline static const uint8_t DEFAULT_CONFIG 	         = 0b00000110;
        inline static const uint8_t DEFAULT_ACL		         = 0b00000000;
        inline static const uint8_t DEFAULT_GYR		         = 0b00000000;
        inline static const uint8_t GYRO_ACL_SYNC            = 0b00000111;
        inline static const uint16_t CONVERSION_CONSTANT_ACL = 16384;
        float gyroScale = 131.0, aclScale = 16384.0;

        bool I2Cwrite(uint8_t addr, uint8_t* data, uint8_t len);

        bool I2Cread(uint8_t addr, uint8_t* data, uint8_t len);

        float offsets[6];

    public:

        MPU6050();

        bool begin();

        void setSettings(uint8_t powerMode, uint8_t mpuConf, uint8_t gyroSmplRt);

        void setPrescaler(uint8_t aclPrescaler, uint8_t gyroPrescaler);

        void calibrate(uint8_t samples);

        bool getRaw(int16_t* array, uint8_t len);

        bool getData(float* array, uint8_t len);
};
#endif /* MPU6050_H_ */