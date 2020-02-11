#include <Arduino.h>
//#include <U8g2lib.h>
#include <SPI.h>
//#include "SolarBMS.h"
#include "WifiService.h"

#include <Adafruit_ADS1015.h>
#include <Wire.h>

#define RELAY_PIN 15

Adafruit_ADS1015 _ads;
//SolarBMS bms(RELAY_PIN, 4, 5, 0x48);

// double dCurrent_voltage = 0;
// double dCurrent_current = 0;
// boolean bPowerEnabled = false;

//U8G2_ST7920_128X64_1_SW_SPI u8g2(U8G2_R0, 12, 14, 16); //SCK = 12, MOSI = 14, CS = 16

void setup();
// void draw();
void loop();
// void drawLayout();
// void drawParams();
double readVoltage();
// double readCurrent();
// boolean isPowerEnabled();
// void cycleRelay();

void setup()
{

  pinMode(RELAY_PIN, OUTPUT);
  
  Serial.begin(115200);
  Serial.println("SolarBMS v0.3");

  Serial.println("Starting two wire");
  
   Wire.begin(4, 5); //SDA SCL
    _ads = Adafruit_ADS1115(0x48);
    _ads.begin();
  
  // Serial.println("Starting LCD");
  // u8g2.setFontRefHeightExtendedText();
  // u8g2.begin();
}

void loop()
{
  //bms.cycleRelay();
  

  Serial.print("Voltage: ");
  Serial.println(readVoltage());

  // Serial.print("Current: ");
  // Serial.println(bms.readCurrent(), 4);

  delay(5000);
}

double readVoltage()
{
  _ads.setGain(GAIN_ONE);
  return _ads.readADC_SingleEnded(2) * 0.000125 * 6; // 1/6 voltage divider
}

// double readCurrent()
// {
//   ads.setGain(GAIN_SIXTEEN); 
//   return ads.readADC_SingleEnded(1) * 0.0000078125 * 1333.33; 
// }

// void cycleRelay() {
// //enable relay
//   boolean bPowerEnabledOld = bPowerEnabled;
//   bPowerEnabled = isPowerEnabled();
//   if(bPowerEnabled != bPowerEnabledOld) {
//     Serial.print("External power: ");
//     Serial.println(bPowerEnabled);
//     digitalWrite(RELAY_PIN, bPowerEnabled);
//   }
// }

// boolean isPowerEnabled()
// {
//   if (bPowerEnabled)
//   {
//     return (readVoltage() > 11.5);
//   }
//   else
//   {
//     return (readVoltage() > 12.5);
//   }
// }

// void draw()
// {
//   u8g2.firstPage();
//   do {
//     drawLayout();
//     drawParams();
//   } while (u8g2.nextPage());
// }

// void drawLayout()
// {
//   u8g2.setFont(u8g_font_helvR08);
//   u8g2.setFontPosTop();
//   /*u8g.drawStr( 3, 50, "Outlets");
//   u8g.drawDisc(70, 55 , 3);
//   u8g.drawCircle(80, 55 , 3);
//   u8g.drawCircle(90, 55 , 3);
//   u8g.drawDisc(100, 55 , 3);
//   u8g.drawCircle(110, 55 , 3);
//   u8g.drawCircle(120, 55 , 3);
//   u8g.drawFrame(0, 48, 128, 16);*/

//   //first params
//   u8g2.drawStr(2, 2, "Volt:");
//   u8g2.drawStr(2, 12, "Amp: "); //add 10 px
//   u8g2.drawStr(2, 22, "Watt: ");

//   //amps params
//   u8g2.drawStr(65, 2, "Ah/1h:");
//   u8g2.drawStr(65, 12, "Ah/12h:"); //add 10 px
//   u8g2.drawStr(65, 22, "Ah/24h:");

//   u8g2.drawFrame(0, 0, 128, 34);

//   //charge & power params
//   u8g2.drawFrame(0, 35, 128, 14);
//   bPowerEnabled ? u8g2.drawStr(2, 37, "Pwr: online") : u8g2.drawStr(2, 37, "Pwr: offline") ;
//   u8g2.drawStr(65, 37, "Chrg: 83%");

//   //u8g2.drawDisc(30, 42, 4);

//   //charge bar
//   int bar_progress = current_voltage * 9.2;
//   u8g2.drawFrame(0, 50, 128, 14);        //charge frame
//   u8g2.drawBox(2, 52, bar_progress, 10); //charge bar (0 = empty, 126 = full)

//   u8g2.setFont(u8g_font_helvR08);
//   u8g2.setFontPosTop();
// }

// void drawParams()
// {
//   char buf[6];
//   char voltage[7];
//   dtostrf(current_voltage, 4, 2, buf);
//   snprintf(voltage, sizeof voltage, "%sV", buf);
//   u8g2.drawStr(25, 2, voltage);
// }
