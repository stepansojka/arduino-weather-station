#ifndef SDS_H
#define SDS_H

#include <Arduino.h>

class Sds
{
public:

  enum Status
  {
    OK = 0,
    CHECKSUM_VERIFICATION_FAILED,
    MESSAGE_HEADER_NOT_FOUND,
    MESSAGE_FOOTER_NOT_FOUND
  };

  static const unsigned long SDS_021_BAUD = 9600;

  Sds(HardwareSerial& serial);

  void begin(unsigned long baud = SDS_021_BAUD);

  Status read(float& pm2_5, float& pm10);


private:
  struct __attribute__ ((packed)) SdsMessage
  {
    uint8_t command;
    uint16_t pm2_5;
    uint16_t pm10;
    uint16_t sensor_id;
    uint8_t checksum;
  };

  static const size_t MESSAGE_SIZE = sizeof(SdsMessage);

  HardwareSerial& m_serial;

  uint8_t m_buffer[MESSAGE_SIZE];

  bool seek_message_header();

  void read_message();

  uint8_t compute_checksum() const;
};


#endif
