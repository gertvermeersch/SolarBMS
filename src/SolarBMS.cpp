#include "SolarBMS.h"

SolarBMS::SolarBMS(int iRelayPin, int SDA, int SCL, int addr)
{
    _iRelayPin = iRelayPin;
    Wire.begin(SDA, SCL); //SDA SCL
    _ads = Adafruit_ADS1115(addr);
    _ads.begin();
     pinMode(iRelayPin, OUTPUT);
}

double SolarBMS::readVoltage()
{
    _ads.setGain(GAIN_ONE);
    uint16_t raw = _ads.readADC_SingleEnded(0);
    Serial.println(raw);
    return raw * 0.000125 * 6; // 1/6 voltage divider
}

double SolarBMS::readCurrent()
{
    _ads.setGain(GAIN_SIXTEEN);
    return _ads.readADC_SingleEnded(1) * 0.0000078125 * 1333.33;
}

bool SolarBMS::isPowerEnabled()
{
    if (_bPowerEnabled)
    {
        return (this->readVoltage() > 11.5);
    }
    else
    {
        return (this->readVoltage() > 12.5);
    }
}

void SolarBMS::cycleRelay()
{
    //enable relay
    bool bPowerEnabledOld = _bPowerEnabled;
    _bPowerEnabled = isPowerEnabled();
    if (_bPowerEnabled != bPowerEnabledOld)
    {
        Serial.print("External power: ");
        Serial.println(_bPowerEnabled);
        digitalWrite(_iRelayPin, _bPowerEnabled);
    }
}

double SolarBMS::getCurrentCurrent() {
    return _dCurrentVoltage;
}

double SolarBMS::getCurrentVoltage() {
    return _dCurrentVoltage;
}

