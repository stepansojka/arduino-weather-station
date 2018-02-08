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

unsigned long time = 0;
unsigned long current_cycle_start = 0;

const unsigned long DELAY_MILLIS = 5000;


void display_temperature_humidity(float t, float h)
{
  lcd.setCursor(0, 0);
  lcd.print("temp:    " + String(t) + "\xDF""C");

  lcd.setCursor(0, 1);
  lcd.print("humidity:" + String(h) + "%");
}

void display_heat_index_humidity(float i, float h)
{
  lcd.setCursor(0, 0);
  lcd.print("heat idx:" + String(i) + "\xDF""C");

  lcd.setCursor(0, 1);
  lcd.print("humidity:" + String(h) + "%");
}

void display_air_quality(float pm2_5, float pm10)
{
  lcd.setCursor(0, 0);
  lcd.print("pm2.5:" + String(pm2_5) + "ugm-3"); 
  lcd.setCursor(0, 1);
  lcd.print("pm10: " + String(pm10) + "ugm-3");
}

void loop()
{
  auto now = millis();
  auto time = millis() - current_cycle_start;

  sds.read();

  float t = dht.readTemperature();
  float h = dht.readHumidity();
  float hi = dht.computeHeatIndex(t, h, false);

  float pm2_5 = .0;
  float pm10 = .0;
  sds.get(pm2_5, pm10);

  if (time < 1*DELAY_MILLIS)
    display_temperature_humidity(t, h);
  else if (time < 2*DELAY_MILLIS)
    display_heat_index_humidity(hi, h);
  else if (time < 3*DELAY_MILLIS)
    display_air_quality(pm2_5, pm10);
  else
    current_cycle_start = now;
}

