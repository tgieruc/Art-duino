// Demo : https://www.instagram.com/p/CNxGrtdB6gC/
// OLED
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 0  // GPIO0
Adafruit_SSD1306 display(OLED_RESET);

#define DELTAD  0.4

#define OLED_w 128
#define OLED_h 64

float delta1 = 1;
float delta2 = 0;


void setup()
{
  Serial.begin(9600);
  Serial.println();
  pinMode(D5, INPUT);
  pinMode(D6, INPUT);
  pinMode(D7, INPUT);
  pinMode(D8, INPUT);
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 64x48)
 
  display.display();
  delay(2000);

  display.clearDisplay();

}

int time_c = 0;
void loop()
{
  if (digitalRead(D5)==1) {
    delta1 += DELTAD;
    display.clearDisplay();
  }
  if (digitalRead(D6)==1) {
    delta1 -= DELTAD;
    display.clearDisplay();
  }
  if (digitalRead(D7)==1) {
    delta2 += DELTAD;
    display.clearDisplay();
  } 
  if (digitalRead(D8)==1) {
    delta2 -= DELTAD;
    display.clearDisplay();
  }
  Serial.println(delta1);
  Serial.println(delta2);
  
  
  for (float i = 0 ; i < 4000; i+=3){
    display.drawPixel((int)(OLED_w/2+sin((i+4000*time_c)/100*delta1+delta2)*20),(int) (OLED_h/2+sin((i+4000*time_c)/100)*20),WHITE);
  }
  time_c++;
  display.display();
}
