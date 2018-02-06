#include <Arduino.h>
#include <LiquidCrystal.h>
#include <DHT.h>

#include "Sds.h"

const uint8_t DHT_PIN = 2;
#define DHT_TYPE DHT22   // DHT 22  (AM2302), AM2321

DHT dht(DHT_PIN, DHT_TYPE);


const uint8_t LCD_RS_PIN = 12;
const uint8_t LCD_EN_PIN = 10;

const uint8_t LCD_D4_PIN = 9;
const uint8_t LCD_D5_PIN = 8;
const uint8_t LCD_D6_PIN = 7;
const uint8_t LCD_D7_PIN = 6;

LiquidCrystal lcd(LCD_RS_PIN, LCD_EN_PIN,
                  LCD_D4_PIN, LCD_D5_PIN,
                  LCD_D6_PIN, LCD_D7_PIN);

const uint8_t LCD_ROWS = 2;
const uint8_t LCD_COLS = 16;

const long SERIAL_BAUD = 9600;

Sds sds(Serial1);


void setup()
{
  lcd.begin(LCD_COLS, LCD_COLS);

  dht.begin();

  Serial.begin(SERIAL_BAUD);
  sds.begin();
}


void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  lcd.setCursor(0, 0);
  lcd.print("temp:    " + String(t) + "\xDF""C");

  lcd.setCursor(0, 1);
  lcd.print("humidity:" + String(h) + "%");

  delay(5000);

  float hif = dht.computeHeatIndex(t, h, false);
  lcd.setCursor(0, 1);
  lcd.print("apparent:" + String(hif) + "\xDF""C");

  delay(5000);

  float pm2_5 = .0;
  float pm10 = .0;

  Sds::Status status = sds.read(pm2_5, pm10);

  if (pm10 > 0.0 && pm2_5 > 0.0)
  {
    lcd.setCursor(0, 0);
    lcd.print("pm2.5:" + String(pm2_5) + "ugm-3"); 
    lcd.setCursor(0, 1);
    lcd.print("pm10: " + String(pm10) + "ugm-3");
    delay(5000);
  }
}

