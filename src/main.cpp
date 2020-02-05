#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>
#include <Adafruit_ADS1015.h>
#include <Wire.h>

#define VOLTAGE_PIN A0
#define RELAY_PIN 2

Adafruit_ADS1115 ads = Adafruit_ADS1115(0x48);

double current_voltage = 12.1;
boolean bPowerEnabled = false;

U8G2_ST7920_128X64_1_SW_SPI u8g2(U8G2_R0, 12, 11, 10); //SCK = 12, MOSI =11, CS 10

void setup();
void draw();
void loop();
void drawLayout();
void drawParams();
double readVoltage();
boolean isPowerEnabled();

void setup()
{
  pinMode(VOLTAGE_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  u8g2.setFontRefHeightExtendedText();
  u8g2.begin();
  Serial.begin(9600);
  Serial.println("SolarBMS v0.1");

  Wire.begin(1, 2); //SDA SCL
  ads.setGain(GAIN_SIXTEEN);
  ads.begin();}

void loop()
{
  //voltage
  current_voltage = readVoltage();
  //enable relay
  bPowerEnabled = isPowerEnabled();
  digitalWrite(RELAY_PIN, bPowerEnabled);

  //draw
  u8g2.firstPage();
  do {
    draw();
  } while (u8g2.nextPage());
  delay(1000);

  //read voltage

  int16_t adc0, adc1, adc2, adc3;

  adc0 = ads.readADC_SingleEnded(0);
  adc1 = ads.readADC_SingleEnded(1);
  adc2 = ads.readADC_SingleEnded(2);
  adc3 = ads.readADC_SingleEnded(3);
  Serial.print("AIN0: "); Serial.println(adc0);
  Serial.print("AIN1: "); Serial.println(adc1);
  Serial.print("AIN2: "); Serial.println(adc2);
  Serial.print("AIN3: "); Serial.println(adc3);
  Serial.println(" ");
}

void draw()
{
  drawLayout();
  drawParams();
}

void drawLayout()
{
  u8g2.setFont(u8g_font_helvR08);
  u8g2.setFontPosTop();
  /*u8g.drawStr( 3, 50, "Outlets");
  u8g.drawDisc(70, 55 , 3);
  u8g.drawCircle(80, 55 , 3);
  u8g.drawCircle(90, 55 , 3);
  u8g.drawDisc(100, 55 , 3);
  u8g.drawCircle(110, 55 , 3);
  u8g.drawCircle(120, 55 , 3);
  u8g.drawFrame(0, 48, 128, 16);*/

  //first params
  u8g2.drawStr(2, 2, "Volt:");
  u8g2.drawStr(2, 12, "Amp: "); //add 10 px
  u8g2.drawStr(2, 22, "Watt: ");

  //amps params
  u8g2.drawStr(65, 2, "Ah/1h:");
  u8g2.drawStr(65, 12, "Ah/12h:"); //add 10 px
  u8g2.drawStr(65, 22, "Ah/24h:");

  u8g2.drawFrame(0, 0, 128, 34);

  //charge & power params
  u8g2.drawFrame(0, 35, 128, 14);
  bPowerEnabled ? u8g2.drawStr(2, 37, "Pwr: online") : u8g2.drawStr(2, 37, "Pwr: offline") ;
  u8g2.drawStr(65, 37, "Chrg: 83%");

  //u8g2.drawDisc(30, 42, 4);

  //charge bar
  int bar_progress = current_voltage * 9.2;
  u8g2.drawFrame(0, 50, 128, 14);        //charge frame
  u8g2.drawBox(2, 52, bar_progress, 10); //charge bar (0 = empty, 126 = full)

  u8g2.setFont(u8g_font_helvR08);
  u8g2.setFontPosTop();
}

void drawParams()
{
  char buf[6];
  char voltage[7];
  dtostrf(current_voltage, 4, 2, buf);
  snprintf(voltage, sizeof voltage, "%sV", buf);
  u8g2.drawStr(25, 2, voltage);
}

double readVoltage()
{
  return analogRead(VOLTAGE_PIN) * 0.0044692737430168 * 3;
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
