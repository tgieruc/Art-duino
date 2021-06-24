// DEMO : https://www.instagram.com/p/CPGtv9Khelz/
// OLED
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 0  // GPIO0
Adafruit_SSD1306 display(OLED_RESET);

#define OLED_w 128
#define OLED_h 64
int cmode = 0;
int counter = 0;
float delta = 0;
float delta_t = 0.1;

void setup()
{
  Serial.begin(9600);
  Serial.println();
  pinMode(D5, INPUT);
  pinMode(D6, INPUT);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 64x48)
 
  display.display();
  delay(2000);
  display.clearDisplay();
}



void loop()
{
  display.clearDisplay();
  if (digitalRead(D5)==1) {
    cmode++;
    delta = 0;
    counter = 0;
  }if (digitalRead(D6)==1) {
    cmode--;
    delta = 0;    
    counter = 0;
  }

  delta += 0.005;
  for (float t = 0; t < 50; t += delta_t) {
     display.drawLine(function1(t), function2(t),function1(t+delta_t), function2(t+delta_t),WHITE);
  }

  display.display();
}

float function1(float t) {
  return OLED_w / 2 + OLED_h / 2 * sin(0.01*counter*t+t) * cos(delta * t);
}

float function2(float t) {
  return OLED_h / 2 + OLED_h / 2 * sin(0.04 * cmode * t);
}

