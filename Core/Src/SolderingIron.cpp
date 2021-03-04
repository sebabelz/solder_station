//
// Created by BelzS on 09.08.2019.
//

#include "SolderingIron.h"
#include <iostream>

SolderingIron::SolderingIron()
{
    control = PID(196.0, 38.0, 172.8);
    control.setInput(&tipTemperature);
    control.setOutputBounds(0, 9000);
}

SolderingIron::~SolderingIron()
{

}

void SolderingIron::configADConverter(I2C_HandleTypeDef *handle)
{
    adConverter.setI2CHandle(handle);
    adConverter.setChannels(Channel::Zero | Channel::One | Channel::Two);
    adConverter.setExternalReference();
}

void SolderingIron::calculateTipTemperature()
{
    tipTemperature = static_cast<uint32_t>(tipGain * static_cast<float>(adConverter.getRawData(Channel::Two))
                                           + getReferenceTemperature(Channel::One));

    if (tipTemperature < 550 && status == ConnectionStatus::HandleConncected)
    {
        status = ConnectionStatus::TipConnected;
    }
}

int SolderingIron::getTipTemperature()
{
    return tipTemperature;
}

void SolderingIron::processControl()
{
    status = adConverter.readAllChannels() ==
             HAL_OK ? ConnectionStatus::HandleConncected : ConnectionStatus::Disconnected;

    calculateTipTemperature();
    control.processData(HAL_GetTick());
}

void SolderingIron::setOutput(uint16_t *output)
{
    control.setOutput(output);
}

int SolderingIron::getSetPoint()
{
    return control.getSetPoint();
}

int SolderingIron::setSetPoint(int setPoint)
{
    control.setSetPoint(setPoint);
    return setPoint;
}

int SolderingIron::increaseSetPoint()
{
    return increaseSetPoint(1);
}

int SolderingIron::decreaseSetPoint()
{
    return decreaseSetPoint(1);
}

int SolderingIron::increaseSetPoint(int value)
{
    int setPoint = setPoint = control.getSetPoint();
    control.setSetPoint(limitSetPoint(setPoint += value));
    return setPoint;
}

int SolderingIron::decreaseSetPoint(int value)
{
    int setPoint = setPoint = control.getSetPoint();
    control.setSetPoint(limitSetPoint(setPoint -= value));
    return setPoint;
}

void SolderingIron::enable()
{
    control.enableControl();
}

void SolderingIron::disable()
{
    control.disableControl();
}

void SolderingIron::toggle()
{
    control.toggleControl();
}



