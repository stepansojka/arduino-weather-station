#include "Sds.h"

/*
  The SDS sensor sends a packet with the measurement over
  over serial port once per second. The packet structure
  looks like this:

  /======================================================\
  | byte no. | name            | content                 |
  |======================================================|
  | 0        | message header  | 0xAA                    |
  |----------|-----------------|-------------------------|
  | 1        | command         | 0xC0                    |
  |----------|-----------------|-------------------------|
  | 2        | data[0]         | pm 2.5 low byte         |
  | 3        | data[1]         | pm 2.5 high byte        |
  |----------|-----------------|-------------------------|
  | 4        | data[2]         | pm 10 low byte          |
  | 5        | data[3]         | pm 10 high byte         |
  |----------|-----------------|-------------------------|
  | 6        | data[4]         | sensor id low byte      |
  | 7        | data[5]         | sensor id high byte     |
  |----------|-----------------|-------------------------|
  | 8        | checksum        | data[0] + ... + data[5] |
  |----------|-----------------|-------------------------|
  | 9        | message footer  | 0xAB                    |
  \------------------------------------------------------/

*/

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
      on_header(c);
    else if (m_position == MESSAGE_COMMAND_IDX)
      on_command(c);
    else if (m_position < MESSAGE_CHECKSUM_IDX)
      on_data(c);
    else if (m_position == MESSAGE_CHECKSUM_IDX)
      on_checksum(c);
    else if (m_position == MESSAGE_FOOTER_IDX)
      on_footer(c);
  }
}

void Sds::on_header(uint8_t byte)
{
  if (byte == MESSAGE_HEADER_BYTE)
    ++m_position;
}

void Sds::on_command(uint8_t byte)
{
  if (byte == MESSAGE_COMMAND_BYTE)
    ++m_position;
  else
    m_position = 0;
}

void Sds::on_data(uint8_t byte)
{
  m_buffer[m_position - 2] = byte;
  ++m_position;
}

void Sds::on_checksum(uint8_t byte)
{
  auto checksum = compute_checksum();
  if (checksum == byte)
    ++m_position;
  else
    m_position = 0;
}

void Sds::on_footer(uint8_t byte)
{
  if (byte == MESSAGE_FOOTER_BYTE)
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
