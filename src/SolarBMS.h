#ifndef SolarBMS_h
#define SolarBMS_h

#include <Arduino.h>
#include <Adafruit_ADS1015.h>
#include <Wire.h>

class SolarBMS {
    public:
        SolarBMS();
        SolarBMS(int iRelayPin, int SDA, int SCL, int addr);
        int readVoltage();
        int readCurrent();
        void determineRelay(double voltage);
        double getLastVoltage();
        int getLastVoltageRaw();
        double getLastCurrent();
        int getLastCurrentRaw();
        bool getRelayOn();
        void intervalRead();
    
    private:
        int _iRelayPin;
        int _iCurrent;
        int _iVoltage;
        bool _bPowerEnabled;
        Adafruit_ADS1115 _ads;
};

#endif


