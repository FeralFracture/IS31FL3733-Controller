#ifndef I2C_UTILS_H
#define I2C_UTILS_H

#include <Arduino.h>
#include <Wire.h>

uint8_t scanI2C(uint8_t* foundAddrs, size_t maxCount);  // returns number of devices found

int writeRegister(uint8_t deviceAddr, uint8_t reg, uint8_t val); // true if success
int writeMultipleRegisters(uint8_t deviceAddr, uint8_t startReg, const uint8_t *data, size_t length, int retries = 3);

bool readRegister(uint8_t deviceAddr, uint8_t reg, uint8_t* outVal); // true if success
size_t readMultipleRegisters(uint8_t deviceAddr, uint8_t startReg, uint8_t* buffer, size_t length);

#endif
