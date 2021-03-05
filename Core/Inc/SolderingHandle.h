#pragma once

#include <vector>
#include "AD7995.h"
#include "PID.h"

#ifdef __cplusplus
extern "C" {

#include <cstdint>
#include "stm32l4xx_hal.h"

}
#endif

enum class ConnectionStatus
{
    Disconnected,
    HandleConncected,
    TipConnected,
};

class SolderingHandle
{
protected:
    const float adcResolution = 1024;
    const float beta = 3940;
    const float refTemp = 298.15;
    const float refVoltage = 3.3;
    const float kelvin = 273.15;
    const float tipGain = 0.48; //0.54


    uint32_t refTemperature = 0;

    ConnectionStatus status = ConnectionStatus::Disconnected;
    AD7995 adConverter;

    static int limitSetPoint(int setPoint);
    float getReferenceTemperature(Channel ch);
public:
    SolderingHandle() = default;
    virtual ~SolderingHandle() = default;

    ConnectionStatus getStatus() const;

    virtual int getSetPoint() = 0;
    virtual int setSetPoint(int setPoint) = 0;
    virtual int increaseSetPoint() = 0;
    virtual int decreaseSetPoint() = 0;
    virtual int increaseSetPoint(int value) = 0;
    virtual int decreaseSetPoint(int value) = 0;

    virtual void processControl() = 0;
    virtual void setOutput(uint16_t *output) = 0;
    virtual void enable() = 0;
    virtual void disable() = 0;
    virtual void toggle() = 0;

    virtual void configADConverter(I2C_HandleTypeDef *handle) = 0;
    virtual void calculateTipTemperature() = 0;
    virtual int getTipTemperature() = 0;
};

