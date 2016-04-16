#include "mpu6050.h"
using namespace std;
int main()
{
    MPU6050 mpu;
    AcceleraterRaw raw = mpu.read_acc_raw_offset();
    cout << "raw.XAis:" << raw.XAis << endl
         << "raw.YAis:" << raw.YAis << endl
         << "raw.ZAis:" << raw.ZAis << endl;
    return 0;
}
