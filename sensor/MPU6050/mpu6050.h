#ifndef MPU6050_H
#define MPU6050_H

#include "sensor.h"
#include "arduino_i2c.h"


class MPU6050
{
public:
    MPU6050();
    AcceleraterRaw read_acc_raw_offset();
    AcceleraterScale read_acc_scale();
    GyroscopeRaw read_gyr_raw_offset();
    GyroscopeScale read_gyr_scale();

protected:
    void set_clock_source(); //配置时钟源
    void set_acc_scale(int scale); //配置加速度量程
    void set_gyr_scale(int scale); //配置陀螺仪量程
    void set_dlpf(int band_width); //配置数字低通滤波器
    void acc_cor_offset(const int & callbrating_acc_cycles);
    void gyr_cor_offset(const int & callbrating_gyr_cycles);
    void mpu_cor_offset();
    AcceleraterRaw read_acc_raw();
    GyroscopeRaw read_gyr_raw();

private:
    static const uint8_t DEVRICE_ADDRESS = 0x68; //MPU6050设备默认地址
    static const uint8_t PWR_MGMT_1 = 0x6B; //配置复位，温度传感器，低功耗模式，时钟源寄存器地址
    static const uint8_t CONFIG = 0x1A; //配置加速度，陀螺仪低通滤波器(DLPF)寄存器地址
    static const uint8_t GYRO_CONFIG = 0x1B; //配置陀螺仪量程寄存器地址
    static const uint8_t ACCEL_CONFIG = 0x1C; //配置加速度量程寄存器地址
    static const uint8_t ACCEL_XOUT_H = 0x3B; //0x3B->0x40,分别存放AXH(加速度),AXL,AYH,AYL.AZH,AZL
    static const uint8_t GYRO_XOUT_H = 0x43; //0x43->0x48, 分别存放GXH(陀螺仪),GXL,GYH,GYL,GZH,GZL
    int ACC_1G;
    AcceleraterRaw acc_offset;
    GyroscopeRaw gyr_offset;
    Arduino_i2c I2C;
    float acc_mScale; //加速度输出分辨率
    float gyr_mScale; //陀螺仪输出分辨率

};

#endif // MPU6050_H
