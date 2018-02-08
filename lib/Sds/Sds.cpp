#include "Sds.h"

const uint8_t MESSAGE_HEADER_BYTE  = 0xAA;
const uint8_t MESSAGE_COMMAND_BYTE = 0xC0;
const uint8_t MESSAGE_FOOTER_BYTE  = 0xAB;

const size_t MESSAGE_HEADER_IDX   = 0;
const size_t MESSAGE_COMMAND_IDX  = 1;
const size_t MESSAGE_CHECKSUM_IDX = 8;
const size_t MESSAGE_FOOTER_IDX   = 9;

Sds::Sds(HardwareSerial& serial):
  m_serial(serial),
  m_position(0),
  m_pm2_5(.0),
  m_pm10(.0)
{}

void Sds::begin(unsigned long baud)
{
  m_serial.begin(baud);
}

void Sds::read()
{
  while (m_serial.available() > 0)
  {
    auto c = m_serial.read();

    if (m_position == MESSAGE_HEADER_IDX)
    {
      on_header(c);
    }
    else if (m_position == MESSAGE_COMMAND_IDX)
    {
      on_command(c);
    }
    else if (m_position < MESSAGE_CHECKSUM_IDX)
    {
      on_data(c)
    }
    else if (m_position == MESSAGE_CHECKSUM_IDX)
    {
      on_checksum(c);
    }
    else if (m_position == MESSAGE_FOOTER_IDX)
    {
      on_footer(c);
    }
  }
}

void Sds::on_header(uint8_t byte)
{
  if (byte == MESSAGE_HEADER_BYTE)
    ++m_position;
}

void Sds::on_command(uint8_t byte)
{
  if (byte == MEASUREMENT_COMMAND_BYTE)
    ++m_position;
  else
    m_position = 0;
}

void Sds::on_data(uint8_t byte)
{
  m_buffer[m_position - 2] = c;
  ++m_position;
}

void Sds::on_checksum(uint8_t byte)
{
  auto checksum = compute_checksum();
  if (checksum == c)
    ++m_position;
  else
    m_position = 0;
}

void Sds::on_footer(uint8_t byte)
{
  if (c == MESSAGE_FOOTER_BYTE)
    new_measurement();

  m_position = 0;
}

uint8_t Sds::compute_checksum() const
{
  unsigned checksum = 0;
  for (size_t i = 0; i < MESSAGE_SIZE; ++i)
    checksum += m_buffer[i];

  return checksum & 0xFF;
}

void Sds::get(float& pm2_5, float& pm10) const
{
  pm2_5 = m_pm2_5;
  pm10  = m_pm10;
}

void Sds::new_measurement()
{
  m_pm2_5 = (m_buffer[1] << 8 | m_buffer[0]) / 10.0;
  m_pm10  = (m_buffer[3] << 8 | m_buffer[2]) / 10.0;
}
