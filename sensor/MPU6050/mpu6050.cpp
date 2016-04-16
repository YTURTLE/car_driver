#include "mpu6050.h"

MPU6050::MPU6050()
{
#ifndef I2C_Init_H
#define I2C_Init_H
    I2C.i2c_init();
#endif // I2C_Init_H
    set_clock_source();
    set_acc_scale(2);
    set_gyr_scale(250);
    set_dlpf(44);
    mpu_cor_offset(); //加速度,陀螺仪零偏矫正

}

MPU6050::MPU6050(int acc_scale, int gyr_scale, int bandwidth)
{
#ifndef I2C_Init_H
#define I2C_Init_H
    I2C.i2c_init();
#endif // I2C_Init_H
    set_clock_source();
    set_acc_scale(acc_scale);
    set_gyr_scale(gyr_scale);
    set_dlpf(bandwidth);
    mpu_cor_offset(); //加速度,陀螺仪零偏矫正
}

void MPU6050::set_clock_source()
{
    I2C.i2c_write_byte(DEVRICE_ADDRESS, PWR_MGMT_1, uint8_t(0x01));;
}

//@set_dlpf
//@band_width 260Hz,184Hz,94Hz,44Hz,21Hz,10Hz,5Hz
void MPU6050::set_dlpf(int band_width)
{
    uint8_t mode;
    switch (band_width)
    {
    case 260:
        mode = 0x00;
        break;
    case 184:
        mode = 0x01;
        break;
    case 94:
        mode = 0x02;
        break;
    case 44:
        mode = 0x03;
        break;
    case 21:
        mode = 0x04;
        break;
    case 10:
        mode = 0x05;
        break;
    case 5:
        mode = 0x06;
        break;
    default:
        mode = 0x07; //保留
        break;
    }
    I2C.i2c_write_byte(DEVRICE_ADDRESS, CONFIG, mode);
}
//@set_gyr_scale
//@scale 250,500,1000,2000
void MPU6050::set_gyr_scale(int scale)
{
    uint8_t mode;
    switch(scale)
    {
    case 250:
        mode = 0x00;
        gyr_mScale = 7.63; //±250º/s,131 LSB/(º/s)
        break;
    case 500:
        mode = 0x01;
        gyr_mScale = 15.27; //±500º/s,65.5 LSB/(º/s)
        break;
    case 1000:
        mode = 0x02;
        gyr_mScale = 30.49; //±1000º/s,32.8 LSB/(º/s)
        break;
    case 2000:
        mode = 0x03;
        gyr_mScale = 60.98; //±2000º/s,16.4 LSB/(º/s)
        break;
    default:
        mode = 0x00;
        gyr_mScale = 7.63;
        break;
    }
    mode = mode << 3;
    I2C.i2c_write_byte(DEVRICE_ADDRESS, GYRO_CONFIG, mode);
}
//@set_acc_scale
//@scale 2,4,8,16
void MPU6050::set_acc_scale(int scale)
{
    uint8_t mode;
    switch(scale)
    {
    case 2: //±2g,16384 LSB/g
        mode = 0x00;
        acc_mScale = 0.06;
        ACC_1G = 16384;
        break;
    case 4: //±4g,8192 LSB/g
        mode = 0x01;
        acc_mScale = 0.12;
        ACC_1G = 8192;
        break;
    case 8: //±8g,4096 LSB/g
        mode = 0x02;
        acc_mScale = 0.24;
        ACC_1G = 4096;
        break;
    case 16: //±16g,2048 LSB/g
        mode = 0x03;
        acc_mScale = 0.49;
        ACC_1G = 2048;
        break;
    default:
        mode = 0x00;
        acc_mScale = 0.06;
        ACC_1G = 16384;
        break;
    }
    mode = mode << 3;
    I2C.i2c_write_byte(DEVRICE_ADDRESS, ACCEL_CONFIG, mode);
}
//读取加速度原始数据
AcceleraterRaw MPU6050::read_acc_raw()
{
    std::vector<uint8_t> buffer = I2C.i2c_read_bytes(DEVRICE_ADDRESS, ACCEL_XOUT_H, uint8_t(6));
    AcceleraterRaw raw;
    raw.XAis = int16_t(int16_t(int16_t(buffer[0]) << 8) | buffer[1]);
    raw.YAis = int16_t(int16_t(int16_t(buffer[2]) << 8) | buffer[3]);
    raw.ZAis = int16_t(int16_t(int16_t(buffer[4]) << 8) | buffer[5]);
    // std::cout << "raw.XAis:" << raw.XAis << std::endl
    //           << "raw.YAis:" << raw.YAis << std::endl
    //           << "raw.ZAis:" << raw.ZAis << std::endl;
    return raw;
}
AcceleraterRaw MPU6050::read_acc_raw_offset()
{
    SensorRaw raw;
    raw = read_acc_raw();
    //std::cout << "@read_acc_raw_offset\n";
    //std::cout << "raw.XAis:" << raw.XAis << std::endl
    //          << "raw.YAis:" << raw.YAis << std::endl
    //          << "raw.ZAis:" << raw.ZAis << std::endl;
    //std::cout << "@read_acc_raw_offset end\n";
    raw = raw - acc_offset;
    return raw;
}

//读取加速度量化后的数据
AcceleraterScale MPU6050::read_acc_scale()
{
    AcceleraterRaw raw = read_acc_raw_offset();
    return (raw * acc_mScale);//单位g
}
//读取陀螺仪原始数据
GyroscopeRaw MPU6050::read_gyr_raw()
{
    std::vector<uint8_t> buffer = I2C.i2c_read_bytes(DEVRICE_ADDRESS, GYRO_XOUT_H, uint8_t(6));
    GyroscopeRaw raw;
    raw.XAis = int16_t(int16_t(int16_t(buffer[0]) << 8) | buffer[1]);
    raw.YAis = int16_t(int16_t(int16_t(buffer[2]) << 8) | buffer[3]);
    raw.ZAis = int16_t(int16_t(int16_t(buffer[4]) << 8) | buffer[5]);
    return raw;
}
GyroscopeRaw MPU6050::read_gyr_raw_offset()
{
    GyroscopeRaw raw;
    raw = read_gyr_raw();
    raw = raw - gyr_offset;
    return raw;
}

//读取陀螺仪量化后的数据
GyroscopeScale MPU6050::read_gyr_scale()
{
    GyroscopeRaw raw = read_gyr_raw_offset();
    return (raw * gyr_mScale); //单位dps
}
void MPU6050::acc_cor_offset(const int & callbrating_acc_cycles)
{
    static int cnt_a = 0;
    static AcceleraterScale temp_scale;
    AcceleraterRaw raw;
    if(cnt_a == 0)
    {
        temp_scale = 0;
        acc_offset = 0;
        cnt_a = 1;
        return;
    }
    raw = read_acc_raw();
    temp_scale += raw;
    if(cnt_a == callbrating_acc_cycles)
    {
        acc_offset = temp_scale / cnt_a;
        acc_offset.ZAis = acc_offset.ZAis - ACC_1G;
        cnt_a = 0;
        std::cout << "acc_offset.XAis:" << acc_offset.XAis << std::endl
                  << "acc_offset.YAis:" << acc_offset.YAis << std::endl
                  << "acc_offset.ZAis:" << acc_offset.ZAis << std::endl;
        return;
    }
    cnt_a ++;
}
void MPU6050::gyr_cor_offset(const int & callbrating_gyr_cycles)
{
    static int cnt_g = 0;
    static GyroscopeScale temp_scale;
    GyroscopeRaw raw;
    if(cnt_g==0)
    {
        temp_scale = 0;
        gyr_offset = 0;
        cnt_g = 1;
        return;
    }
    raw = read_gyr_raw();
    temp_scale += raw;
    if(cnt_g == callbrating_gyr_cycles)
    {
        gyr_offset = temp_scale / cnt_g;
        cnt_g = 0;
        std::cout << "gyr_offset.XAis:" << gyr_offset.XAis << std::endl
                  << "gyr_offset.YAis:" << gyr_offset.YAis << std::endl
                  << "gyr_offset.ZAis:" << gyr_offset.ZAis << std::endl;
        return;
    }
    cnt_g ++;
}
void MPU6050::mpu_cor_offset()
{
    const int cal_acc_cycles = 400, cal_gyr_cycles = 1000;

    for(int i=0;i<=cal_acc_cycles;i++)
    {
        acc_cor_offset(cal_acc_cycles);
        gyr_cor_offset(cal_gyr_cycles);
    }
    for(int n=0;n<=cal_gyr_cycles;n++)
    {
        gyr_cor_offset(cal_gyr_cycles);
    }
}
void setup(){}
void loop(){}
