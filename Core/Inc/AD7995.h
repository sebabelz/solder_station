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
    const uint16_t resolution = 1024;
    bool connected = false;

private:
    uint8_t address = (0x28 << 1); // NOLINT(hicpp-signed-bitwise)
    uint8_t config = 0x00;
    uint8_t channelCount = 0;
    uint16_t rawData[4];
    I2C_HandleTypeDef *handle = nullptr;

    void setConfig(uint8_t position, uint8_t value);
    void clearConfig(uint8_t position);
    void countChannels();

public:
    AD7995();
    ~AD7995() = default;

    void setI2CHandle(I2C_HandleTypeDef *handle);
    void setChannels(Channel ch);
    void setExternalReference();
    void clearExternalReference();
    uint16_t getRawData(Channel ch);
    uint16_t getResolution() const;
    bool isConnected() const;

    HAL_StatusTypeDef readAllChannels();
    HAL_StatusTypeDef readChannel(Channel ch);
};

#endif //AD7995_H