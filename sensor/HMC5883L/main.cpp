#include "hmc5883l.h"
#include <iostream>
using namespace std;
int main()
{
    float heading = 0;
    float headingDegrees = 0;
    while(1)
    {
        MagnetometerRaw raw = hmc.read_magnetometer_raw();
        MagnetometerScale scaled = hmc.read_magnetometer_scale();
        heading = atan2(scaled.YAxis, scaled.XAxis);
        heading += 50.03/1000;
        if(heading < 0)
            heading += 2*PI;
        if(heading > 2*PI)
            heading -= 2*PI;
        headingDegrees = heading * 180/M_PI;
        cout << "raw.XAxis:" << raw.XAxis << "\traw.YAxis:"
             << raw.YAxis << "\traw.ZAxis:" << raw.ZAxis << endl;
        cout << "scaled.XAxis:" << scaled.XAxis << "\tscaled.YAxis:" << scaled.YAxis
             << "\tscaled.ZAxis:" << scaled.ZAxis << endl;
        cout << "headingDegrees:" << headingDegrees << endl;
    }
    //return 0;
}
void setup(){}
void loop(){}
