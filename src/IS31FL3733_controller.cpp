#include "IS31FL3733_controller.h"

IS31FL3733_Controller::IS31FL3733_Controller(int SDB_PIN)
{
    pinMode(SDB_PIN, OUTPUT);
    digitalWrite(SDB_PIN, HIGH);
    setShutdown(false);
    for (uint8_t reg = 0x00; reg <= 0x17; reg++)
    {
        setLEDRowPowerStatus(reg, B00000000);
    }
}
void IS31FL3733_Controller::switchPage(int page)
{
    if (current_page != page)
    {
        writeRegister(IS31FL3733_ADDR, 0xFE, 0xC5);
        writeRegister(IS31FL3733_ADDR, 0xFD, page);
        current_page = page;
    }
}
void IS31FL3733_Controller::setShutdown(bool shutdown)
{
    switchPage(3);
    writeRegister(IS31FL3733_ADDR, 0x00, shutdown ? 0x00 : 0x01);
}
void IS31FL3733_Controller::setGlobalBrightness(uint8_t modifier)
{
    switchPage(3);
    writeRegister(IS31FL3733_ADDR, 0x01, modifier);
}
void IS31FL3733_Controller::setLEDRowPowerStatus(uint8_t row_reg, uint8_t power_code)
{
    switchPage(0);
    writeRegister(IS31FL3733_ADDR, row_reg, power_code);
}
void IS31FL3733_Controller::setLEDPWM(uint8_t led_reg, uint8_t pwm)
{
    switchPage(1);
    writeRegister(IS31FL3733_ADDR, led_reg, pwm);
}
void IS31FL3733_Controller::powerAll(uint8_t color_mask, bool powered)
{
    uint8_t fill = powered ? 0xFF : 0x00;
    if (color_mask == (COLOR_R | COLOR_G | COLOR_B))
    {
        switchPage(0);
        uint8_t data[24];
        memset(data, fill, sizeof(data));       
        writeMultipleRegisters(IS31FL3733_ADDR, 0x00, data, sizeof(data));
        return;
    }
    for (int i = 0; i < 8; i++)
    {
        setLEDPowers(i, color_mask, fill);
    }
}
void IS31FL3733_Controller::setLEDPowers(int row, uint8_t color_mask, uint8_t power_code)
{
    // Red
    if (color_mask & COLOR_R)
    {
        uint8_t row_addr = RED_ROW_LOOKUP[row];
        setLEDRowPowerStatus(row_addr, power_code);
    }

    // Green
    if (color_mask & COLOR_G)
    {
        uint8_t row_addr = GREEN_ROW_LOOKUP[row];
        setLEDRowPowerStatus(row_addr, power_code);
    }

    // Blue
    if (color_mask & COLOR_B)
    {
        uint8_t row_addr = BLUE_ROW_LOOKUP[row];
        setLEDRowPowerStatus(row_addr, power_code);
    }
}
