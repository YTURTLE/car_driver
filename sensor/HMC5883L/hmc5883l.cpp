#include "hmc5883l.h"


HMC5883L::HMC5883L()
{
#ifndef I2C_Init_H
#define I2C_Init_H
    i2c.i2c_init();
#endif // I2C_Init_H
    set_scale(M_SCALE_1P3); //配置量程：±1.3Ga
    set_measurement_mode(); //配置测量模式:连续测量
}
HMC5883L::HMC5883L(M_SCALE scale)
{
#ifndef I2C_Init_H
#define I2C_Init_H
    i2c.i2c_init();
#endif // I2C_Init_H
    set_scale(scale);
    set_measurement_mode(); //配置测量模式:连续测量
}

//@set_scale
//@scale M_SCALE_0P88, M_SCALE_1P3, M_SCALE_1P9, M_SCALE_2P5, M_SCALE_4P0
//       M_SCALE_4P7,  M_SCALE_5P6, M_SCALE_8P1
void HMC5883L::set_scale(M_SCALE scale)
{
    uint8_t mode;
    switch (scale)
    {
    case M_SCALE_0P88:
        mode = 0x00;
        mScale = 0.73;
        break;
    case M_SCALE_1P3:
        mode = 0x01;
        mScale = 0.92;
        break;
    case M_SCALE_1P9:
        mode = 0x02;
        mScale = 1.22;
        break;
    case M_SCALE_2P5:
        mode = 0x03;
        mScale = 1.52;
        break;
    case M_SCALE_4P0:
        mode = 0x04;
        mScale = 2.27;
        break;
    case M_SCALE_4P7:
        mode = 0x05;
        mScale = 2.56;
        break;
    case M_SCALE_5P6:
        mode = 0x06;
        mScale = 3.03;
        break;
    case M_SCALE_8P1:
        mode = 0x07;
        mScale = 4.35;
        break;
    default:
        mode = 0x01;
        mScale = 0.92;
        break;
    }
    mode = mode << 5;
    i2c.i2c_write_byte(DEVRICE_ADDRESS, CRB_ADDRESS, mode);
}
//函数名称：set_measurment_mode
//功能说明：配置HMC5883L测量模式为连续测量模式
void HMC5883L::set_measurement_mode()
{
    i2c.i2c_write_byte(DEVRICE_ADDRESS, MR_ADDRESS, uint8_t(0x00));
}
//函数名称: read_magnetometer_raw
//功能说明：读取HCM5883L各轴原始数据,返回结构体MagnetometerRaw变量
MagnetometerRaw HMC5883L::read_magnetometer_raw()
{
    std::vector<uint8_t> buffer = i2c.i2c_read_bytes(DEVRICE_ADDRESS, XMR_ADDRESS, uint8_t(6));
    MagnetometerRaw raw;
    raw.XAis = int16_t(int16_t(int16_t(buffer[0]) << 8) | buffer[1]);
    raw.ZAis = int16_t(int16_t(int16_t(buffer[2]) << 8) | buffer[3]);
    raw.YAis = int16_t(int16_t(int16_t(buffer[4]) << 8) | buffer[5]);
    return raw;
}
//函数名称：read_magnetometer_scale
//功能说明：读取HCM5883L各轴磁感应强度,返回结构体MagnetometerScale变量
MagnetometerScale HMC5883L::read_magnetometer_scale()
{
    MagnetometerRaw raw = read_magnetometer_raw();
    //MagnetometerScale scaled;
    return (raw * mScale);
}
