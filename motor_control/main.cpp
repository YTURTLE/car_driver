//#include <iostream>
#include "MotorControl.h"
#include     <fcntl.h>
using namespace std;
int main(void)
{
   mc.serialInit();
   cout << "serialInit end\n";
    //serialEventRun();
   mc.motorGoForward();
   cout << "mc.motorGoF\n";
   delay(1000);
   //mc.motorGoLeft();
   //delay(1000);
   mc.motorGoStop();
   //cout << "mc.motorGoStop\n";

}
void setup(){}
void loop(){}
