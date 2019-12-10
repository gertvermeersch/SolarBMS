#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>



U8G2_ST7920_128X64_1_SW_SPI u8g2(U8G2_R0, 12, 11, 10); //SCK = 12, MOSI =11, CS 10

void setup();
void draw();
void loop();

void setup() {
  u8g2.setFontRefHeightExtendedText();
  u8g2.begin();


  //draw
  u8g2.firstPage();
  do {
    draw();
  } while ( u8g2.nextPage() );
}

void loop() {
  
}

void draw() {
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
  u8g2.drawStr(2, 2, "Volt: 12.5V");
  u8g2.drawStr(2, 12, "Amp: 1.25A"); //add 10 px
  u8g2.drawStr(2, 22, "Watt: 15.6W");

  //amps params
  u8g2.drawStr(65, 2, "Ah/1h: 5");
  u8g2.drawStr(65, 12, "Ah/12h: 12"); //add 10 px
  u8g2.drawStr(65, 22, "Ah/24h: 14");

  u8g2.drawFrame(0, 0, 128, 34);

  //charge & power params
  u8g2.drawFrame(0, 35, 128, 14);
  u8g2.drawStr(2, 37, "Pwr: offline");
  u8g2.drawStr(65, 37, "Chrg: 83%");

  //u8g2.drawDisc(30, 42, 4);

  //charge bar
  u8g2.drawFrame(0, 50, 128, 14); //charge frame
  u8g2.drawBox(2, 52, 124, 10); //charge bar (0 = empty, 124 = full)
  

  

  u8g2.setFont(u8g_font_helvR08);
  u8g2.setFontPosTop();

 

 

}