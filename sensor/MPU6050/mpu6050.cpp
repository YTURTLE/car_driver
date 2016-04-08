#include "mpu6050.h"
MPU6050 mpu;
void MPU6050::mpuInit(){
    Wire.begin();
    //reset();
    setClockSource();
    setFullScaleAccRange(); // 2g
    setFullScaleGyroRange();// 250
    setSleepEnabled(); //false
    setDLPFEnabled();//44Hz
}
void MPU6050::reset(){
    uint8_t b;
    uint8_t count;
    Wire.beginTransmission(DEVADRESS);
    Wire.write((uint8_t)0x6B);
    Wire.endTransmission();
    Wire.beginTransmission(DEVADRESS);
    Wire.requestFrom(DEVADRESS, (uint8_t)1);
    for(;Wire.available();count++)
    {
        b = Wire.read();
    }
    Wire.endTransmission();
    uint8_t data = 1;
    uint8_t bitNum = 7;
    b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
    Wire.beginTransmission(DEVADRESS);
    Wire.write((uint8_t)0x6B);
    Wire.write(b);
    Wire.endTransmission();
}

int MPU6050::writeBits(uint8_t regAddr,uint8_t bitStart, uint8_t length, uint8_t data){
    uint8_t count = 0;
    Wire.beginTransmission(DEVADRESS);
    Wire.write(regAddr);
    Wire.endTransmission();
    Wire.beginTransmission(DEVADRESS);
    Wire.requestFrom(DEVADRESS, (uint8_t)1);
    uint8_t b;
    for(;Wire.available();count++){
        b = Wire.read();
    }
    Wire.endTransmission();
    uint8_t soure = data;
    if(count != 0){
        uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
        soure <<= (bitStart - length + 1); // shift data into correct position
        soure &= mask; // zero all non-important bits in data
        b &= ~(mask); // zero all important bits in existing byte
        b |= soure; // combine data with existing byte
        Wire.beginTransmission(DEVADRESS);
        Wire.write(regAddr);
        Wire.write(b);
        Wire.endTransmission();
        std::cout << "b:" << (int)b << std::endl;
    }else{
        return -1;
    }
    return 0;
}

void MPU6050::setDLPFEnabled(){
    int status;
    //std::cout << "setDLPFEnabled \n";
    status = writeBits(0x1A, 2, 3, 0x05);
    if(status == -1)
        std::cout << "setDLPFEnabled false\n";
}

void MPU6050::setClockSource(){
    int status;
    status = writeBits(0x6B, 2, 3, 0x01);
    if(status == -1)
        std::cout << "setClock false\n";
}
void MPU6050::setFullScaleAccRange(){
    int status;
    writeBits(0x1C, 4, 2, 0x00);
    if(status == -1)
        std::cout << "setAccRange false\n";
}
void MPU6050::setFullScaleGyroRange(){
    int status;
    writeBits(0x1B, 4, 2, 0x00);
    if(status == -1)
        std::cout << "setGyroRange false\n";
}
void MPU6050::setSleepEnabled(){
    int status;
    writeBits(0x6B, 6, 1, 0x00);
    if(status == -1)
        std::cout << "setSleepEnabled false\n";
}
void MPU6050::getMotion6(){
    uint8_t count = 0;
    uint8_t buffer[14];
    Wire.beginTransmission(DEVADRESS);
    Wire.write((uint8_t)0x3B);
    Wire.endTransmission();
    Wire.beginTransmission(DEVADRESS);
    Wire.requestFrom(DEVADRESS, (uint8_t)14);
    for(;Wire.available();count++){
        buffer[count] = Wire.read();
    }
    Wire.endTransmission();
    this->acc[0] = (((int16_t)buffer[0]) << 8) | buffer[1];
    this->acc[1] = (((int16_t)buffer[2]) << 8) | buffer[3];
    this->acc[2] = (((int16_t)buffer[4]) << 8) | buffer[5];
    this->gyr[0] = (((int16_t)buffer[8]) << 8) | buffer[9];
    this->gyr[1] = (((int16_t)buffer[10]) << 8) | buffer[11];
    this->gyr[2] = (((int16_t)buffer[12]) << 8) | buffer[13];
}
void MPU6050::readAcc(int *acc_x, int *acc_y, int *acc_z){
    *acc_x = this->acc[0] - this->accOffset[0];
    *acc_y = this->acc[1] - this->accOffset[1];
    *acc_z = this->acc[2] - this->accOffset[2];
}
void MPU6050::readGyr(int *gyr_x, int *gyr_y, int *gyr_z){
    *gyr_x = this->gyr[0] - this->gyrOffset[0];
    *gyr_y = this->gyr[1] - this->gyrOffset[1];
    *gyr_z = this->gyr[2] - this->gyrOffset[2];
}
void MPU6050::accCorOffset(){
    static float tempAcc[3]={0,0,0};
    static int16_t cnt_a=0;

    if(cnt_a==0)
    {
        this->accOffset[0] = 0;
        this->accOffset[1] = 0;
        this->accOffset[2] = 0;
        tempAcc[0] = 0;
        tempAcc[1] = 0;
        tempAcc[2] = 0;
        cnt_a = 1;
        std::cout <<"hello\n";
        return;
    }
    tempAcc[0] += this->acc[0];
    tempAcc[1] += this->acc[1];
    tempAcc[2] += this->acc[2];
    if(cnt_a == CALIBRATING_ACC_CYCLES)
    {
        this->accOffset[0] = tempAcc[0]/cnt_a;
        this->accOffset[1] = tempAcc[1]/cnt_a;
        this->accOffset[2] = tempAcc[2]/cnt_a - ACC_1G;
        std::cout << "accOffset.x:" << accOffset[0] << std::endl;
        std::cout << "accOffset.y:" << accOffset[1] << std::endl;
        std::cout << "accOffset.z:" << accOffset[2] << std::endl;
        cnt_a = 0;
        return;
    }
    cnt_a++;
}
void MPU6050::gyrCorOffset(){
    static float tempGyr[3]={0,0,0};
    static int16_t cnt_a=0;

    if(cnt_a==0)
    {
        this->gyrOffset[0] = 0;
        this->gyrOffset[1] = 0;
        this->gyrOffset[2] = 0;
        tempGyr[0] = 0;
        tempGyr[1] = 0;
        tempGyr[2] = 0;
        cnt_a = 1;
        return;
    }
    tempGyr[0] += this->gyr[0];
    tempGyr[1] += this->gyr[1];
    tempGyr[2] += this->gyr[2];
    if(cnt_a == CALIBRATING_GYRO_CYCLES)
    {
        this->gyrOffset[0] = tempGyr[0]/cnt_a;
        this->gyrOffset[1] = tempGyr[1]/cnt_a;
        this->gyrOffset[2] = tempGyr[2]/cnt_a;
        cnt_a = 0;
        std::cout << "gyrOffset.x:" << gyrOffset[0] << std::endl;
        std::cout << "gyrOffset.y:" << gyrOffset[1] << std::endl;
        std::cout << "gyrOffset.z:" << gyrOffset[2] << std::endl;
        return;
    }
    cnt_a++;
}
void MPU6050::LPF_1st_Factor_Cal(float deltaT, float Fcut){
    this->LPF_1st_coe = deltaT / (deltaT + 1 / (2 * Pi * Fcut));
}
void MPU6050::LPF_2nd_Factor_Cal(float deltaT, float Fcut)
{
    float a = 1 / (2 * Pi * Fcut * deltaT);
    this->LPF_2nd_coe_b0 = 1 / (a*a + 3*a + 1);
    this->LPF_2nd_coe_a1  = (2*a*a + 3*a) / (a*a + 3*a + 1);
    this->LPF_2nd_coe_a2  = (a*a) / (a*a + 3*a + 1);
    std::cout << "b0:" << LPF_2nd_coe_b0 << std::endl;
    std::cout << "a1:" << LPF_2nd_coe_a1 << std::endl;
    std::cout << "a2:" << LPF_2nd_coe_a2 << std::endl;
}
void MPU6050::LPF_1st(float data[]){
    data[0] = this->last_data[0]*(1-this->LPF_1st_coe) + data[0]*this->LPF_1st_coe;
    data[1] = this->last_data[1]*(1-this->LPF_1st_coe) + data[1]*this->LPF_1st_coe;
    data[2] = this->last_data[2]*(1-this->LPF_1st_coe) + data[2]*this->LPF_1st_coe;

    this->last_data[0] = data[0];
    this->last_data[1] = data[1];
    this->last_data[2] = data[2];

   // std::cout << "acc_x:" << data[0] << std::endl;
   // std::cout << "acc_y:" << data[1] << std::endl;
   // std::cout << "acc_z:" << data[2] << std::endl;
}
void MPU6050::LPF_2nd(float acc[])
{

    acc[0] = acc[0] * this->LPF_2nd_coe_b0 + this->last_data[0] * this->LPF_2nd_coe_a1 -
            this->prve_data[0] * this->LPF_2nd_coe_a2;
    acc[1] = acc[1] * this->LPF_2nd_coe_b0 + this->last_data[1] * this->LPF_2nd_coe_a1 -
            this->prve_data[1] * this->LPF_2nd_coe_a2;

    this->prve_data[0] = this->last_data[0];
    this->prve_data[1] = this->last_data[1];

    this->last_data[0] = acc[0];
    this->last_data[1] = acc[1];

}
