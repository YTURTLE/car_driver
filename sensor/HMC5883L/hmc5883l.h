#ifndef HMC5883L_H
#define HMC5883L_H

#include "sensor.h"
#include "arduino_i2c.h"


class HMC5883L
{
public:
    enum M_SCALE
    {
        M_SCALE_0P88,//±0.88Ga(1370)
        M_SCALE_1P3, //±1.3Ga(1090)
        M_SCALE_1P9, //±1.9Ga(820)
        M_SCALE_2P5, //±2.5Ga(660
        M_SCALE_4P0, //±4.0Ga(440)
        M_SCALE_4P7, //±4.7Ga(390)
        M_SCALE_5P6, //,±5.6Ga(330)
        M_SCALE_8P1 //±8.1Ga(230)
    };

    HMC5883L();
    HMC5883L(M_SCALE scale);
    MagnetometerRaw read_magnetometer_raw(); //读取磁罗仪原始数据
    MagnetometerScale read_magnetometer_scale(); //读取磁感应强度
protected:
    void set_scale(M_SCALE scale); //配置设备的量程范围
    void set_measurement_mode(); //配置设备的测量模式

private:
    static const uint8_t DEVRICE_ADDRESS = 0x1E; //hmc5883l设备地址(0x3C>>1 == 0x1E)
    static const uint8_t CRB_ADDRESS = 0x01;     //CRB(Configuration Register B)寄存器地址,配置设备的量程(scale)范围
    static const uint8_t MR_ADDRESS = 0x02;      //MR(Mode Register)寄存器地址，配置设备的测量模式
    static const uint8_t XMR_ADDRESS = 0x03; //XMR(X MSB Register),从0x03->0x08,分别为X_MSB,X_LSB,Z_MSB,Z_LSB,Y_MSB,Y_LSB
    float mScale; //输出分辨率
    Arduino_i2c i2c;
};
#endif // HMC5883L_H
