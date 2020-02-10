#ifndef SolarBMS_h
#define SolarBMS_h

#include <Arduino.h>
#include <Adafruit_ADS1015.h>
#include <Wire.h>

class SolarBMS {
    public:
        SolarBMS(int iRelayPin, int SDA, int SCL, int addr);
        double readVoltage();
        double readCurrent();
        bool isPowerEnabled();
        void cycleRelay();
        double getCurrentVoltage();
        double getCurrentCurrent();
    private:
        int _iRelayPin;
        double _dCurrentVoltage;
        double _dCurrentCurrent;
        bool _bPowerEnabled;
        Adafruit_ADS1115 _ads;
};

#endif


