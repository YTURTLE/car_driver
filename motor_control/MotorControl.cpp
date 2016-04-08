#include "MotorControl.h"
MotorControl mc;

void MotorControl::serialInit()
{
    init();
    Serial.begin(9600);
}
int MotorControl::commans(char buff[])
{
    Serial.flush();
    int nwrite = 0;
    for(int i=0;i<5;i++)
    {
        nwrite = Serial.write(buff[i]);
        if(nwrite == 0)
        {
            std::cout << "TX Data False\n";
            return -1;
        }
        //delay(1);
    }
    return 0;
}
int MotorControl::motorGoStop()
{
    char buff[5] = {0xff, 0x00,0x00, 0x00, 0xff};
    if(commans(buff) == -1)
    {
        std::cout << "MotorGoStop False\n";
        return -1;
    }
    this->motionSign = 0;
    return 0;
}
int MotorControl::motorGoForward()
{
    char buff[5] = {0xff, 0x00, 0x01, 0x00, 0xff};
    if(commans(buff) == -1)
    {
        std::cout << "MotorGoForward False\n";
        return -1;
    }
    this->motionSign = 1;
    return 0;
}
int MotorControl::motorGoBack()
{
    char buff[5] = {0xff, 0x00, 0x02, 0x00, 0xff};
    if(commans(buff) == -1)
    {
        std::cout << "MotorGoBack False\n";
        return -1;
    }
    this->motionSign = 1;
    return 0;

}
int MotorControl::motorGoRight()
{
    char buff[5] = {0xff, 0x00, 0x04, 0x00, 0xff};
    if(commans(buff) == -1)
    {
        std::cout << "MotorGoRight False\n";
        return -1;
    }
    this->motionSign = 1;
    return 0;
}
int MotorControl::motorGoLeft()
{
    char buff[5] = {0xff, 0x00, 0x03, 0x00, 0xff};
    if(commans(buff) == -1)
    {
        std::cout << "MotorGoLeft False\n";
        return -1;
    }
    this->motionSign = 1;
    return 0;
}
int MotorControl::readServo1Angle()
{
    return this->angle1;
}
int MotorControl::readServo2Angle()
{
  return this->angle2;
}
int MotorControl::servo1Control(int angle)
{
    if(angle > 160)
    {
        std::cout << angle << "> 160 set servo1 angle false\n";
        return -1;
    }
   // Serial.flush();
    char buff[5] = {0xff, 0x01, 0x07, 0x00, 0xff};
    buff[3] = angle;
    if(commans(buff) == -1)
    {
        std::cout << "Servo1Control False\n";
        return -1;
    }
    this->angle1 = angle;
    return 0;
}
int MotorControl::servo2Control(int angle)
{
    if(angle > 90)
    {
        std::cout << angle << "> 90 set servo2 angle false\n";
        return -1;
    }
    char buff[5] = {0xff, 0x01, 0x07, 0x00, 0xff};
    buff[3] = angle;
    if(commans(buff) == -1)
    {
        std::cout << "Servo2Control False\n";
        return -1;
    }
    this->angle2 = angle;
    return 0;
}
