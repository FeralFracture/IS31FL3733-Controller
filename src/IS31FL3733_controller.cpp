#include "IS31FL3733_controller.h"

uint8_t reverseByte(uint8_t x);
IS31FL3733_Controller::IS31FL3733_Controller(int SDB_PIN, bool syncing)
{
    pinMode(SDB_PIN, OUTPUT);
    digitalWrite(SDB_PIN, HIGH);
    for (int i = 0; i < 16; i++)
    {
        current_page[i] = 0x03;
    }
    uint8_t devices[16];
    uint8_t count = scanI2C(devices, sizeof(devices));
    bool masterSet = false;
    for (uint8_t i = 0; i < count; i++)
    {
        if (devices[i] >= 0x50 && devices[i] <= 0x5F)
        {
            // Serial.printf("Found Addr: 0x%02X\n", devices[i]);
            writeRegister(devices[i], 0xFE, 0xC5);
            writeRegister(devices[i], 0xFD, 0x03);
            if (count > 1 && !masterSet && syncing)
            {
                masterSet = true;
                writeRegister(devices[i], 0x00, B01000001);
            }
            else if (count > 1 && syncing)
            {
                writeRegister(devices[i], 0x00, B10000001);
            }
            else
            {
                writeRegister(devices[i], 0x00, 0x01);
            }
            for (uint8_t reg = 0x00; reg <= 0x17; reg++)
            {
                setRowPowerStatus(reg, B00000000, devices[i] - IS31FL3733_ADDR);
            }
        }
    }
}
void IS31FL3733_Controller::switchPage(int page, uint8_t module)
{

    if (current_page[module] != page)
    {
        writeRegister(IS31FL3733_ADDR + module, 0xFE, 0xC5);
        writeRegister(IS31FL3733_ADDR + module, 0xFD, page);
        current_page[module] = page;
    }
}
void IS31FL3733_Controller::setShutdown(bool shutdown, uint8_t module)
{
    switchPage(3, module);
    writeRegister(IS31FL3733_ADDR + module, 0x00, shutdown ? 0x00 : 0x01);
}
void IS31FL3733_Controller::setGlobalBrightness(uint8_t modifier, uint8_t module)
{
    switchPage(3, module);
    writeRegister(IS31FL3733_ADDR + module, 0x01, modifier);
}
void IS31FL3733_Controller::setRowPowerStatus(uint8_t row_reg, uint8_t power_code, uint8_t module)
{
    switchPage(0, module);
    writeRegister(IS31FL3733_ADDR + module, row_reg, power_code);
}
void IS31FL3733_Controller::setLEDPWM(uint8_t led_reg, uint8_t pwm, uint8_t module)
{
    switchPage(1, module);
    writeRegister(IS31FL3733_ADDR + module, led_reg, pwm);
}
void IS31FL3733_Controller::setColorPower(uint8_t color_mask, bool powered, uint8_t module)
{
    uint8_t fill = powered ? 0xFF : 0x00;
    if (color_mask == (COLOR_R | COLOR_G | COLOR_B))
    {
        switchPage(0, module);
        uint8_t data[24];
        memset(data, fill, sizeof(data));
        writeMultipleRegisters(IS31FL3733_ADDR + module, 0x00, data, sizeof(data));
        return;
    }
    for (int i = 0; i < 8; i++)
    {
        setLEDRowPower(i, color_mask, fill, module);
    }
}
void IS31FL3733_Controller::setLEDRowPower(int row, uint8_t color_mask, uint8_t power_code, uint8_t module)
{
    // Red
    if (color_mask & COLOR_R)
    {
        uint8_t row_addr = RED_ROW_LOOKUP[7 - row];
        setRowPowerStatus(row_addr, reverseByte(power_code), module);
    }

    // Green
    if (color_mask & COLOR_G)
    {
        uint8_t row_addr = GREEN_ROW_LOOKUP[7 - row];
        setRowPowerStatus(row_addr, reverseByte(power_code), module);
    }

    // Blue
    if (color_mask & COLOR_B)
    {
        uint8_t row_addr = BLUE_ROW_LOOKUP[7 - row];
        setRowPowerStatus(row_addr, reverseByte(power_code), module);
    }
}

uint8_t reverseByte(uint8_t x)
{
    uint8_t result = 0;

    for (int i = 0; i < 8; i++)
    {
        result <<= 1;      // make room
        result |= (x & 1); // copy lowest bit
        x >>= 1;           // shift input
    }

    return result;
}
