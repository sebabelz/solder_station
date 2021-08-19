//
// Created by BelzS on 28.07.2019.
//

#ifndef AD7995_H
#define AD7995_H

#include <array>
#include <ostream>
#include <type_traits>
#include "Channel.h"

#ifdef __cplusplus
extern "C" {

#include <cstdint>
#include "stm32l4xx_hal.h"

}
#endif


class AD7995 final
{
private:
    const uint16_t _resolution = 1024;
    bool connected = false;

private:
    uint8_t _address = 0; // NOLINT(hicpp-signed-bitwise)
    uint8_t config = 0x00;
    uint8_t channelCount = 0;
    uint16_t rawData[4];
    I2C_HandleTypeDef *_handle = nullptr;

    void setConfig(uint8_t position, uint8_t value);
    void clearConfig(uint8_t position);
    void countChannels();

public:
    AD7995();
    explicit AD7995(uint8_t address);
    ~AD7995() = default;

    void setAddress(uint8_t address);
    void setI2CHandle(I2C_HandleTypeDef *handle);
    I2C_HandleTypeDef* getI2CHandle();
    void setChannels(Channel ch);
    void setExternalReference();
    void clearExternalReference();
    uint16_t getRawData(Channel ch);
    uint16_t getResolution() const;

    HAL_StatusTypeDef readAllChannels();
    HAL_StatusTypeDef readChannel(Channel ch);
};

#endif //AD7995_H