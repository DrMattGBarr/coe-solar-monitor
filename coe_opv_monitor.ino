/***************************************************
   ANFF OPV Solar Monitor
   Date: 22-01-2024
   Author: Matthew G Barr

 ****************************************************/
// Libraries
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_INA219.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

#define TFT_SCK  13
#define TFT_MISO 12
#define TFT_MOSI 11
#define TFT_CS   10
#define TFT_DC   8
#define TFT_RST  9

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
Adafruit_INA219 ina219(0x40);

uint8_t voltage_divider_pin = A0;
float voltage_V = 0; 
float current_mA = 0;
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

  tft.initR(INITR_BLACKTAB);      // Init ST7735S chip, black tab
  tft.fillScreen(ST77XX_BLACK);
  tft.setRotation(1);
  drawText(10, 50, 2, "OPV Testing", ST77XX_WHITE, ST77XX_BLACK);
  delay(1000);
  tft.fillScreen(ST77XX_BLACK);
}

void loop(void) 
{
  voltage_V = analogRead(voltage_divider_pin) / (10.24);
  current_mA = ina219.getCurrent_mA();
  if (current_mA <= 0) 
    {
    current_mA = 0;
    }

  power_W = voltage_V * current_mA / 1000;
  energy_Wh_d = power_W * 3600 * 9;

  Serial.print("Voltage: "); Serial.print(voltage_V, 1);  Serial.print(" V,   ");
  Serial.print("Current: "); Serial.print(current_mA, 0); Serial.print(" mA,   ");
  Serial.print("Power: "); Serial.print(power_W); Serial.print(" W,   ");
  Serial.print("Energy: "); Serial.print(energy_Wh_d, 0); Serial.println(" Whr/day");

  drawText(2, 5, 1, "Voltage:", ST77XX_WHITE, ST77XX_BLACK);
  String voltage_string = String(voltage_V, 0);
  drawText(52, 5, 1, voltage_string, ST77XX_WHITE, ST77XX_BLACK);
  drawText(86, 5, 1, "V", ST77XX_WHITE, ST77XX_BLACK);

  drawText(2, 18, 1, "Current:", ST77XX_WHITE, ST77XX_BLACK);
  String current_string = String(current_mA, 0);
  drawText(52, 18, 1, current_string, ST77XX_WHITE, ST77XX_BLACK);
  drawText(86, 18, 1, "mA", ST77XX_WHITE, ST77XX_BLACK);

  delay(1000);
}

void testdrawtext(char *text, uint16_t color) {
  tft.setCursor(0, 0);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.print(text);
}

void drawText(int x_pos, int y_pos, int text_size, String text_literal, uint16_t text_colour, uint16_t text_background_colour) {
  tft.setCursor(x_pos, y_pos);
  tft.setTextColor(text_colour, text_background_colour);
  tft.setTextSize(text_size);
  tft.setTextWrap(false);
  tft.print(text_literal);
}