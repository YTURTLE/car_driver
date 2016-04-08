#include <fstream>
#include <iostream>
#include <time.h>
#include "mpu6050.h"
//#include "MotorControl.h"
using namespace std;
int main(void){
    ofstream acc_x("ax.txt");
    ofstream acc_y("ay.txt");
    ofstream ax_2nd("ax_2nd.txt");
    ofstream ay_2nd("ay_2nd.txt");
    //mc.serialInit(); //pcduino串口初始化
    mpu.mpuInit(); //mpu6050初始化，加速度量程+-2g,陀螺仪量程+-250
    mpu.LPF_2nd_Factor_Cal(0.5*1e-3, 1500); //一阶低通滤波器参数计算，截止频率50hz，周期1ms
    cout << "Init end~\n";
    /*加速度陀螺仪数据零偏矫正*/
    for(int i=0;i<=CALIBRATING_ACC_CYCLES;i++)
    {
        mpu.getMotion6();
        mpu.accCorOffset();
        mpu.gyrCorOffset();
    }
    for(int n=CALIBRATING_ACC_CYCLES+1;n<=CALIBRATING_GYRO_CYCLES;n++)
    {
        mpu.getMotion6();
        mpu.gyrCorOffset();
    }
    cout << "Offset End\n";
    float acc[3] = {0};
    //double vel_x = 0, vel_y = 0, vel_pre_x = 0 , vel_pre_y = 0;
    double dis_x = 0, dis_y = 0;
    int ax,ay,az,count=0;
    //unsigned long last_time=0, deltaT=0;

    //mc.motorGoForward();//控制小车前进，并使运动标志（mc.motionSign）置为1
    while(1)
    {
        /*隔1ms采集传感器数据，并进行积分运算*/
        //if(deltaT >= 1)
       // {

            count++;

            if(count == 500)
                break;
            mpu.getMotion6(); //获取传感器数据
            mpu.readAcc(&ax, &ay, &az); //读取传感器数据
            acc[0] = ax;
            acc[1] = ay;
            acc[2] = az;

            acc_x << ax << " ";
            acc_y << ay << " ";

            mpu.LPF_2nd(acc);//加速度二阶低通滤波

            ax_2nd << acc[0] << " ";
            ay_2nd << acc[1] << " ";
            /*速度位移积分运算，采用梯形积分法*/

            /*

            if(mc.motionSign == 1)
            {

                vel_x += ((acc[0]*ACCCOMPANY) +
                        (((acc[0]-mpu.last_data[0])*ACCCOMPANY)/2))*0.001; //mpu.last_data为上次加速度采集数据
                vel_y += ((acc[1]*ACCCOMPANY)+
                        (((acc[1]-mpu.last_data[1])*ACCCOMPANY)/2))*0.001; //单位cm/s

                dis_x += (vel_x + ((vel_x - vel_pre_x)/2))*0.001;
                dis_y += (vel_y + ((vel_y - vel_pre_y)/2))*0.001;

                mpu.last_data[0] = acc[0];
                mpu.last_data[1] = acc[1];
                vel_pre_x = vel_x;
                vel_pre_y = vel_y;

            }
            else
                vel_pre_x = vel_x = 0;*/

        //}

       // last_time = millis(); //获取系统当前运行时间,单位ms
        //cout << last_time << endl;
       // deltaT += millis() - last_time;
        //cout << deltaT << endl;

    }
   // mc.motorGoStop();
    // cout << acc[2] << endl;
    cout << dis_x << endl;
    cout << dis_y << endl;
    return 0;
}
    void setup(){}
    void loop(){}
