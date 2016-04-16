#include "sensor.h"

SensorRaw::SensorRaw()
{
    this->XAis = 0;
    this->YAis = 0;
    this->ZAis = 0;
}
void SensorRaw::operator=(const int & t)
{
    this->XAis = t;
    this->YAis = t;
    this->ZAis = t;
}
SensorScale SensorRaw::operator *(const float & t) const
{
    SensorScale scale;
    scale.XAis = this->XAis * t;
    scale.YAis = this->YAis * t;
    scale.ZAis = this->ZAis * t;
    return scale;
}
SensorRaw SensorRaw::operator-(const SensorRaw & t) const
{
    SensorRaw raw;
    raw.XAis = this->XAis - t.XAis;
    raw.YAis = this->YAis - t.YAis;
    raw.ZAis = this->ZAis - t.ZAis;
    return raw;
}

SensorScale::SensorScale()
{
    this->XAis = 0;
    this->YAis = 0;
    this->ZAis = 0;
}
void SensorScale::operator=(const int & t)
{
    this->XAis = t;
    this->YAis = t;
    this->ZAis = t;
}
void SensorScale::operator+=(const SensorRaw & t)
{
    this->XAis = this->XAis + t.XAis;
    this->YAis = this->YAis + t.YAis;
    this->ZAis = this->ZAis + t.ZAis;
}
SensorRaw SensorScale::operator/(const int & t) const
{
    SensorRaw raw;
    raw.XAis = this->XAis / t;
    raw.YAis = this->YAis / t;
    raw.ZAis = this->ZAis / t;
    return raw;
}
