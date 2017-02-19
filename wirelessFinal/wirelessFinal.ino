//gyro
#include "I2Cdev.h"
#include "MPU6050.h"
//wireless
#include <SPI.h>
#include <RH_NRF24.h>
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif
MPU6050 accelgyro;
// Singleton instance of the radio driver
RH_NRF24 nrf24;
//MPU6050 accelgyro(0x69); // <-- use for AD0 high
int16_t ax, ay, az;
int16_t gx, gy, gz;
#define OUTPUT_READABLE_ACCELGYRO

//Declaration
int blueLED = 3;
int speaker = 7;
void setup()
{
  Serial.begin(9600);
  pinMode(blueLED, OUTPUT);
  while (!Serial);
  if (!nrf24.init())
    Serial.println("init failed");
  // Defaults after init are 2.402 GHz (channel 2), 2Mbps, 0dBm
  if (!nrf24.setChannel(2))
    Serial.println("setChannel failed");
  if (!nrf24.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm))
    Serial.println("setRF failed");

    //-----Gyros----
    // join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

    // initialize serial communication
    // (38400 chosen because it works as well at 8MHz as it does at 16MHz, but
    // it's really up to you depending on your project)
//    Serial.begin(38400);

    // initialize device
    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();

    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
       
}

int tick=0;
void loop()
{
    // read raw accel/gyro measurements from device
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    // these methods (and a few others) are also available
    //accelgyro.getAcceleration(&ax, &ay, &az);
    //accelgyro.getRotation(&gx, &gy, &gz);
//
    #ifdef OUTPUT_READABLE_ACCELGYRO
        // display tab-separated accel/gyro x/y/z values
        Serial.print("a/g:\t");
        Serial.print(ax/1000); Serial.print("\t");
        Serial.print(ay/1000); Serial.print("\t");
        Serial.print(az/1000); Serial.print("\t");
        Serial.print(gx/1000); Serial.print("\t");
        Serial.print(gy/1000); Serial.print("\t");
        Serial.println(gz/1000);
    #endif
//
//    #ifdef OUT PUT_BINARY_ACCELGYRO
//        Serial.write((uint8_t)(ax >> 8)); Serial.write((uint8_t)(ax & 0xFF));
//        Serial.write((uint8_t)(ay >> 8)); Serial.write((uint8_t)(ay & 0xFF));
//        Serial.write((uint8_t)(az >> 8)); Serial.write((uint8_t)(az & 0xFF));
//        Serial.write((uint8_t)(gx >> 8)); Serial.write((uint8_t)(gx & 0xFF));
//        Serial.write((uint8_t)(gy >> 8)); Serial.write((uint8_t)(gy & 0xFF));
//        Serial.write((uint8_t)(gz >> 8)); Serial.write((uint8_t)(gz & 0xFF));
//    #endif
    int ccc=ax/1000;
    //button is pressed, message should be sent, turn the green LED on
    digitalWrite(blueLED, HIGH);

    // Send a message
    uint8_t data[] = {char(ax/1000),char(ay/1000),char(az/1000)};
//    Serial.print(ax/1000);
//    Serial.print(" | ");
//    Serial.println((int)(char(ax/100)));
    nrf24.send(data, sizeof(data));
//    delay(200);
    nrf24.waitPacketSent();
    digitalWrite(blueLED, LOW);

}
