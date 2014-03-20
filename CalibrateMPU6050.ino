#include <Average.h>
#include "I2Cdev.h"
#include "MPU6050.h"

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

#define HIST_SIZE 200

MPU6050 accelgyro;

int16_t sensor_temperature;

int16_t ax, ay, az;
int16_t gx, gy, gz;

float aax[HIST_SIZE];
float aay[HIST_SIZE];
float aaz[HIST_SIZE];

float agx[HIST_SIZE];
float agy[HIST_SIZE];
float agz[HIST_SIZE];

int16_t off_ax, off_ay, off_az;
int16_t off_gx, off_gy, off_gz;

int since = millis();
int timestep = 0;

#define OUTPUT_READABLE_ACCELGYRO

#define LED_PIN 13
bool blinkState = false;

void setup() {
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

    Serial.begin(38400);
    delay(5000);
    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();
    accelgyro.setFullScaleGyroRange(MPU6050_GYRO_FS_2000);
    accelgyro.setFullScaleAccelRange(MPU6050_ACCEL_FS_2);

    Serial.println("Testing device connections...");
    Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

    // configure Arduino LED for
    pinMode(LED_PIN, OUTPUT);
}

void loop() {
    sensor_temperature = (accelgyro.getTemperature() + 12412) / 340;
    
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    
    off_ax = accelgyro.getXAccelOffset();
    off_ay = accelgyro.getYAccelOffset();
    off_az = accelgyro.getZAccelOffset();
    off_gx = accelgyro.getXGyroOffset();
    off_gy = accelgyro.getYGyroOffset();
    off_gz = accelgyro.getZGyroOffset();
    
    aax[timestep] = ax;
    aay[timestep] = ay;
    aaz[timestep] = az;
    agx[timestep] = gx;
    agy[timestep] = gy;
    agz[timestep] = gz;
    
    if (millis() - since < 180000) {
        printData();
        blinkState = !blinkState;
        digitalWrite(LED_PIN, blinkState);
        delay(1000);
        return;
    }

    
    if (ax > 0) off_ax--; else if (ax < 0) off_ax++;
    if (ay > 0) off_ay--; else if (ay < 0) off_ay++;
    if (az > 16384) off_az--; else if (az < 16384) off_az++;
    
    if (gx > 0) off_gx--; else if (gx < 0) off_gx++;
    if (gy > 0) off_gy--; else if (gy < 0) off_gy++;
    if (gz > 0) off_gz--; else if (gz < 0) off_gz++;
    
    accelgyro.setXAccelOffset(off_ax);
    accelgyro.setYAccelOffset(off_ay);
    accelgyro.setZAccelOffset(off_az);
    accelgyro.setXGyroOffset(off_gx);
    accelgyro.setYGyroOffset(off_gy);
    accelgyro.setZGyroOffset(off_gz);

    printData();
    // blink LED to indicate activity
    blinkState = !blinkState;
    digitalWrite(LED_PIN, blinkState);    
    timestep++;
    if (timestep >= HIST_SIZE) {
        timestep = 0;
    }
    delay(200);
}

void printData()
{
    Serial.print("temp: ");
    Serial.print(sensor_temperature); Serial.print("\t");
    Serial.print("a/g:\t");
    Serial.print(ax); Serial.print("\t");
    Serial.print(ay); Serial.print("\t");
    Serial.print(az); Serial.print("\t");
    Serial.print(gx); Serial.print("\t");
    Serial.print(gy); Serial.print("\t");
    Serial.print(gz); Serial.print("\t");
    Serial.print(off_ax); Serial.print("\t");
    Serial.print(off_ay); Serial.print("\t");
    Serial.print(off_az); Serial.print("\t");
    Serial.print(off_gx); Serial.print("\t");
    Serial.print(off_gy); Serial.print("\t");
    Serial.print(off_gz); Serial.print("\t");
    Serial.print(mean(aax, HIST_SIZE)); Serial.print("\t");
    Serial.print(mean(aay, HIST_SIZE)); Serial.print("\t");
    Serial.print(mean(aaz, HIST_SIZE)); Serial.print("\t");
    Serial.print(mean(agx, HIST_SIZE)); Serial.print("\t");
    Serial.print(mean(agy, HIST_SIZE)); Serial.print("\t");
    Serial.print(mean(agz, HIST_SIZE)); Serial.print("\t");
    Serial.print(stddev(aax, HIST_SIZE)); Serial.print("\t");
    Serial.print(stddev(aay, HIST_SIZE)); Serial.print("\t");
    Serial.print(stddev(aaz, HIST_SIZE)); Serial.print("\t");
    Serial.print(stddev(agx, HIST_SIZE)); Serial.print("\t");
    Serial.print(stddev(agy, HIST_SIZE)); Serial.print("\t");
    Serial.print(stddev(agz, HIST_SIZE));
    Serial.println("");
}
