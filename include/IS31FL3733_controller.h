#ifndef IS31FL3733_CONTROLLER_H
#define IS31FL3733_CONTROLLER_H

#include <Arduino.h>
#include <Wire.h>
#include "i2c_utils.h"

#define IS31FL3733_ADDR 0x50
#define COLOR_R 0x01
#define COLOR_G 0x02
#define COLOR_B 0x04

const uint8_t RED_ROW_LOOKUP[8] =
    {
        0x13,
        0x0D,
        0x07,
        0x01,
        0x12,
        0x0C,
        0x06,
        0x00,
};
const uint8_t GREEN_ROW_LOOKUP[8] =
    {
        0x15,
        0x0F,
        0x09,
        0x03,
        0x14,
        0x0E,
        0x08,
        0x02,
};
const uint8_t BLUE_ROW_LOOKUP[8] =
    {
        0x17,
        0x11,
        0x0B,
        0x05,
        0x16,
        0x10,
        0x0A,
        0x04,
};

class IS31FL3733_Controller
{
private:
    int module_count;
    uint8_t current_page[16];
    void switchPage(int page, uint8_t module = 0x00);

public:
    IS31FL3733_Controller(int SBD_PIN, bool syncing = false);
    void setShutdown(bool mode, uint8_t module = 0x00);
    void setGlobalBrightness(uint8_t modifier, uint8_t module = 0x00);
    void setLEDPWM(uint8_t led_reg, uint8_t pwm, uint8_t module = 0x00);
    void setLEDRowPower(int row, uint8_t color_mask, uint8_t power_code, uint8_t module = 0x00);
    void setColorPower(uint8_t color_mask, bool powered = true, uint8_t module = 0x00);
    
    void setRowPowerStatus(uint8_t row_reg, uint8_t power_code, uint8_t module = 0x00);
};

#endif