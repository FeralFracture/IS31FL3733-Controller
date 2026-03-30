#include <Arduino.h>
#include "IS31FL3733_controller.h"

const int SDA_PIN = 21;
const int SCL_PIN = 23;
const int INTB_PIN = 21;
const int SDB_PIN = 22;
int global_brightness = 0;

const int light_delays[6] = {60, 200, 25, 5, 300, 20},
          red_pwm = 100,
          blue_pwm = 100,
          green_pwm = 100,
          purple_pwm[2] = {100, 100},
          yellow_pwm[2] = {100, 100},
          cyan_pwm[2] = {100, 100};

IS31FL3733_Controller *controller;

uint8_t rotR(uint8_t x, uint8_t n);
void debugGlobalBrightness();
void setAllPWM(uint8_t val, uint8_t module = 0);
void scanAllI2C();
void transition(bool on);
void debugRGBLED();
void setLEDPWM(uint8_t columns, uint8_t rows, uint8_t pwm, uint8_t colors, uint8_t module = 0x00);
void disgustingTestCode();

uint8_t data[] = {
    B10001110,
    B01011010,
    B00110110};

uint8_t red_pwm_lookup[] = {
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, //0
  0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F // 5
};
uint8_t green_pwm_lookup[] = {
  0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
  0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
};
uint8_t blue_pwm_lookup[] = {
  // 0x#0 - 0x#7 = rows 1-4,
  // 0x#8 - 0x#F = rows 5-8

  //0x(0/3/6/9)# = Red
  //0x(1/4/7/A)# = Green
  //0x(2/5/8/B)# = Blue 
};

void setup()
{
  Serial.begin(921600);
  Serial.println(" ");

  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.setClock(400000);

  scanAllI2C();
  controller = new IS31FL3733_Controller(SDB_PIN, false);
  delay(1000);
  controller->setGlobalBrightness(10, 0);
  // controller->powerAll(COLOR_R | COLOR_G | COLOR_B, true, 0);
  for (int i = 0; i < 8; i++)
  {
    controller->setLEDRowPower(i, COLOR_R, data[0] >> i | data[0] << 8 - i);
    controller->setLEDRowPower(i, COLOR_G, data[1] >> i | data[1] << 8 - i);
    controller->setLEDRowPower(i, COLOR_B, data[2] >> i | data[2] << 8 - i);
  }

  setAllPWM(1, 0);
  controller->setLEDPWM(0x27, 100, 0);
  controller->setLEDPWM(0x2F, 100, 0);
  delay(1500);

  // delay(light_delays[0]);

  // controller->setGlobalBrightness(25, 5);
  // controller->powerAll(COLOR_R | COLOR_B, true, 5);
  // setAllPWM(12, 5);
  // delay(light_delays[0]);

  // controller->setGlobalBrightness(25, 4);
  // controller->powerAll(COLOR_R, true, 4);
  // setAllPWM(12, 4);
  // delay(light_delays[0]);

  // controller->setGlobalBrightness(40, 8);
  // controller->powerAll(COLOR_R | COLOR_G, true, 8);
  // setAllPWM(12, 8);
  // delay(light_delays[0]);

  // controller->setGlobalBrightness(10, 12);
  // controller->powerAll(COLOR_G, true, 12);
  // setAllPWM(12, 12);
  // delay(light_delays[1]);
  // transition(false);
  //  { delay(light_delays[1]);
  // transition(true);
  // delay(light_delays[1]);
  // transition(false);
  // delay(100); }
  // setAllPWM(0, 0);
  // setAllPWM(0, 5);
  // setAllPWM(0, 4);
  // setAllPWM(0, 8);
  // setAllPWM(0, 12);
  // controller->powerAll(COLOR_R | COLOR_G | COLOR_B, true, 0);
  // controller->powerAll(COLOR_R | COLOR_G | COLOR_B, true, 5);
  // controller->powerAll(COLOR_R | COLOR_G | COLOR_B, true, 4);
  // controller->powerAll(COLOR_R | COLOR_G | COLOR_B, true, 8);
  // controller->powerAll(COLOR_R | COLOR_G | COLOR_B, true, 12);
  // controller->setGlobalBrightness(64, 0);
  // controller->setGlobalBrightness(64, 5);
  // controller->setGlobalBrightness(64, 4);
  // controller->setGlobalBrightness(64, 8);
  // controller->setGlobalBrightness(64, 12);

  // disgustingTestCode();
}

void loop()
{
for (int i = 0; i < 8; i++)
{
    for (int j = 0; j < 8; j++)
    {
        uint8_t r = rotR(data[0], j + i);
        uint8_t g = rotR(data[1], j + i);
        uint8_t b = rotR(data[2], j + i);

        controller->setLEDRowPower(j, COLOR_R, r);
        controller->setLEDRowPower(j, COLOR_G, g);
        controller->setLEDRowPower(j, COLOR_B, b);
    }
    delay(1000);
}
  // for (int i = 63; i >= 0; i--)
  // {
  //   controller->setGlobalBrightness(i, 0);
  //   controller->setGlobalBrightness(i, 5);
  //   controller->setGlobalBrightness(i, 4);
  //   controller->setGlobalBrightness(i, 8);
  //   controller->setGlobalBrightness(i, 12);
  //   delay(50);
  // }
  // delay(50);
  // for (int i = 1; i < 65; i++)
  // {
  //   controller->setGlobalBrightness(i, 0);
  //   controller->setGlobalBrightness(i, 5);
  //   controller->setGlobalBrightness(i, 4);
  //   controller->setGlobalBrightness(i, 8);
  //   controller->setGlobalBrightness(i, 12);
  //   delay(50);
  // }
}

uint8_t rotR(uint8_t x, uint8_t n) {
 n &= 7;
 if (n == 0) return x;
  return (x >> n) | (x << (8 - n));
}

void scanAllI2C()
{
  uint8_t devices[16];
  uint8_t count = scanI2C(devices, sizeof(devices));
  Serial.println("\nI2C Devices Found:");
  for (uint8_t i = 0; i < count; i++)
  {
    Serial.printf("- 0x%02X\n", devices[i]);
  }
}

void setAllPWM(uint8_t val, uint8_t module)
{
  for (uint8_t reg = 0x00; reg <= 0xBF; reg++)
  {
    controller->setLEDPWM(reg, val, module);
  }
}

void DebugRGBLED()
{
  for (int i = 0; i < 2; i++)
  {
    for (uint8_t reg_offset = 0xC0; reg_offset > 0x00; reg_offset -= 0x10)
    {
      for (uint8_t reg = 0x00; reg < 0x08; reg += 0x01)
      {

        controller->setLEDPWM(reg + reg_offset - 0x10 + (i == 0 ? 0x00 : 0x08), 40, 0);
        delay(light_delays[5]);
      }

      delay(light_delays[4]);

      for (uint8_t reg = 0x08; reg > 0x00; reg -= 0x01)
      {
        controller->setLEDPWM(reg + reg_offset - 0x11 + (i == 0 ? 0x00 : 0x08), 0, 0);
        delay(light_delays[5]);
      }
    }
  }
}

void transition(bool on)
{
  if (on)
  {
    for (int i = 0; i < 8; i++)
    {
      controller->setRowPowerStatus(BLUE_ROW_LOOKUP[i], B11111111, 0);
      delay(light_delays[2]);
    }

    for (int i = 0; i < 8; i++)
    {
      controller->setRowPowerStatus(BLUE_ROW_LOOKUP[i], B11111111, 5);
      controller->setRowPowerStatus(RED_ROW_LOOKUP[i], B11111111, 5);
      delay(light_delays[2]);
    }

    for (int i = 0; i < 8; i++)
    {
      controller->setRowPowerStatus(RED_ROW_LOOKUP[i], B11111111, 4);
      delay(light_delays[2]);
    }

    for (int i = 7; i >= 0; i--)
    {
      controller->setRowPowerStatus(GREEN_ROW_LOOKUP[i], B11111111, 8);
      controller->setRowPowerStatus(RED_ROW_LOOKUP[i], B11111111, 8);
      delay(light_delays[2]);
    }
    for (int i = 7; i >= 0; i--)
    {
      controller->setRowPowerStatus(GREEN_ROW_LOOKUP[i], B11111111, 12);
      delay(light_delays[2]);
    }
  }
  else
  {
    for (int i = 0; i < 8; i++)
    {
      controller->setRowPowerStatus(BLUE_ROW_LOOKUP[i], 0x00, 0);
      delay(light_delays[2]);
    }

    for (int i = 0; i < 8; i++)
    {
      controller->setRowPowerStatus(BLUE_ROW_LOOKUP[i], 0x00, 5);
      controller->setRowPowerStatus(RED_ROW_LOOKUP[i], 0x00, 5);
      delay(light_delays[2]);
    }

    for (int i = 0; i < 8; i++)
    {
      controller->setRowPowerStatus(RED_ROW_LOOKUP[i], 0x00, 4);
      delay(light_delays[2]);
    }

    for (int i = 7; i >= 0; i--)
    {
      controller->setRowPowerStatus(GREEN_ROW_LOOKUP[i], 0x00, 8);
      controller->setRowPowerStatus(RED_ROW_LOOKUP[i], 0x00, 8);
      delay(light_delays[2]);
    }
    for (int i = 7; i >= 0; i--)
    {
      controller->setRowPowerStatus(GREEN_ROW_LOOKUP[i], 0x00, 12);
      delay(light_delays[2]);
    }
  }
}

void setLEDPWM(uint8_t columns, uint8_t rows, uint8_t pwm, uint8_t colors, uint8_t module)
{
  for (int row = 0; row < 8; row++)
  {
    if (!((rows >> (7 - row)) & 0x01))
    {
      continue;
    }
    for (int column = 0; column < 8; column++)
    {
      if (!((columns >> (7 - column)) & 0x01))
      {
        continue;
      }
      uint8_t odd_offset = (row < 4 ? 0x00 : 0x08);
      uint8_t reg_offset = 0x30 * (row % 4);

      if (colors & COLOR_R)
      {
        controller->setLEDPWM(0xB0 - reg_offset + ((uint8_t)column) + odd_offset, pwm, module);
      }
      if (colors & COLOR_G)
      {
        controller->setLEDPWM(0xB0 - reg_offset + ((uint8_t)column) + odd_offset - 0x10, pwm, module);
      }
      if (colors & COLOR_B)
      {
        controller->setLEDPWM(0xB0 - reg_offset + ((uint8_t)column) + odd_offset - 0x20, pwm, module);
      }
      delay(30);
    }
  }
}

void disgustingTestCode()
{
  setLEDPWM(B11111111, B10000000, 40, COLOR_R);
  setLEDPWM(B10000000, B11111111, 40, COLOR_B);

  setLEDPWM(B11111111, B01000000, 35, COLOR_R);
  setLEDPWM(B01000000, B11111111, 35, COLOR_B);

  setLEDPWM(B11111111, B00100000, 30, COLOR_R);
  setLEDPWM(B00100000, B11111111, 30, COLOR_B);

  setLEDPWM(B11111111, B00010000, 25, COLOR_R);
  setLEDPWM(B00010000, B11111111, 25, COLOR_B);

  setLEDPWM(B11111111, B00001000, 20, COLOR_R);
  setLEDPWM(B00001000, B11111111, 20, COLOR_B);

  setLEDPWM(B11111111, B00000100, 15, COLOR_R);
  setLEDPWM(B00000100, B11111111, 15, COLOR_B);

  setLEDPWM(B11111111, B00000010, 10, COLOR_R);
  setLEDPWM(B00000010, B11111111, 10, COLOR_B);

  setLEDPWM(B11111111, B00000001, 5, COLOR_R);
  setLEDPWM(B00000001, B11111111, 5, COLOR_B);

  setLEDPWM(B11111111, B10000000, 40, COLOR_R, 5);
  setLEDPWM(B10000000, B11111111, 5, COLOR_G, 5);

  setLEDPWM(B11111111, B01000000, 35, COLOR_R, 5);
  setLEDPWM(B01000000, B11111111, 10, COLOR_G, 5);

  setLEDPWM(B11111111, B00100000, 30, COLOR_R, 5);
  setLEDPWM(B00100000, B11111111, 15, COLOR_G, 5);

  setLEDPWM(B11111111, B00010000, 25, COLOR_R, 5);
  setLEDPWM(B00010000, B11111111, 20, COLOR_G, 5);

  setLEDPWM(B11111111, B00001000, 20, COLOR_R, 5);
  setLEDPWM(B00001000, B11111111, 25, COLOR_G, 5);

  setLEDPWM(B11111111, B00000100, 15, COLOR_R, 5);
  setLEDPWM(B00000100, B11111111, 30, COLOR_G, 5);

  setLEDPWM(B11111111, B00000010, 10, COLOR_R, 5);
  setLEDPWM(B00000010, B11111111, 35, COLOR_G, 5);

  setLEDPWM(B11111111, B00000001, 5, COLOR_R, 5);
  setLEDPWM(B00000001, B11111111, 40, COLOR_G, 5);

  setLEDPWM(B11111111, B10000000, 40, COLOR_G, 4);
  setLEDPWM(B10000000, B11111111, 40, COLOR_B, 4);

  setLEDPWM(B11111111, B01000000, 35, COLOR_G, 4);
  setLEDPWM(B01000000, B11111111, 35, COLOR_B, 4);

  setLEDPWM(B11111111, B00100000, 30, COLOR_G, 4);
  setLEDPWM(B00100000, B11111111, 30, COLOR_B, 4);

  setLEDPWM(B11111111, B00010000, 25, COLOR_G, 4);
  setLEDPWM(B00010000, B11111111, 25, COLOR_B, 4);

  setLEDPWM(B11111111, B00001000, 20, COLOR_G, 4);
  setLEDPWM(B00001000, B11111111, 20, COLOR_B, 4);

  setLEDPWM(B11111111, B00000100, 15, COLOR_G, 4);
  setLEDPWM(B00000100, B11111111, 15, COLOR_B, 4);

  setLEDPWM(B11111111, B00000010, 10, COLOR_G, 4);
  setLEDPWM(B00000010, B11111111, 10, COLOR_B, 4);

  setLEDPWM(B11111111, B00000001, 5, COLOR_G, 4);
  setLEDPWM(B00000001, B11111111, 5, COLOR_B, 4);

  setLEDPWM(B00000001, B00000001, 160, COLOR_R, 8);
  setLEDPWM(B00000010, B00000001, 140, COLOR_R, 8);
  setLEDPWM(B00000100, B00000001, 120, COLOR_R, 8);
  setLEDPWM(B00001000, B00000001, 100, COLOR_R, 8);
  setLEDPWM(B00010000, B00000001, 80, COLOR_R, 8);
  setLEDPWM(B00100000, B00000001, 60, COLOR_R, 8);
  setLEDPWM(B01000000, B00000001, 40, COLOR_R, 8);
  setLEDPWM(B10000000, B00000001, 20, COLOR_R, 8);

  setLEDPWM(B00000001, B00000010, 120, COLOR_R, 8);
  setLEDPWM(B00000010, B00000010, 100, COLOR_R, 8);
  setLEDPWM(B00000100, B00000010, 80, COLOR_R, 8);
  setLEDPWM(B00001000, B00000010, 60, COLOR_R, 8);
  setLEDPWM(B00010000, B00000010, 40, COLOR_R, 8);
  setLEDPWM(B00100000, B00000010, 20, COLOR_R, 8);
  setLEDPWM(B01000000, B00000010, 10, COLOR_R, 8);
  setLEDPWM(B10000000, B00000010, 5, COLOR_R, 8);

  setLEDPWM(B00000001, B00000100, 80, COLOR_R, 8);
  setLEDPWM(B00000010, B00000100, 60, COLOR_R, 8);
  setLEDPWM(B00000100, B00000100, 40, COLOR_R, 8);
  setLEDPWM(B00001000, B00000100, 20, COLOR_R, 8);
  setLEDPWM(B00010000, B00000100, 10, COLOR_R, 8);
  setLEDPWM(B00100000, B00000100, 5, COLOR_R, 8);
  setLEDPWM(B01000000, B00000100, 1, COLOR_R, 8);
  setLEDPWM(B10000000, B00000100, 0, COLOR_R, 8);

  setLEDPWM(B00000001, B00001000, 80, COLOR_G, 8);
  setLEDPWM(B00000010, B00001000, 60, COLOR_G, 8);
  setLEDPWM(B00000100, B00001000, 40, COLOR_G, 8);
  setLEDPWM(B00001000, B00001000, 20, COLOR_G, 8);
  setLEDPWM(B00010000, B00001000, 10, COLOR_G, 8);
  setLEDPWM(B00100000, B00001000, 5, COLOR_G, 8);
  setLEDPWM(B01000000, B00001000, 1, COLOR_G, 8);
  setLEDPWM(B10000000, B00001000, 0, COLOR_G, 8);

  setLEDPWM(B00000001, B00010000, 120, COLOR_G, 8);
  setLEDPWM(B00000010, B00010000, 100, COLOR_G, 8);
  setLEDPWM(B00000100, B00010000, 80, COLOR_G, 8);
  setLEDPWM(B00001000, B00010000, 60, COLOR_G, 8);
  setLEDPWM(B00010000, B00010000, 40, COLOR_G, 8);
  setLEDPWM(B00100000, B00010000, 20, COLOR_G, 8);
  setLEDPWM(B01000000, B00010000, 10, COLOR_G, 8);
  setLEDPWM(B10000000, B00010000, 5, COLOR_G, 8);

  setLEDPWM(B00000001, B00100000, 160, COLOR_G, 8);
  setLEDPWM(B00000010, B00100000, 140, COLOR_G, 8);
  setLEDPWM(B00000100, B00100000, 120, COLOR_G, 8);
  setLEDPWM(B00001000, B00100000, 100, COLOR_G, 8);
  setLEDPWM(B00010000, B00100000, 80, COLOR_G, 8);
  setLEDPWM(B00100000, B00100000, 60, COLOR_G, 8);
  setLEDPWM(B01000000, B00100000, 40, COLOR_G, 8);
  setLEDPWM(B10000000, B00100000, 20, COLOR_G, 8);

  setLEDPWM(B00000001, B01000000, 160, COLOR_B, 8);
  setLEDPWM(B00000010, B01000000, 140, COLOR_B, 8);
  setLEDPWM(B00000100, B01000000, 120, COLOR_B, 8);
  setLEDPWM(B00001000, B01000000, 100, COLOR_B, 8);
  setLEDPWM(B00010000, B01000000, 80, COLOR_B, 8);
  setLEDPWM(B00100000, B01000000, 60, COLOR_B, 8);
  setLEDPWM(B01000000, B01000000, 40, COLOR_B, 8);
  setLEDPWM(B10000000, B01000000, 20, COLOR_B, 8);

  setLEDPWM(B00000001, B10000000, 120, COLOR_B, 8);
  setLEDPWM(B00000010, B10000000, 100, COLOR_B, 8);
  setLEDPWM(B00000100, B10000000, 80, COLOR_B, 8);
  setLEDPWM(B00001000, B10000000, 60, COLOR_B, 8);
  setLEDPWM(B00010000, B10000000, 40, COLOR_B, 8);
  setLEDPWM(B00100000, B10000000, 20, COLOR_B, 8);
  setLEDPWM(B01000000, B10000000, 10, COLOR_B, 8);
  setLEDPWM(B10000000, B10000000, 5, COLOR_B, 8);

  setLEDPWM(B10000000, B11111111, 160, COLOR_G, 12);
  setLEDPWM(B11000000, B10000000, 160, COLOR_G, 12);
  setLEDPWM(B00110000, B10000000, 120, COLOR_G, 12);
  setLEDPWM(B00001100, B10000000, 80, COLOR_G, 12);
  setLEDPWM(B00000011, B10000000, 40, COLOR_G, 12);
  setLEDPWM(B01000000, B01000000, 140, COLOR_G, 12);
  setLEDPWM(B00110000, B01000000, 100, COLOR_G, 12);
  setLEDPWM(B00001100, B01000000, 60, COLOR_G, 12);
  setLEDPWM(B00000011, B01000000, 20, COLOR_G, 12);
  setLEDPWM(B01000000, B00100000, 120, COLOR_G, 12);
  setLEDPWM(B00110000, B00100000, 80, COLOR_G, 12);
  setLEDPWM(B00001100, B00100000, 40, COLOR_G, 12);
  setLEDPWM(B01000000, B00010000, 100, COLOR_G, 12);
  setLEDPWM(B00110000, B00010000, 60, COLOR_G, 12);
  setLEDPWM(B00001100, B00010000, 20, COLOR_G, 12);
  setLEDPWM(B01000000, B00001000, 80, COLOR_G, 12);
  setLEDPWM(B00110000, B00001000, 40, COLOR_G, 12);
  setLEDPWM(B01000000, B00000100, 60, COLOR_G, 12);
  setLEDPWM(B00110000, B00000100, 20, COLOR_G, 12);
  setLEDPWM(B01000000, B00000010, 60, COLOR_G, 12);
  setLEDPWM(B01000000, B00000001, 40, COLOR_G, 12);

  setLEDPWM(B00000001, B11111111, 160, COLOR_B, 12);
  setLEDPWM(B00000011, B10000000, 160, COLOR_B, 12);
  setLEDPWM(B00001100, B10000000, 120, COLOR_B, 12);
  setLEDPWM(B00110000, B10000000, 80, COLOR_B, 12);
  setLEDPWM(B11000000, B10000000, 40, COLOR_B, 12);
  setLEDPWM(B00000010, B01000000, 140, COLOR_B, 12);
  setLEDPWM(B00001100, B01000000, 100, COLOR_B, 12);
  setLEDPWM(B00110000, B01000000, 60, COLOR_B, 12);
  setLEDPWM(B11000000, B01000000, 20, COLOR_B, 12);
  setLEDPWM(B00000010, B00100000, 120, COLOR_B, 12);
  setLEDPWM(B00001100, B00100000, 80, COLOR_B, 12);
  setLEDPWM(B00110000, B00100000, 40, COLOR_B, 12);
  setLEDPWM(B00000010, B00010000, 100, COLOR_B, 12);
  setLEDPWM(B00001100, B00010000, 60, COLOR_B, 12);
  setLEDPWM(B00110000, B00010000, 20, COLOR_B, 12);
  setLEDPWM(B00000010, B00001000, 80, COLOR_B, 12);
  setLEDPWM(B00001100, B00001000, 40, COLOR_B, 12);
  setLEDPWM(B00000010, B00000100, 60, COLOR_B, 12);
  setLEDPWM(B00001100, B00000100, 20, COLOR_B, 12);
  setLEDPWM(B00000010, B00000010, 60, COLOR_B, 12);
  setLEDPWM(B00000010, B00000001, 40, COLOR_B, 12);

  setLEDPWM(B00000011, B10000000, 160, COLOR_B, 12);
  setLEDPWM(B00001100, B10000000, 120, COLOR_B, 12);
  setLEDPWM(B00110000, B10000000, 80, COLOR_B, 12);
  setLEDPWM(B11000000, B10000000, 40, COLOR_B, 12);
  setLEDPWM(B00000011, B01000000, 140, COLOR_B, 12);
  setLEDPWM(B00001100, B01000000, 100, COLOR_B, 12);
  setLEDPWM(B00110000, B01000000, 60, COLOR_B, 12);
  setLEDPWM(B11000000, B01000000, 20, COLOR_B, 12);
  setLEDPWM(B00000011, B00100000, 120, COLOR_B, 12);
  setLEDPWM(B00001100, B00100000, 80, COLOR_B, 12);
  setLEDPWM(B00110000, B00100000, 40, COLOR_B, 12);
  setLEDPWM(B00000011, B00010000, 100, COLOR_B, 12);
  setLEDPWM(B00001100, B00010000, 60, COLOR_B, 12);
  setLEDPWM(B00110000, B00010000, 20, COLOR_B, 12);
  setLEDPWM(B00000011, B00001000, 80, COLOR_B, 12);
  setLEDPWM(B00001100, B00001000, 40, COLOR_B, 12);
  setLEDPWM(B00000011, B00000100, 60, COLOR_B, 12);
  setLEDPWM(B00001100, B00000100, 20, COLOR_B, 12);
  setLEDPWM(B00000011, B00000010, 60, COLOR_B, 12);
  setLEDPWM(B00000010, B00000001, 40, COLOR_B, 12);

  setLEDPWM(B11111111, B00000001, 160, COLOR_R, 12);
  setLEDPWM(B10000000, B00000011, 160, COLOR_R, 12);
  setLEDPWM(B10000000, B00001100, 120, COLOR_R, 12);
  setLEDPWM(B10000000, B00110000, 80, COLOR_R, 12);
  setLEDPWM(B10000000, B11000000, 40, COLOR_R, 12);
  setLEDPWM(B01000000, B00000010, 140, COLOR_R, 12);
  setLEDPWM(B01000000, B00001100, 100, COLOR_R, 12);
  setLEDPWM(B01000000, B00110000, 60, COLOR_R, 12);
  setLEDPWM(B01000000, B11000000, 20, COLOR_R, 12);
  setLEDPWM(B00100000, B00000010, 120, COLOR_R, 12);
  setLEDPWM(B00100000, B00001100, 80, COLOR_R, 12);
  setLEDPWM(B00100000, B00110000, 40, COLOR_R, 12);
  setLEDPWM(B00010000, B00000010, 100, COLOR_R, 12);
  setLEDPWM(B00010000, B00001100, 60, COLOR_R, 12);
  setLEDPWM(B00010000, B00110000, 20, COLOR_R, 12);
  setLEDPWM(B00001000, B00000010, 80, COLOR_R, 12);
  setLEDPWM(B00001000, B00001100, 40, COLOR_R, 12);
  setLEDPWM(B00000100, B00000010, 60, COLOR_R, 12);
  setLEDPWM(B00000100, B00001100, 20, COLOR_R, 12);
  setLEDPWM(B00000010, B00000010, 60, COLOR_R, 12);
  setLEDPWM(B00000001, B00000010, 40, COLOR_R, 12);
}