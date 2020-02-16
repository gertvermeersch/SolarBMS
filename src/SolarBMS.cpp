#include "SolarBMS.h"

SolarBMS::SolarBMS() {

}

SolarBMS::SolarBMS(int iRelayPin, int SDA, int SCL, int addr)
{
    _iRelayPin = iRelayPin;
    Wire.begin(SDA, SCL); //SDA SCL
    _ads = Adafruit_ADS1115(addr);
    _ads.begin();
    pinMode(iRelayPin, OUTPUT);
    int16_t adc0, adc1, adc2, adc3;

    adc0 = _ads.readADC_SingleEnded(0);
    adc1 = _ads.readADC_SingleEnded(1);
    adc2 = _ads.readADC_SingleEnded(2);
    adc3 = _ads.readADC_SingleEnded(3);
    Serial.print("AIN0: "); Serial.println(adc0);
    Serial.print("AIN1: "); Serial.println(adc1);
    Serial.print("AIN2: "); Serial.println(adc2);
    Serial.print("AIN3: "); Serial.println(adc3);
    Serial.println(" ");
}

uint16_t SolarBMS::readVoltage()
{
    _ads.setGain(GAIN_ONE);
    delay(10);
    _iVoltage = _ads.readADC_SingleEnded(2);
    Serial.println(_iVoltage);
    delay(10);
    return _iVoltage;
}

double SolarBMS::getLastVoltage() {
    return _iVoltage * 0.000125 * 6.15; // 1/6 voltage divider
}

uint16_t SolarBMS::readCurrent()
{
    // ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
    // ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
    // ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
    // ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
    // ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
    // ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV
    _ads.setGain(GAIN_SIXTEEN);
    delay(10);
    _iCurrent = _ads.readADC_SingleEnded(3); 
    Serial.println(_iCurrent);
    delay(10);
    return _iCurrent;
}

double SolarBMS::getLastCurrent() {
    return _iCurrent * 0.0000078125 * 1333.33;
}

uint16_t SolarBMS::getLastVoltageRaw() {
    return _iVoltage;
}

uint16_t SolarBMS::getLastCurrentRaw() {
    return _iCurrent;
}

void SolarBMS::determineRelay(double voltage)
{
    //enable relay
    bool bPowerEnabledOld = _bPowerEnabled;
    if (_bPowerEnabled)
    {
        _bPowerEnabled = (voltage > 11.5);
    }
    else
    {
        _bPowerEnabled = (voltage > 12.5);
    }

    if (_bPowerEnabled != bPowerEnabledOld)
    {
        Serial.print("External power: ");
        Serial.println(_bPowerEnabled);
        digitalWrite(_iRelayPin, _bPowerEnabled);
    }
}
