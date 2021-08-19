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
    TipConnected,
    TweezerConncected,
};



class SolderingHandle
{
private:
    const float adcResolution = 1024;
    const float beta = 3940;
    const float refTemp = 298.15;
    const float refVoltage = 3.3;
    const float kelvin = 273.15;
    const float tipGain = 0.48; //0.54



    AD7995 _adConverter;
    PID _control[2];
    ConnectionStatus _status = ConnectionStatus::Disconnected;

    uint16_t _tipTemperature[2] = { 0 };
    float refTemperature = 0;
    int _setPoint = 0;

    int limitSetPoint(int setPoint);
    float getReferenceTemperature(Channel ch);
public:
    SolderingHandle();
    explicit SolderingHandle(I2C_HandleTypeDef *handle);

    virtual ~SolderingHandle() = default;

    ConnectionStatus getStatus() const;

    int getSetPoint() const;
    int setSetPoint(int setPoint);

    void setOutput(uint16_t *out);
    void setOutput(uint16_t *out1, uint16_t *out2);
    void enable();
    void disable();
    void toggle();

    void setI2CHandle(I2C_HandleTypeDef *handle);
    void checkConnection();

    int getTipTemperature();
    void calculateTipTemperature();

    void processControl();



};

