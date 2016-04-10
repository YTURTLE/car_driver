#ifndef HMC5883L_H
#define HMC5883L_H

#include "Arduino.h"
#include "Wire.h"
#include <iostream>

typedef unsigned char uint8_t;
typedef short int16_t;

struct MagnetometerRaw
{
    int XAxis;
    int YAxis;
    int ZAxis;
};

struct MagnetometerScale
{
    float XAxis;
    float YAxis;
    float ZAxis;
};

class HMC5883L
{
public:
    HMC5883L();
    MagnetometerRaw read_magnetometer_raw(); //读取磁罗仪原始数据
    MagnetometerScale read_magnetometer_scale(); //读取磁感应强度

protected:
    void write_byte(uint8_t reg_address, uint8_t data); //往寄存器地址处写进一个字节
    uint8_t* read_bytes(uint8_t reg_address, uint8_t length); //读取长度为length的字节数
    void set_scale(uint8_t scale); //配置设备的量程范围
    void set_measurement_mode(); //配置设备的测量模式

private:
    static const uint8_t DEVRICE_ADDRESS = 0x1E; //hmc5883l设备地址(0x3C>>1 == 0x1E)
    static const uint8_t CRB_ADDRESS = 0x01;     //CRB(Configuration Register B)寄存器地址,配置设备的量程(scale)范围
    static const uint8_t MR_ADDRESS = 0x02;      //MR(Mode Register)寄存器地址，配置设备的测量模式
    static const uint8_t XMR_ADDRESS = 0x03; //XMR(X MSB Register),从0x03->0x08,分别为X_MSB,X_LSB,Z_MSB,Z_LSB,Y_MSB,Y_LSB
    float mScale; //输出分辨率
};
extern HMC5883L hmc;
#endif // HMC5883L_H
