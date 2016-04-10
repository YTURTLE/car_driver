#include "hmc5883l.h"

HMC5883L hmc();

HMC5883L::HMC5883L()
{
    Wire.begin();
    set_scale(uint8_t(0x01)); //配置量程：±1.3Ga
    set_measurement_mode(); //配置测量模式:连续测量
}
//函数名称：write_byte
//输入参数：uint8_t reg_address, uint8_t data
//参数说明：reg_address,寄存器地址. data,写进寄存器的值
//功能说明：在指定的寄存器地址(reg_address)写进一个字节数据(data)
void HMC5883L::write_byte(uint8_t reg_address, uint8_t data)
{
    Wire.beginTransmission(DEVRICE_ADDRESS);
    Wire.write(reg_address);
    Wire.write(data);
    Wire.endTransmission();
}
//函数名称：read_bytes
//输入参数：uint8_t reg_address, uint8_t length
//参数说明：reg_address,寄存器地址. length,读取数据的长度
//功能说明：在指定的寄存器地址(reg_address)读取数据长度为length个字节
uint8_t* HMC5883L::read_bytes(uint8_t reg_address, uint8_t length)
{
    Wire.beginTransmission(DEVRICE_ADDRESS);
    Wire.write(reg_address);
    Wire.endTransmission();
    uint8_t buffer[length];
    int count = 0;
    Wire.beginTransmission(DEVRICE_ADDRESS);
    Wire.requestFrom(DEVRICE_ADDRESS, length);
    for(;Wire.available();count++)
    {
        buffer[count] = Wire.read();
    }
    Wire.endTransmission();
    return buffer;
}
//函数名称：set_scale
//输入参数：uint8_t scale
//功能说明：配置HCM5883L的量程范围，scale取值:0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07
//        分别对应量程范围:±0.88Ga(1370),±1.3Ga(1090),±1.9Ga(820),±2.5Ga(660),±4.0Ga(440),±4.7Ga(390),±5.6Ga(330),
//        ±8.1Ga(230).括号后面为对应量程的分辨率
void HMC5883L::set_scale(uint8_t scale)
{
    switch (scale) {
        case 0x00:
            mScale = 0.73;
            break;
        case 0x01:
            mScale = 0.92;
            break;
        case 0x02:
            mScale = 1.22;
            break;
        case 0x03:
            mScale = 1.52;
            break;
        case 0x04:
            mScale = 2.27;
            break;
        case 0x05:
            mScale = 2.56;
            break;
        case 0x06:
            mScale = 3.03;
            break;
        case 0x07:
            mScale = 4.35;
            break;
        default:
            scale = 0x01;
            mScale = 0.92;
            break;
    }
    scale = scale << 5;
    write_byte(CRB_ADDRESS, scale);
}
//函数名称：set_measurment_mode
//功能说明：配置HMC5883L测量模式为连续测量模式
void HMC5883L::set_measurement_mode()
{
    write_byte(MR_ADDRESS, 0x00);
}
//函数名称: read_magnetometer_raw
//功能说明：读取HCM5883L各轴原始数据,返回结构体MagnetometerRaw变量
MagnetometerRaw HMC5883L::read_magnetometer_raw()
{
    uint8_t* buffer = read_bytes(XMR_ADDRESS, uint8_t(6));
    MagnetometerRaw raw;
    raw.XAxis = int16_t(int16_t(int16_t(buffer[0]) << 8) | buffer[1]);
    raw.ZAxis = int16_t(int16_t(int16_t(buffer[2]) << 8) | buffer[3]);
    raw.YAxis = int16_t(int16_t(int16_t(buffer[4]) << 8) | buffer[5]);
    return raw;
}
//函数名称：read_magnetometer_scale
//功能说明：读取HCM5883L各轴磁感应强度,返回结构体MagnetometerScale变量
MagnetometerScale HMC5883L::read_magnetometer_scale()
{
    MagnetometerRaw raw = read_magnetometer_raw();
    MagnetometerScale scaled;
    scaled.XAxis = raw.XAxis * mScale;
    scaled.ZAxis = raw.ZAxis * mScale;
    scaled.YAxis = raw.YAxis * mScale;
    return scaled;
}
