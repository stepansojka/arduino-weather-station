#include <Arduino.h>
#include <LiquidCrystal.h>
#include <DHT.h>

#include <math.h>

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

const uint8_t LCD_COLS = 16;
const uint8_t LCD_ROWS = 2;

const long SERIAL_BAUD = 9600;

const unsigned DELAY_MILLIS = 3000;

unsigned long current_cycle_start = 0;

Sds sds(Serial1);

void setup()
{
  lcd.begin(LCD_COLS, LCD_ROWS);

  dht.begin();

  Serial.begin(SERIAL_BAUD);
  sds.begin();
}

void lcd_printf(size_t row, const char* format, ...)
{
  static const size_t LCD_BUFFER_SIZE = LCD_COLS + 1;
  char lcd_buffer[LCD_BUFFER_SIZE];

  va_list args;
  va_start(args, format);
  vsnprintf(lcd_buffer, LCD_BUFFER_SIZE, format, args);
  va_end(args);

  lcd.setCursor(0, row);
  lcd.print(lcd_buffer);
}

void display_temperature_humidity()
{
  auto t = dht.readTemperature();
  lcd_printf(0, "temp:     %4s\xDF""C", String(t, 1).c_str());

  auto h = dht.readHumidity();
  lcd_printf(1, "humidity: %4s%% ", String(h, 1).c_str());
}

void display_heat_index_humidity()
{
  auto t = dht.readTemperature();
  auto h = dht.readHumidity();
  auto hi = dht.computeHeatIndex(t, h, false);

  lcd_printf(0, "heat idx: %4s\xDF""C", String(hi, 1).c_str());
  lcd_printf(1, "humidity: %4s%% ", String(h, 1).c_str());
}

void display_air_quality()
{
  float pm2_5 = .0;
  float pm10 = .0;
  sds.get(pm2_5, pm10);

  lcd_printf(0, "pm2.5: %3d ugm-3", lroundf(pm2_5));
  lcd_printf(1, "pm10:  %3d ugm-3", lroundf(pm10));
}

void loop()
{
  auto now = millis();
  auto time = millis() - current_cycle_start;

  sds.read();

  if (time < 1*DELAY_MILLIS)
    display_temperature_humidity();
  else if (time < 2*DELAY_MILLIS)
    display_heat_index_humidity();
  else if (time < 3*DELAY_MILLIS)
    display_air_quality();
  else
    current_cycle_start = now;
}

