#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-signed-bitwise"

#include <cstdio>
#include "AD7995.h"

AD7995::AD7995()
{
}

void AD7995::setI2CHandle(I2C_HandleTypeDef *handle)
{
    if (handle == nullptr)
    {
        printf("ERROR");
    }
    this->handle = handle;
}

void AD7995::setChannels(Channel ch)
{
    setConfig(0x0F, static_cast<std::underlying_type<Channel>::type>(ch) << 4);
    countChannels();
}

void AD7995::setExternalReference()
{
    setConfig(0x7F, 0x08);
    countChannels();
}

void AD7995::clearExternalReference()
{
    clearConfig(0x08);
}

void AD7995::setConfig(uint8_t position, uint8_t value)
{
    this->config &= position;
    this->config |= value;
}

void AD7995::clearConfig(uint8_t position)
{
    this->config &= ~position;
}

void AD7995::countChannels()
{
    this->channelCount = 0;
    auto tmp = (this->config >> 4);

    for (int i = 0; i < 4; ++i)
    {
        if ((tmp & 0x01) == 1)
        {
            ++this->channelCount;
        }
        tmp >>= 1;
    }

    if ((this->config & 0x08) && (this->config & 0x80))
    {
        --this->channelCount;
    }
}

uint16_t AD7995::getRawData(Channel ch)
{
    switch (ch) {
        case Channel::Zero:
            return rawData[0];
        case Channel::One:
            return rawData[1];
        case Channel::Two:
            return rawData[2];
        case Channel::Three:
            return rawData[3];
        default:
            return 0;
    }
}

uint16_t AD7995::getResolution() const
{
    return resolution;
}

bool AD7995::isConnected() const
{
    return connected;
}

HAL_StatusTypeDef AD7995::readAllChannels()
{
    HAL_I2C_Master_Transmit(this->handle, this->address, &this->config, sizeof(this->config), 1);

    uint8_t data[8] = {0};
    HAL_StatusTypeDef status = HAL_I2C_Master_Receive(this->handle, this->address, data, this->channelCount * 2 * sizeof(uint8_t), 1);

    for (int i = 0, j = 0; i < channelCount; ++i, j += 2)
    {
        uint8_t pos = (data[j] >> 4) & 0x03;
        this->rawData[pos] = (((data[j] << 6) & 0x3C0) | ((data[j + 1] >> 2) & 0x3F));
    }
    connected = status == HAL_OK;
    return status;
}

HAL_StatusTypeDef AD7995::readChannel(Channel ch)
{
    auto tmpConfig = config;
    HAL_I2C_Master_Transmit(this->handle, this->address, &tmpConfig, sizeof(tmpConfig), 1);

    uint8_t data[2] = {0};
    HAL_StatusTypeDef status = HAL_I2C_Master_Receive(this->handle, this->address, data, 2 * sizeof(uint8_t), 1);


    uint8_t pos = (data[0] >> 4) & 0x03;
    this->rawData[pos] = (((data[0] << 6) & 0x3C0) | ((data[1] >> 2) & 0x3F));

    connected = status == HAL_OK;
    return status;
}