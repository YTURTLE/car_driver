/*#include "arduino_i2c.h"

void Arduino_i2c::i2c_init()
{
    Wire.begin();
}
void Arduino_i2c::i2c_write_byte(uint8_t dev_address, uint8_t reg_address, uint8_t data)
{
    Wire.beginTransmission(dev_address);
    Wire.write(reg_address);
    Wire.write(data);
    Wire.endTransmission();
}
std::vector<uint8_t> Arduino_i2c::i2c_read_bytes(uint8_t dev_address, uint8_t reg_address, uint8_t length)
{
    Wire.beginTransmission(dev_address);
    Wire.write(reg_address);
    Wire.endTransmission();

    std::vector<uint8_t> buffer(length);
    int count = 0;

    Wire.beginTransmission(dev_address);
    Wire.requestFrom(dev_address, length);
    for(;Wire.available();count++)
    {
        buffer[count] = Wire.read();
    }
    Wire.endTransmission();

    return buffer;
}
*/
