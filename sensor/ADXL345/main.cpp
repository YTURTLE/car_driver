#include <fstream>
#include <iostream>
#include <time.h>
#include "adxl345.h"
#include <MotorControl.h>
using namespace std;
int main(void){
    ofstream ax("adxl_ay.txt");
    ofstream kalman_ax("kalman_ay.txt");
    srand(unsigned(time(NULL)));
    mc.serialInit(); //pcduino串口初始化
    adx.init_adxl345();

    for(int i=0;i<=400;i++)
    {
        adx.get_acc();
        adx.acc_cor_offset();
    }

    float acc[3] = {0}, deltaV[8] = {0};
    float vel_x = 0, dis_x = 0,vel_sum_x = 0;
    int count = 0, n = 0;
    mc.motorGoForward();
    while (1) {
        count++;
        n++;
        if(count==500)
            break;

        adx.get_acc();
        acc[0] = adx.acc_y;
        /*acc[1] = adx.acc_y;
        acc[2] = adx.acc_z;*/
        ax << adx.acc_y << " ";



        adx.kalman_filter(&acc[0]);
        kalman_ax << acc[0] << " ";

        deltaV[n] = acc[0]*adx.ACC_COMPANY;
        vel_sum_x += deltaV[n];

        if(n == 7)
        {
            n = rand()%8;
            vel_x += vel_sum_x - 8*1e-3*deltaV[n]*deltaV[n];
            dis_x += vel_x;

            n = -1;
            vel_sum_x = 0;
        }
    }
    mc.motorGoStop();
    cout << "dis_y:" << dis_x << endl;
    return 0;

}
void setup(){}
void loop(){}
