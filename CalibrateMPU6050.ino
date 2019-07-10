#include <Average.h>
#include "I2Cdev.h"
#include "MPU6050.h"


#define BAUD 115200

/* If your sensor's VCC is hooked to a GPIO pin define it here */
#define VCC_PIN GPIO_NUM_5


#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

#ifdef ESP32
// I2C pins
#define SDA_PIN GPIO_NUM_18
#define SCL_PIN GPIO_NUM_19
#endif // ESP32
// I2C bus speed in KHz 100 - 400
#define WIRE_SPEED 400

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

//#define LED_PIN 13
bool blinkState = false;

void setup() {
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#  ifdef ESP32
        Wire.begin( SDA_PIN, SCL_PIN, WIRE_SPEED * 1000);
#  else // ESP32
        Wire.begin();
#  endif // ESP32
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup( WIRE_SPEED, true);
#endif

    Serial.begin( BAUD);
    delay(5000);

#ifdef VCC_PIN    // MPUs VCC pin hooked to IO pin?  powercycle it!
    pinMode( VCC_PIN, OUTPUT);
    digitalWrite( VCC_PIN, LOW);
    delay( 100);
    digitalWrite( VCC_PIN, HIGH);
    delay( 100);
#endif // VCC
    
    accelgyro.reset();
    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();
    accelgyro.setFullScaleGyroRange(MPU6050_GYRO_FS_2000);
    accelgyro.setFullScaleAccelRange(MPU6050_ACCEL_FS_2);

    Serial.println("Testing device connections...");
    if( accelgyro.testConnection() )
    {
      Serial.println("MPU6050 connection successful");
    }
    else
    {
      Serial.println("MPU6050 connection failed");
      // no need to continue
      while( true )
        ;
    }

#ifdef LED_PIN
    // configure Arduino LED for
    pinMode(LED_PIN, OUTPUT);
#endif LED_PIN
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
#ifdef LED_PIN
        digitalWrite(LED_PIN, blinkState);
#endif LED_PIN
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
#ifdef LED_PIN
    digitalWrite(LED_PIN, blinkState);    
#endif LED_PIN
    timestep++;
    if (timestep >= HIST_SIZE) {
        timestep = 0;
    }
    delay(200);
}

void printData()
{
    Serial.print("temp: ");
    Serial.print(sensor_temperature);
    Serial.print("\ta/g:\t");
    Serial.print(ax); Serial.print("\t");
    Serial.print(ay); Serial.print("\t");
    Serial.print(az); Serial.print("\t");
    Serial.print(gx); Serial.print("\t");
    Serial.print(gy); Serial.print("\t");
    Serial.print(gz); Serial.print("\t");
    Serial.print("\toffset:\t");
    Serial.print(off_ax); Serial.print("\t");
    Serial.print(off_ay); Serial.print("\t");
    Serial.print(off_az); Serial.print("\t");
    Serial.print(off_gx); Serial.print("\t");
    Serial.print(off_gy); Serial.print("\t");
    Serial.print(off_gz); Serial.print("\t");
    Serial.print("\tmean:\t");
    Serial.print(mean(aax, HIST_SIZE)); Serial.print("\t");
    Serial.print(mean(aay, HIST_SIZE)); Serial.print("\t");
    Serial.print(mean(aaz, HIST_SIZE)); Serial.print("\t");
    Serial.print(mean(agx, HIST_SIZE)); Serial.print("\t");
    Serial.print(mean(agy, HIST_SIZE)); Serial.print("\t");
    Serial.print(mean(agz, HIST_SIZE)); Serial.print("\t");
    Serial.print("\tstddev:\t");
    Serial.print(stddev(aax, HIST_SIZE)); Serial.print("\t");
    Serial.print(stddev(aay, HIST_SIZE)); Serial.print("\t");
    Serial.print(stddev(aaz, HIST_SIZE)); Serial.print("\t");
    Serial.print(stddev(agx, HIST_SIZE)); Serial.print("\t");
    Serial.print(stddev(agy, HIST_SIZE)); Serial.print("\t");
    Serial.print(stddev(agz, HIST_SIZE));
    Serial.println();
}

