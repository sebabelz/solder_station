//
// Created by BelzS on 09.08.2019.
//

#ifndef SOLDERINGIRON_H
#define SOLDERINGIRON_H

#include "SolderingHandle.h"
#include "PID.h"

#ifdef __cplusplus
extern "C" {

#include <cstdint>
#include "stm32l4xx_hal.h"

}

#endif

class SolderingIron : public SolderingHandle
{
private:
    PID control;
    uint16_t tipTemperature = 0;

public:

    SolderingIron();
    ~SolderingIron() override;

    void configADConverter(I2C_HandleTypeDef *handle) override;
    void calculateTipTemperature() override;
    int getTipTemperature() override;
    void processControl() override;
    void setOutput(uint16_t *output) override;

    int getSetPoint() override;
    int setSetPoint(int setPoint) override;
    int increaseSetPoint() override;
    int decreaseSetPoint() override;
    int increaseSetPoint(int value) override;
    int decreaseSetPoint(int value) override;
    void enable() override;
    void disable() override;
    void toggle() override;
};

#endif //SOLDERINGIRON_H