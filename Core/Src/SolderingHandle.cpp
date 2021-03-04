//
// Created by BelzS on 17.08.2019.
//

#include <cmath>
#include "SolderingHandle.h"

int SolderingHandle::limitSetPoint(int setPoint)
{
    if (setPoint > 500)
        setPoint = 500;
    if (setPoint < 0)
        setPoint = 0;

    return setPoint;
}

float SolderingHandle::getReferenceTemperature(Channel ch)
{
    float voltage = refVoltage / adcResolution * static_cast<float>(adConverter.getRawData(ch));;
    float invTemp = 1 / refTemp + 1 / beta * std::log(voltage / (refVoltage - voltage));
    refTemperature = 1 / invTemp - kelvin;
    return refTemperature;
}

ConnectionStatus SolderingHandle::getStatus() const
{
    return status;
}