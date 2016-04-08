#ifndef MPU6050_H
#define MPU6050_H
#include <iostream>
#include "/usr/include/pcduino/Arduino.h"
#include "/usr/include/pcduino/Wire.h"
#define Pi                                  3.141592653f
typedef short                               int16_t;
typedef unsigned char                       uint8_t;
#define CALIBRATING_ACC_CYCLES              400
#define CALIBRATING_GYRO_CYCLES             1000
#define ACC_1G                              16384                // 2g
#define ACCCOMPANY                          0.06f// (1/16384)*980   g/LSB 单位：cm/s^2 0.06mg/LSB
#define GYROCOMPANY                         0.007633587786259542f // (1/131) (º/s)/LSB
class MPU6050
{
private:
  static const uint8_t DEVADRESS = 0x68; //address pin low (GND), default for InvenSense evaluation board
  int16_t acc[3];
  int16_t gyr[3];
  int accOffset[3];
  int gyrOffset[3];
  float LPF_1st_coe;
  float LPF_2nd_coe_a1, LPF_2nd_coe_a2, LPF_2nd_coe_b0;
  //const static float Fcut = 50;
  static int writeBits(uint8_t regAddr,uint8_t bitStart, uint8_t length, uint8_t data);
  static void setClockSource();
  static void setFullScaleAccRange();
  static void setFullScaleGyroRange();
  static void setSleepEnabled();
  static void setDLPFEnabled();
public:
  float last_data[3], prve_data[3];
  void LPF_2nd(float data[]);
  void LPF_2nd_Factor_Cal(float deltaT, float Fcut);
  void LPF_1st_Factor_Cal(float deltaT, float Fcut);
  void LPF_1st(float data[]);
  void reset();
  void mpuInit();
  void getMotion6();
  void readAcc(int* acc_x, int* acc_y, int* acc_z);
  void readGyr(int* gyr_x, int* gyr_y, int* gyr_z);
  void accCorOffset();
  void gyrCorOffset();
};
extern MPU6050 mpu;
#endif // MPU6050_H
