#ifndef SolarBMS_h
#define SolarBMS_h

#include <Arduino.h>
#include <Adafruit_ADS1015.h>
#include <Wire.h>

class SolarBMS {
    public:
        SolarBMS(int iRelayPin, int SDA, int SCL, int addr);
        int16_t readVoltage();
        int16_t readCurrent();
        void determineRelay(double voltage);
        double getLastVoltage();
        int16_t getLastVoltageRaw();
        double getLastCurrent();
        int16_t getLastCurrentRaw();
    
    private:
        int _iRelayPin;
        int16_t _iCurrent;
        int16_t _iVoltage;
        bool _bPowerEnabled;
        Adafruit_ADS1115 _ads;
};

#endif


