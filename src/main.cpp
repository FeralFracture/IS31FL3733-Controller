#include <Arduino.h>
#include "IS31FL3733_controller.h"

const int SDA_PIN = 4;
const int SCL_PIN = 22;
const int INTB_PIN = 21;
const int SDB_PIN = 23;
int global_brightness = 0;

const int light_delays[3] = {10, 600, 5};
const int brightness_levels[10] = {1, 5, 10, 25, 50, 75, 100, 150, 250, 255};
IS31FL3733_Controller *controller;

void debugGlobalBrightness();
void setAllPWM(uint8_t val, uint8_t module = 0);

void setup()
{
  Serial.begin(921600);
  Serial.println(" ");

  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.setClock(400000);

  controller = new IS31FL3733_Controller(SDB_PIN);
  controller->setGlobalBrightness(10, 5);
  controller->powerAll(COLOR_G | COLOR_B, true, 5);
  setAllPWM(12, 5);

  controller->setGlobalBrightness(10, 0);
  controller->powerAll(COLOR_B, true, 0);
  setAllPWM(12, 0);

  //   int pwm_level = 5;
  // for (uint8_t i = 0x00; i <= 0xBF; i++)
  // {
  //   controller->setLEDPWM(i, pwm_level);
  //   pwm_level += 3;
  //   if (pwm_level > 255)
  //   {
  //     pwm_level = 1;
  //   }
  // }
}

void loop()
{

  // debugGlobalBrightness();
}

void setAllPWM(uint8_t val, uint8_t module)
{
  for (uint8_t reg = 0x00; reg <= 0xBF; reg++)
  {
    controller->setLEDPWM(reg, val, module);
  }
}
void debugGlobalBrightness()
{
  Serial.println(brightness_levels[global_brightness]);
  // RED
  for (int i = 0; i < 8; i++)
  {
    delay(light_delays[0]);
    controller->setLEDPowers(i, COLOR_R, B11111111);
  }
  delay(light_delays[1]);
  for (int i = 8; i >= 0; i--)
  {
    delay(light_delays[2]);
    controller->setLEDPowers(i, COLOR_R, B00000000);
  }

  // BLUE
  for (int i = 0; i < 8; i++)
  {
    delay(light_delays[0]);
    controller->setLEDPowers(i, COLOR_B, B11111111);
  }
  delay(light_delays[1]);
  for (int i = 8; i >= 0; i--)
  {
    delay(light_delays[2]);
    controller->setLEDPowers(i, COLOR_B, B00000000);
  }

  // GREEN
  for (int i = 0; i < 8; i++)
  {
    delay(light_delays[0]);
    controller->setLEDPowers(i, COLOR_G, B11111111);
  }
  delay(light_delays[1]);
  for (int i = 8; i >= 0; i--)
  {
    delay(light_delays[2]);
    controller->setLEDPowers(i, COLOR_G, B00000000);
  }

  // RG
  for (int i = 0; i < 8; i++)
  {
    delay(light_delays[0]);
    controller->setLEDPowers(i, COLOR_R | COLOR_G, B11111111);
  }
  delay(light_delays[1]);
  for (int i = 8; i >= 0; i--)
  {
    delay(light_delays[2]);
    controller->setLEDPowers(i, COLOR_R | COLOR_G, B00000000);
  }

  // RB
  for (int i = 0; i < 8; i++)
  {
    delay(light_delays[0]);
    controller->setLEDPowers(i, COLOR_R | COLOR_B, B11111111);
  }
  delay(light_delays[1]);
  for (int i = 8; i >= 0; i--)
  {
    delay(light_delays[2]);
    controller->setLEDPowers(i, COLOR_R | COLOR_B, B00000000);
  }

  // GB
  for (int i = 0; i < 8; i++)
  {
    delay(light_delays[0]);

    controller->setLEDPowers(i, COLOR_G | COLOR_B, B11111111);
  }
  delay(light_delays[1]);
  for (int i = 8; i >= 0; i--)
  {
    delay(light_delays[2]);
    controller->setLEDPowers(i, COLOR_G | COLOR_B, B00000000);
  }

  // RGB
  for (int i = 0; i < 8; i++)
  {
    delay(light_delays[0]);
    controller->setLEDPowers(i, COLOR_R | COLOR_G | COLOR_B, B11111111);
  }
  delay(light_delays[1]);
  for (int i = 8; i >= 0; i--)
  {
    delay(light_delays[2]);
    controller->setLEDPowers(i, COLOR_R | COLOR_G | COLOR_B, B00000000);
  }

  global_brightness += 1;
  if (global_brightness > 9)
  {
    global_brightness = 0;
  }
  controller->setGlobalBrightness(brightness_levels[global_brightness]);
}
