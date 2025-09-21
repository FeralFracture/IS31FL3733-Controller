#include "i2c_utils.h"

void terminal_write_scanI2C()
{
    // Scan I2C bus and print devices
    uint8_t devices[8];
    uint8_t count = scanI2C(devices, sizeof(devices));
    Serial.printf("I2C devices found: %u\n", count);
    for (uint8_t i = 0; i < count; i++)
    {
        Serial.printf("  0x%02X\n", devices[i]);
    }
}

uint8_t scanI2C(uint8_t *foundAddrs, size_t maxCount)
{
    uint8_t count = 0;
    for (uint8_t addr = 1; addr < 127; ++addr)
    {
        Wire.beginTransmission(addr);
        if (Wire.endTransmission() == 0)
        {
            if (count < maxCount)
            {
                foundAddrs[count] = addr;
            }
            count++;
        }
    }
    return count; // total devices found
}

bool writeRegister(uint8_t deviceAddr, uint8_t reg, uint8_t val)
{
    Wire.beginTransmission(deviceAddr);
    Wire.write(reg);
    Wire.write(val);
    return (Wire.endTransmission() == 0);
}

bool writeMultipleRegisters(uint8_t deviceAddr, uint8_t startReg, const uint8_t *data, size_t length)
{
    Wire.beginTransmission(deviceAddr);
    Wire.write(startReg);
    Wire.write(data, length);
    return (Wire.endTransmission() == 0);
}

bool readRegister(uint8_t deviceAddr, uint8_t reg, uint8_t *outVal)
{
    Wire.beginTransmission(deviceAddr);
    Wire.write(reg);
    if (Wire.endTransmission(false) != 0)
        return false;

    if (Wire.requestFrom(deviceAddr, (uint8_t)1) == 1)
    {
        *outVal = Wire.read();
        return true;
    }
    return false;
}

size_t readMultipleRegisters(uint8_t deviceAddr, uint8_t startReg, uint8_t *buffer, size_t length)
{
    Wire.beginTransmission(deviceAddr);
    Wire.write(startReg);
    if (Wire.endTransmission(false) != 0)
        return 0;

    size_t bytesRead = Wire.requestFrom(deviceAddr, (uint8_t)length);
    for (size_t i = 0; i < bytesRead; i++)
    {
        buffer[i] = Wire.read();
    }
    return bytesRead;
}
