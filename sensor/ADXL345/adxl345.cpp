#include "adxl345.h"
ADXL345 adx;
ADXL345::ADXL345()
{

}

void ADXL345::init_adxl345(){
    const uint8_t POWER_CTL = 0x2D;      // 电源模式控制地址
    const uint8_t DATA_FORMAT = 0x31;    // 数据格式地址
    const uint8_t BW_RATE = 0x2C;        // 数据速率控制地址
    Wire.begin();
    write_byte(POWER_CTL, 0x00);   //进入待机模式
    write_byte(DATA_FORMAT, 0x08); //13位AD全分辨率模式,测量范围+-2g,比例因子4mg/LSB
    write_byte(BW_RATE, 0x0F);     //数据输出速率3200HZ,带宽1600HZ
    write_byte(POWER_CTL, 0x08);   //进入测量模式

}
void ADXL345::write_byte(uint8_t reg_address, uint8_t data){
    Wire.beginTransmission(DEVRICE_ADRESS);
    Wire.write(reg_address);
    Wire.write(data);
    Wire.endTransmission();
}
void ADXL345::get_acc(){
    int16_t acc[3] = {0};
    uint8_t buffer[6] = {0};
    uint8_t count = 0;
    Wire.beginTransmission(DEVRICE_ADRESS);
    Wire.write(uint8_t(0x32));
    Wire.endTransmission();
    Wire.beginTransmission(DEVRICE_ADRESS);
    Wire.requestFrom(DEVRICE_ADRESS, (uint8_t)6);
    for(;Wire.available();count++)
    {
        buffer[count] = Wire.read();
    }
    //std::cout << "count:" << int(count) << std::endl;
    Wire.endTransmission();
    acc[0] = (((int16_t)buffer[1]) << 8) + buffer[0];
    acc[1] = (((int16_t)buffer[3]) << 8) + buffer[2];
    acc[2] = (((int16_t)buffer[5]) << 8) + buffer[4];

    this->acc_x = int(acc[0]) - this->accOffset[0];
    this->acc_y = int(acc[1]) - this->accOffset[1];
    this->acc_z = int(acc[2]) - this->accOffset[2];
}
void ADXL345::kalman_filter(float *acc_x){
    static float predict_covariance = 10;
    static float gain = 0;
    static float quality_noise = 0.001;
    static float r_obs = 0.01;
    static float predict_value_x = 0;

    predict_covariance = predict_covariance + quality_noise; //更新当前预测协方差P(k,k-1)
    gain = predict_covariance / (predict_covariance + r_obs); //更新卡尔曼增益K(k)
    *acc_x = predict_value_x + gain * (this->acc_x-predict_value_x);            //更新估测新值
    predict_covariance = (1-gain)*predict_covariance*(1-gain) + gain*r_obs*gain;//更新预测协方差P(k)
    predict_value_x = *acc_x;


}
void ADXL345::acc_cor_offset(){
    static float tempAcc[3]={0,0,0};
    static int16_t cnt_a=0;
    static const int ACC_1G = 256;
    if(cnt_a==0)
    {
        this->accOffset[0] = 0;
        this->accOffset[1] = 0;
        this->accOffset[2] = 0;
        tempAcc[0] = 0;
        tempAcc[1] = 0;
        tempAcc[2] = 0;
        cnt_a = 1;
        //std::cout <<"hello\n";
        return;
    }
    tempAcc[0] += this->acc_x;
    tempAcc[1] += this->acc_y;
    tempAcc[2] += this->acc_z;
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
