/***************************************************
   ANFF OPV Solar Monitor
   Date: 23-01-2024
   Author: Matthew G. Barr
   Project: Bangkok OPV demonstrator

 ****************************************************/
// Libraries
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_INA219.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

// Definitions
#define TFT_SCK  13
#define TFT_MISO 12
#define TFT_MOSI 11
#define TFT_CS   10
#define TFT_DC   8
#define TFT_RST  9

// Global variables
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
Adafruit_INA219 ina219(0x40);
uint8_t voltage_divider_pin = A0;
uint16_t update_period = 250;
unsigned long time_now = 0;
float voltage_V = 0; 
uint16_t current_mA = 0;
float power_W = 0;
float energy_Wh_d = 0;

// Preallocated char arrays
char current[6];
char current_label[10] = "Current: ";
char current_units[3] = "mA";
char voltage[6];
char voltage_label[10] = "Voltage: ";
char voltage_units[2] = "V"; 
char power[6];
char power_label[8] = "Power: ";
char power_units[2] = "W";
char energy[6];
char energy_label[9] = "Energy: ";
char energy_units[8] = "Wh/day";


void setup(void) {
  Serial.begin(115200);
  while (!Serial){
    delay(10);
  }

  if (! ina219.begin()) {
    Serial.println("Error: failed to find INA219 current meter.");
    while (1) { delay(10); }
  }

  //ina219.setCalibration_32V_1A();
  tft.initR(INITR_BLACKTAB); 
  tft.fillScreen(ST77XX_BLACK);
  tft.setRotation(1);
  drawText(10, 50, 2, "OPV Testing", ST77XX_WHITE, ST77XX_BLACK);
  delay(1000);
  tft.fillScreen(ST77XX_BLACK);
  
}


void loop(void) 
{
  if((millis() - time_now) > update_period) {
    time_now = millis();
    
    voltage_V = analogRead(voltage_divider_pin) / (10.24);
    dtostrf(voltage_V, 5, 1, voltage);

    current_mA = ina219.getCurrent_mA();
    if (current_mA <= 0) {
      current_mA = 0;
    }
    dtostrf(current_mA, 5, 0, current);
    
    power_W = voltage_V * current_mA / 1000;
    dtostrf(power_W, 5, 2, power);

    energy_Wh_d = power_W * 9;
    dtostrf(energy_Wh_d, 5, 0 , energy);

    // Serial.print("Voltage: "); Serial.print(voltage_V, 1);  Serial.print(" V,   ");
    // Serial.print("Current: "); Serial.print(current_mA); Serial.print(" mA,   ");
    // Serial.print("Power: "); Serial.print(power_W); Serial.print(" W,   ");
    // Serial.print("Energy: "); Serial.print(energy_Wh_d, 0); Serial.println(" Wh/day");

    drawText(2, 5, 1, voltage_label, ST77XX_WHITE, ST77XX_BLACK);
    drawText(50, 5, 1, voltage, ST77XX_WHITE, ST77XX_BLACK);
    drawText(86, 5, 1, voltage_units, ST77XX_WHITE, ST77XX_BLACK);

    drawText(2, 18, 1, current_label, ST77XX_WHITE, ST77XX_BLACK);
    drawText(48, 18, 1, current, ST77XX_WHITE, ST77XX_BLACK);
    drawText(86, 18, 1, current_units, ST77XX_WHITE, ST77XX_BLACK);

    drawText(2, 31, 1, power_label, ST77XX_WHITE, ST77XX_BLACK);
    drawText(48, 31, 1, power, ST77XX_WHITE, ST77XX_BLACK);
    drawText(86, 31, 1, power_units, ST77XX_WHITE, ST77XX_BLACK);
    
    drawText(2, 44, 1, energy_label, ST77XX_WHITE, ST77XX_BLACK);
    drawText(48, 44, 1, energy, ST77XX_WHITE, ST77XX_BLACK);
    drawText(86, 44, 1, energy_units, ST77XX_WHITE, ST77XX_BLACK);

  }
}


void drawText(int x_pos, int y_pos, int text_size, String text_literal, uint16_t text_colour, uint16_t text_background_colour) {
  tft.setCursor(x_pos, y_pos);
  tft.setTextColor(text_colour, text_background_colour);
  tft.setTextSize(text_size);
  tft.setTextWrap(false);
  tft.print(text_literal);
}
