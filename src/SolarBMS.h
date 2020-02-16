#ifndef SolarBMS_h
#define SolarBMS_h

#include <Arduino.h>
#include <Adafruit_ADS1015.h>
#include <Wire.h>

class SolarBMS {
    public:
        SolarBMS();
        SolarBMS(int iRelayPin, int SDA, int SCL, int addr);
        uint16_t readVoltage();
        uint16_t readCurrent();
        void determineRelay(double voltage);
        double getLastVoltage();
        uint16_t getLastVoltageRaw();
        double getLastCurrent();
        uint16_t getLastCurrentRaw();
    
    private:
        int _iRelayPin;
        uint16_t _iCurrent;
        uint16_t _iVoltage;
        bool _bPowerEnabled;
        Adafruit_ADS1115 _ads;
};

#endif


