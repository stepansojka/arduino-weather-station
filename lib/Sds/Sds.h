#ifndef SDS_H
#define SDS_H

#include <Arduino.h>

class Sds
{
public:
  static const unsigned long SDS_021_BAUD = 9600;

  Sds(HardwareSerial& serial);

  void begin(unsigned long baud = SDS_021_BAUD);

  void read();

  void get(float& pm2_5, float& pm10) const;

private:
  static const size_t MESSAGE_SIZE = 6;
  uint8_t m_buffer[MESSAGE_SIZE];

  HardwareSerial& m_serial;

  size_t m_position;

  float m_pm2_5;

  float m_pm10;

  void on_header(uint8_t byte);

  void on_command(uint8_t byte);

  void on_data(uint8_t byte);

  void on_checksum(uint8_t byte);

  void on_footer(uint8_t byte);

  uint8_t compute_checksum() const;

  void new_measurement();
};

#endif
