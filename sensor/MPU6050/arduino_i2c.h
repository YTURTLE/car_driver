#ifndef ARDUINO_I2C_H
#define ARDUINO_I2C_H

#include <iostream>
#include <vector>
#include "Arduino.h"
#include "Wire.h"


typedef unsigned char uint8_t;
class Arduino_i2c
{
public:
    void i2c_init()
    {
        Wire.begin();
    }

    void i2c_write_byte(uint8_t dev_address, uint8_t reg_address, uint8_t data)
    {
        Wire.beginTransmission(dev_address);
        Wire.write(reg_address);
        Wire.write(data);
        Wire.endTransmission();
    }

    std::vector<uint8_t> i2c_read_bytes(uint8_t dev_address, uint8_t reg_address, uint8_t length)
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
            //std::cout << int(buffer[count]) << std::endl;
        }
        Wire.endTransmission();

        return buffer;
    }
};

#endif // ARDUINO_I2C_H
