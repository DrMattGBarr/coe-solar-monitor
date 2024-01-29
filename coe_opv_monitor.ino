/***************************************************
   ANFF OPV Solar Monitor
   Date: 29-01-2024
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
#define TFT_RST  9
#define TFT_DC   8
#define TFT_LITE 6

// External library objects
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
Adafruit_INA219 ina219(0x40);

// Global variables
uint8_t voltage_divider_pin = A0;
uint16_t update_period = 250;
unsigned long time_now = 0;
float voltage_V = 0; 
uint16_t current_mA = 0;
float power_W = 0;
float energy_Wh_d = 0;
float energy_scaling_factor = 5.0;

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
char energy_units[7] = "Wh/day";


void setup(void) {
  Serial.begin(115200);
  pinMode(TFT_LITE, OUTPUT);

  //ina219.setCalibration_32V_1A();
  tft.initR(INITR_BLACKTAB); 
  digitalWrite(TFT_LITE, HIGH);
  tft.fillScreen(ST77XX_BLACK);
  tft.setRotation(3);
  drawText(2, 5, 1, "Connecting to INA219..." ,ST77XX_WHITE, ST77XX_BLACK);
  delay(250);

  if (! ina219.begin()) {
    drawText(2, 18, 1, "Failed to connect.", ST77XX_WHITE, ST77XX_BLACK);
    Serial.println("Error: failed to find INA219 current meter.");
    while (1) { delay(10); }
  }

  drawText(2, 18, 1, "Connected." ,ST77XX_WHITE, ST77XX_BLACK);
  delay(250);

  drawText(2, 31, 1, "Connecting serial..." ,ST77XX_WHITE, ST77XX_BLACK);
  while (!Serial) {
    delay(10);
  }

  delay(250);
  drawText(2, 44, 1, "Connected." ,ST77XX_WHITE, ST77XX_BLACK);
  delay(250);
  drawText(2, 57, 1, "Firmware version: 1.02" ,ST77XX_WHITE, ST77XX_BLACK);
  delay(500);
  tft.fillScreen(ST77XX_BLACK);

  drawText(52, 10, 3, "OPV", ST77XX_WHITE, ST77XX_BLACK);
  drawText(22, 40, 3, "Tester", ST77XX_WHITE, ST77XX_BLACK);
  drawText(40, 90, 2, "by ANFF", ST77XX_WHITE, ST77XX_BLACK);
  delay(3000);
  tft.fillScreen(ST77XX_BLACK);
  drawText(2, 74, 3, "Kardinia", ST77XX_GREEN, ST77XX_BLACK);
  drawText(2, 102, 3, "Energy", ST77XX_GREEN, ST77XX_BLACK);
}


void loop(void) 
{
  if((millis() - time_now) > update_period) {
    time_now = millis();
    
    voltage_V = analogRead(voltage_divider_pin) / (10.24 * 1.15);
    dtostrf(voltage_V, 5, 1, voltage);

    current_mA = ina219.getCurrent_mA();
    if (current_mA <= 0) {
      current_mA = 0;
    }
    else if (current_mA > 3000) {
      current_mA = 3000;
    }
    dtostrf(current_mA, 5, 0, current);
    
    power_W = voltage_V * current_mA / 1000;
    dtostrf(power_W, 5, 2, power);

    energy_Wh_d = power_W * energy_scaling_factor;
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


void drawText(uint8_t x_pos, uint8_t y_pos, uint8_t text_size, String text_literal, uint16_t text_colour, uint16_t text_background_colour) {
  tft.setCursor(x_pos, y_pos);
  tft.setTextColor(text_colour, text_background_colour);
  tft.setTextSize(text_size);
  tft.setTextWrap(false);
  tft.print(text_literal);
}
