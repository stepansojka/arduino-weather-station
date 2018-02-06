#include "Sds.h"

const uint8_t MESSAGE_HEADER = 0xAA;
const uint8_t MESSAGE_FOOTER = 0xAB;

Sds::Sds(HardwareSerial& serial):
  m_serial(serial)
{}

void Sds::begin(unsigned long baud)
{
  m_serial.begin(baud);
}

bool Sds::seek_message_header()
{
  static const unsigned MAX_BYTES_READ = 2*MESSAGE_SIZE;

  for (size_t bytes = 0; bytes < MAX_BYTES_READ; ++bytes) {
    if (m_serial.read() == MESSAGE_HEADER)
      return true;
  }

  return false;
}

void Sds::read_message()
{
  for (size_t i = 0; i < MESSAGE_SIZE; ++i)
    m_buffer[i] = m_serial.read();
}

Sds::Status Sds::read(float& pm2_5, float& pm10)
{
  if (!seek_message_header())
    return MESSAGE_HEADER_NOT_FOUND;

  read_message();

  if (m_serial.read() != MESSAGE_FOOTER)
    return MESSAGE_FOOTER_NOT_FOUND;

  SdsMessage* message = reinterpret_cast<SdsMessage*>(m_buffer);

  if (message->checksum != compute_checksum())
    return CHECKSUM_VERIFICATION_FAILED;

  pm2_5 = message->pm2_5/10.0;
  pm10 = message->pm10/10.0;
  return OK;
}

uint8_t Sds::compute_checksum() const
{
  unsigned checksum = 0;
  for (size_t i = 1; i < MESSAGE_SIZE - 1; ++i)
    checksum += m_buffer[i];

  return checksum & 0xFF;
}
