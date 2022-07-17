#include <Wire.h>
#include <MPU6050.h>

/*  
 *   This example shows how to obtain raw values form the sensor
 */

MPU6050 mpu;

int16_t sensorData[6];

void setup() {
  Serial.begin(115200);

  Wire.begin();           // Initialize I2C communication
  Wire.setClock(400000);  // Set I2C clock speed to highest permissable by the MPU6050 sensor

  while(!mpu.begin()) {   // Initialize MPU6050
    Serial.println("MPU6050 could not be initialized! Check sensor and wiring.");
    delay(1000);
  }
}

void loop() {
  mpu.getData(sensorData, 6); // Retreives accelerometer and gyroscope data

  Serial.print(sensorData[0]);
  Serial.print('\t');
  Serial.print(sensorData[1]);
  Serial.print('\t');
  Serial.print(sensorData[2]);
  Serial.print('\t');
  Serial.print(sensorData[3]);
  Serial.print('\t');
  Serial.print(sensorData[4]);
  Serial.print('\t');
  Serial.print(sensorData[5]);
  Serial.print('\n');
  delay(1000);
}
