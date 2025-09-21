#include "i2c_utils.h"

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

int writeRegister(uint8_t deviceAddr, uint8_t reg, uint8_t val)
{
    Wire.beginTransmission(deviceAddr);
    Wire.write(reg);
    Wire.write(val);
    int status = Wire.endTransmission();
    if (status != 0)
    {
        Serial.printf("I2C writeRegister: addr=0x%02X reg=0x%02X data=0x%02X | status=%d\n", deviceAddr, reg, val, status);
    }
    return status; // 0 = success, nonzero = error code
}

int writeMultipleRegisters(uint8_t deviceAddr, uint8_t startReg, const uint8_t *data, size_t length, int retries)
{
    int status;
    for (int attempt = 0; attempt < retries; attempt++)
    {
        Wire.beginTransmission(deviceAddr);
        Wire.write(startReg);
        Wire.write(data, length);
        status = Wire.endTransmission();

        if (status == 0)
            return 0; // success

        Serial.printf("I2C writeMultipleRegisters failed (try %d/%d): addr=0x%02X status=%d\n",
                      attempt + 1, retries, deviceAddr, status);
        delay(2); // small delay before retry
    }
    return status; // return last error code
}

bool readRegister(uint8_t deviceAddr, uint8_t reg, uint8_t *outVal)
{
    Wire.beginTransmission(deviceAddr);
    Wire.write(reg);
    int status = Wire.endTransmission(false); // send restart
    if (status != 0)
    {
        Serial.printf("I2C readRegister failed: addr=0x%02X reg=0x%02X status=%d\n", deviceAddr, reg, status);
        return false;
    }

    if (Wire.requestFrom(deviceAddr, (uint8_t)1) != 1)
    {
        Serial.printf("I2C readRegister requestFrom failed: addr=0x%02X reg=0x%02X\n", deviceAddr, reg);
        return false;
    }

    *outVal = Wire.read();
    return true;
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
