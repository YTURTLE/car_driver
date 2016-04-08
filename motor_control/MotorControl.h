#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H
#include <iostream>
#include "Arduino.h"
class MotorControl
{
private:
    int angle1; // 舵机1角度
    int angle2; // 舵机2角度
    static int commans(char buff[]);
public:
    int motionSign;
    void serialInit();            //串口初始化
    int motorGoForward();         //前进
    int motorGoBack();            //倒退
    int motorGoRight();           //右转
    int motorGoLeft();            //左转
    int motorGoStop();            //停止
    int servo1Control(int angle); //舵机1控制角度，angle < 160
    int servo2Control(int angle); //舵机2控制角度, angle < 90
    int readServo1Angle();        //读取舵机1角度
    int readServo2Angle();        //读取舵机2角度
};
extern MotorControl mc;
#endif // MOTORCONTROL_H
