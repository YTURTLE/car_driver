#ifndef ADXL345_H
#define ADXL345_H
#include <iostream>
#include <Arduino.h>
#include <Wire.h>
typedef unsigned char uint8_t;
typedef short int16_t;

class ADXL345
{
private:
    static const uint8_t DEVRICE_ADRESS = 0x53; // 从机地址,SD0接地地址为0x53,接电源为0x3A
    static const int CALIBRATING_ACC_CYCLES = 400;
    int accOffset[3];
    static void write_byte(uint8_t reg_address, uint8_t data);

public:
    static const int ACC_COMPANY = 4;
    int acc_x, acc_y, acc_z;
    void init_adxl345();
    void get_acc();
    void kalman_filter(float* acc_x);
    void acc_cor_offset();
    ADXL345();
};
extern ADXL345 adx;
#endif // ADXL345_H
