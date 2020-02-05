#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>
#include <Adafruit_ADS1015.h>
#include <Wire.h>

#define VOLTAGE_PIN A0
#define RELAY_PIN 2

Adafruit_ADS1115 ads = Adafruit_ADS1115(0x48);

double dCurrent_voltage = 0;
double dCurrent_current = 0;
boolean bPowerEnabled = false;

U8G2_ST7920_128X64_1_SW_SPI u8g2(U8G2_R0, 12, 14, 16); //SCK = 12, MOSI = 14, CS = 16

void setup();
void draw();
void loop();
// void drawLayout();
// void drawParams();
double readVoltage();
double readCurrent();
boolean isPowerEnabled();

void setup()
{
  pinMode(VOLTAGE_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  
  Serial.begin(115200);
  Serial.println("SolarBMS v0.2");

  Serial.println("Starting two wire");
  Wire.begin(4, 5); //SDA SCL

  // ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
  // ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  // ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
  // ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
  // ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
  // ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV
  ads.begin();

  // Serial.println("Starting LCD");
  // u8g2.setFontRefHeightExtendedText();
  // u8g2.begin();
}

void loop()
{
  //enable relay
  bPowerEnabled = isPowerEnabled();
  digitalWrite(RELAY_PIN, bPowerEnabled);

  Serial.print("Voltage: ");
  Serial.println(readVoltage());

  Serial.print("Current: ");
  Serial.println(readCurrent());

  delay(1000);
}

double readVoltage()
{
  ads.setGain(GAIN_ONE);
  return ads.readADC_SingleEnded(0) * 0.000125 * 6 + 0.2; // 1/6 voltage divider, 0.2 deviation
}

double readCurrent()
{
  ads.setGain(GAIN_SIXTEEN); //TODO: Convert to amps
  return ads.readADC_SingleEnded(1) * 0.0000078125; // 1/6 voltage divider, 0.2 deviation
}

boolean isPowerEnabled()
{
  if (bPowerEnabled)
  {
    return (readVoltage() > 11.5);
  }
  else
  {
    return (readVoltage() > 12.5);
  }
}

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
