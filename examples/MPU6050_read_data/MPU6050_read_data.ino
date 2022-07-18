#include <Wire.h>
#include <MPU6050.h>

// Settings to use with the setSettings function
#define POWER_SETTINGS    0b00000000
#define MPU_SETTINGS      0b00000110
#define GYRO_SAMPLE_RATE  0b00000111
// For more information on how to use these please refer to
// https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Datasheet1.pdf

/*  
 *   This example shows how to obtain raw values form the sensor
 */

MPU6050 mpu;              // Creating the MPU object

float sensorData[6];      // Creating float array to store the IMU's data

void setup() {
  Serial.begin(115200);   // Start serial communication

  Wire.begin();           // Initialize I2C communication
  Wire.setClock(400000);  // Set I2C clock speed to highest permissable by the MPU6050 sensor

  while(!mpu.begin()) {   // Initialize MPU6050
    Serial.println("MPU6050 could not be initialized! Check sensor and wiring.");
    delay(1000);
  }

  //mpu.setSettings(POWER_SETTINGS, MPU_SETTINGS, GYRO_SAMPLE_RATE);  // Uncomment to change the default MPU settings
  //mpu.setPrescaler(0, 0);                                           // Uncomment to change the accelerometer and gyroscope sensitivty respectively
 
  /*
    Prescaler sensitivity table
  +=======+========+=============+
  | Value |  Acl  |     Gyro     |
  +=======+=======+==============+
  |   0   |  ±2g  |  ±250 deg/s  |
  |       |       |              |
  |   1   |  ±4g  |  ±500 deg/s  |
  |       |       |              |
  |   2   |  ±8g  |  ±1000 deg/s |
  |       |       |              |
  |   3   | ±16g  |  ±2000 deg/s |
  +=======+=======+==============+
  */
  
  mpu.calibrate(500);     // Calibrating offsets for the MPU6050, do not move the sensor during this time
  delay(100);
}

void loop() {
  mpu.getData(sensorData, 6); // Retreives accelerometer and gyroscope data

  Serial.print("Acl X: ");
  Serial.print(sensorData[0]);
  Serial.print("g\tAcl Y: ");
  Serial.print(sensorData[1]);
  Serial.print("g\tAcl Z: ");
  Serial.print(sensorData[2]);
  Serial.print("g\tGyr X: ");
  Serial.print(sensorData[3]);
  Serial.print("deg/s\tGyr Y: ");
  Serial.print(sensorData[4]);
  Serial.print("deg/s\tGyr Z: ");
  Serial.print(sensorData[5]);
  Serial.print("deg/s\n");

  delay(1000);
}
