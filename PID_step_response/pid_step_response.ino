// OLED
#include <Multi_BitBang.h>
#include <Multi_OLED.h>


#define OLED_w 128
#define OLED_h 64

#define NUM_DISPLAYS 2
#define NUM_BUSES 2
#define BASE_CONTRAST 1000
// I2C bus info
uint8_t scl_list[NUM_BUSES] = {D1, D1}; //{9,9,9,9};
uint8_t sda_list[NUM_BUSES] = {D2, D0}; //{5,6,7,8};
int32_t speed_list[NUM_BUSES] = {4000000L, 4000000L}; // OLED display info
uint8_t bus_list[NUM_DISPLAYS] = {0, 1}; // can be multiple displays per bus
uint8_t addr_list[NUM_DISPLAYS] = {0x3c, 0x3c};
uint8_t type_list[NUM_DISPLAYS] = {OLED_128x64, OLED_128x64};
uint8_t flip_list[NUM_DISPLAYS] = {0, 1};
uint8_t invert_list[NUM_DISPLAYS] = {0, 0};

unsigned char bmp[OLED_w * OLED_h / 8];

int Kp_delta = 0;
int Ti_delta = 0;
int Td_delta = 0;
int part = 0;
void setup()
{
  Serial.begin(500000);
  Serial.println();
  pinMode(D3, INPUT_PULLUP);
  pinMode(D4, INPUT_PULLUP);
  pinMode(D5, INPUT_PULLUP);
  pinMode(D6, INPUT_PULLUP);
  pinMode(D7, INPUT);
  pinMode(D8, INPUT);
  Multi_I2CInit(sda_list, scl_list, speed_list, NUM_BUSES);
  Multi_OLEDInit(bus_list, addr_list, type_list, flip_list, invert_list, NUM_DISPLAYS);
  for (uint8_t i = 0; i < NUM_DISPLAYS; i++)
  {
    Multi_OLEDFill(i, 0);
    Multi_OLEDSetContrast(i, BASE_CONTRAST);

  }

}
char flt[16] = {' '};
float result = 0;
void loop()
{

  get_sequ();
  Multi_OLEDWriteString(0, 0, 0, "", FONT_NORMAL, 0);

  Multi_OLEDLoadBMP(0, bmp);
  if (digitalRead(D5) == 0) {
    Kp_delta--;
  } else if (digitalRead(D6) == 0) {
    Kp_delta++;
  }
  Serial.write(Kp_delta);
  if (digitalRead(D3) == 0) {
    Ti_delta--;
  } else if (digitalRead(D4) == 0) {
    Ti_delta++;
  }
  Serial.write(Ti_delta);
  if (digitalRead(D7) == 1) {
    Td_delta--;
  } else if (digitalRead(D8) == 1) {
    Td_delta++;
  }
  Serial.write(Td_delta);
  result = 1.7433 + 0.5 * Kp_delta;
  String buf;
  buf += F("Kp: ");
  buf += String(result, 6);
  buf.toCharArray(flt, 16);
  Multi_OLEDWriteString(1, 0, 0, flt, FONT_NORMAL, 0);
  result = 0.1755 + 0.5 * Ti_delta;
  buf = "";
  buf += F("Ti: ");
  buf += String(result, 6);
  buf.toCharArray(flt, 16);
  Multi_OLEDWriteString(1, 0, 2, flt, FONT_NORMAL, 0);
  result = 0.0439 + 0.1 * Td_delta;
  buf = "";
  buf += F("Td: ");
  buf += String(result, 6);
  buf.toCharArray(flt, 16);
  Multi_OLEDWriteString(1, 0, 4, flt, FONT_NORMAL, 0);

}

void wait_for_serial() {
  while (Serial.available() <= 0) {

  }
}
unsigned char temp;

void get_sequ() {
  get_init_sequ();
  for (int i = 0; i < OLED_w * OLED_h / 8; ++i) {
    wait_for_serial();
    temp = Serial.read();
    bmp[i] = temp ;
  }
  //  int i = 0;
  //  while (i < OLED_w * OLED_h / 8) {
  //    if (Serial.available()) {
  //      bmp[i] = Serial.read();
  //      ++i;
  //    }
  //  }

}

void get_init_sequ() {
  uint8_t init_i = 0;
  while (init_i < 2) {
    wait_for_serial();
    if (Serial.read() == 69) {
      init_i++;
    }
  }

}
