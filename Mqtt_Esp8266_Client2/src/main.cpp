#include <Arduino.h>
#include "DFRobot_AHT20.h"

DFRobot_AHT20 aht20;

void setup()
{
  Serial.begin(115200);
  while (!Serial)
  {
    // Wait for USB serial port to connect. Needed for native USB port only
  }

  uint8_t status;
  while ((status = aht20.begin()) != 0)
  {
    Serial.print("AHT20 sensor initialization failed. error status : ");
    Serial.println(status);
    delay(1000);
  }
}

void loop()
{
  if (aht20.startMeasurementReady(/* crcEn = */ true))
  {
    Serial.print("temperature(-40~85 C): ");
    // Get temp in Celsius (℃), range -40-80℃
    Serial.print(aht20.getTemperature_C());
    Serial.print(" C, ");
    // Get temp in Fahrenheit (F)
    Serial.print(aht20.getTemperature_F());
    Serial.print(" F\t");
    Serial.print("humidity(0~100): ");
    // Get relative humidity (%RH), range 0-100℃
    Serial.print(aht20.getHumidity_RH());
    Serial.println(" %RH");
    delay(1000);
  }
}