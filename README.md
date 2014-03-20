CalibrateMPU6050
================

Calibrate MPU6050 procedure


Setup
-----

1. Copy the folders from libraries/ to your Arduino IDE workspace libraries folder
2. Compile and Run in Arduino IDE


How to use
----------

* For the first 180 seconds, the Calibration procedure is in WARM-UP mode.
* After 180 seconds, the procedure adjusts the offset value to satisfy the sensor reading to expected value.
* By observing the offset values, the mean of sensor readings and the standard deviation in Serial Monitor, pick the offset values when they become stable.
* Use the offset value in your program as below:

```arduino
#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"

#define MPU6050_ACCEL_OFFSET_X -773
#define MPU6050_ACCEL_OFFSET_Y 3278
#define MPU6050_ACCEL_OFFSET_Z 1660
#define MPU6050_GYRO_OFFSET_X  -24
#define MPU6050_GYRO_OFFSET_Y  -62
#define MPU6050_GYRO_OFFSET_Z  -6
  
MPU6050 mpu;
  
void setup() {
    Wire.begin();
    mpu.initialize();
    mpu.setXAccelOffset(MPU6050_ACCEL_OFFSET_X);
    mpu.setYAccelOffset(MPU6050_ACCEL_OFFSET_Y);
    mpu.setZAccelOffset(MPU6050_ACCEL_OFFSET_Z);
    mpu.setXGyroOffset(MPU6050_GYRO_OFFSET_X);
    mpu.setYGyroOffset(MPU6050_GYRO_OFFSET_Y);
    mpu.setZGyroOffset(MPU6050_GYRO_OFFSET_Z);
}
  
void loop() {
}
```

