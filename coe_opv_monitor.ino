/***************************************************
   ANFF OPV Solar Monitor
   Date: 22-01-2024
   Author: Matthew G Barr

 ****************************************************/
// Libraries
#include <Wire.h>
#include <Adafruit_INA219.h>

Adafruit_INA219 ina219(0x40);

uint8_t voltage_divider_pin = A0;
float voltage_V = 0; 
uint16_t current_mA = 0;
float power_W = 0;
float energy_Wh_d = 0;

void setup(void) 
{
  Serial.begin(115200);
  while (!Serial) 
  {
    delay(10);
  }

  if (! ina219.begin()) 
  {
    Serial.println("Failed to find INA219 current meter");
    while (1) { delay(10); }
  }

  //ina219.setCalibration_32V_1A();
  //ina219.setCalibration_16V_400mA();

}

void loop(void) 
{
  voltage_V = analogRead(voltage_divider_pin) / (10.24);
  current_mA = ina219.getCurrent_mA();
  power_W = voltage_V * current_mA / 1000;
  energy_Wh_d = power_W * 3600 * 9;

  Serial.print("Voltage: "); Serial.print(voltage_V, 1);  Serial.print(" V,   ");
  Serial.print("Current: "); Serial.print(current_mA); Serial.print(" mA,   ");
  Serial.print("Power: "); Serial.print(power_W); Serial.print(" W,   ");
  Serial.print("Energy: "); Serial.print(energy_Wh_d, 0); Serial.println(" Whr/day");

  delay(1000);
}
