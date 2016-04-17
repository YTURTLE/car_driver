#include "hmc5883l.h"
#include <iostream>
using namespace std;
int main()
{
    HMC5883L hmc;
    float heading = 0;
    float headingDegrees = 0;
    while(1)
    {
        MagnetometerRaw raw = hmc.read_magnetometer_raw();
        MagnetometerScale scaled = hmc.read_magnetometer_scale();
        heading = atan2(scaled.YAis, scaled.XAis);
        heading += 50.03/1000;
        if(heading < 0)
            heading += 2*PI;
        if(heading > 2*PI)
            heading -= 2*PI;
        headingDegrees = heading * 180/M_PI;
        cout << "raw.XAis:" << raw.XAis << "\traw.YAxis:"
             << raw.YAis << "\traw.ZAxis:" << raw.ZAis << endl;
        cout << "scaled.XAxis:" << scaled.XAis << "\tscaled.YAxis:" << scaled.YAis
             << "\tscaled.ZAxis:" << scaled.ZAis << endl;
        cout << "headingDegrees:" << headingDegrees << endl;
    }
    //return 0;
}
void setup(){}
void loop(){}

