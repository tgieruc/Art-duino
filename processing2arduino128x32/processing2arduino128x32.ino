// OLED
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define OLED_RESET 0 
Adafruit_SSD1306 display(OLED_RESET);


#define OLED_w 128
#define OLED_h 32


unsigned char bmp[OLED_w * OLED_h / 8];
uint16_t part = 0;


void setup()
{
  Serial.begin(2000000);
  Serial.println();

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
    display.display();
    
  delay(2000);
  display.clearDisplay();

}


void loop()
{
    if (part == OLED_w * OLED_h / 8){
      part = 0;
      display.clearDisplay();
      display.drawBitmap(0, 0, bmp, OLED_w, OLED_h, WHITE);
      display.display();
    }
    if (Serial.available()){
      bmp[part++] = Serial.read();
    }
}
