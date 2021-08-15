#include <cmath>
#include "SolderingHandle.h"

SolderingHandle::SolderingHandle() {
    for (uint8_t i=0; i<2;++i) {
        _control[i] = PID(196.0, 38.0, 172.8);
        _control[i].setInput(&_tipTemperature[i]);
        _control[i].setOutputBounds(0, 9000);
    }
}

SolderingHandle::SolderingHandle(I2C_HandleTypeDef *handle) {
    _adConverter.setI2CHandle(handle);
    for (uint8_t i=0; i<2; ++i) {
        _control[i] = PID(196.0, 38.0, 172.8);
        _control[i].setInput(&_tipTemperature[i]);
        _control[i].setOutputBounds(0, 9000);
    }
}

int SolderingHandle::limitSetPoint(int setPoint)
{
    if (setPoint > 500) { setPoint = 500; }
    if (setPoint < 0) { setPoint = 0; }

    return setPoint;
}

float SolderingHandle::getReferenceTemperature(Channel ch)
{
    float voltage = refVoltage / adcResolution * static_cast<float>(_adConverter.getRawData(ch));;
    float invTemp = 1 / refTemp + 1 / beta * std::log(voltage / (refVoltage - voltage));
    refTemperature = 1 / invTemp - kelvin;
    return refTemperature;
}

ConnectionStatus SolderingHandle::getStatus() const
{
    return _status;
}

int SolderingHandle::getSetPoint() const {
    return _setPoint;
}

int SolderingHandle::setSetPoint(int setPoint)
{
    _setPoint = limitSetPoint(setPoint);
    _control[0].setSetPoint(_setPoint);
    _control[1].setSetPoint(_setPoint);
    return _setPoint;
}

void SolderingHandle::setOutput(uint16_t *out) {
    _control[0].setOutput(out);
}

void SolderingHandle::setOutput(uint16_t *out1, uint16_t *out2) {
    _control[0].setOutput(out1);
    _control[1].setOutput(out2);
}

void SolderingHandle::enable() {
    _control[0].enableControl();
    _control[1].enableControl();
}

void SolderingHandle::disable() {
    _control[0].disableControl();
    _control[1].disableControl();
}

void SolderingHandle::toggle() {
    _control[0].toggleControl();
    _control[1].toggleControl();
}


void SolderingHandle::setI2CHandle(I2C_HandleTypeDef *handle) {
    _adConverter.setI2CHandle(handle);
}

void SolderingHandle::checkConnection() {
    if (HAL_OK == HAL_I2C_IsDeviceReady(_adConverter.getI2CHandle(), (0x28 << 1), 1, 1)) {
        _adConverter.setAddress((0x28 << 1));
        _adConverter.setChannels(Channel::Zero | Channel::One | Channel::Two);
        _adConverter.setExternalReference();
        _status = ConnectionStatus::TipConnected;

        return;
    }
    if (HAL_OK == HAL_I2C_IsDeviceReady(_adConverter.getI2CHandle(), (0x29 << 1), 1, 1)) {
        _adConverter.setAddress((0x29 << 1));
        _adConverter.setChannels(Channel::Zero | Channel::One | Channel::Two | Channel::Three);
        _adConverter.clearExternalReference();
        _status = ConnectionStatus::TweezerConncected;

        return;
    }

    _status = ConnectionStatus::Disconnected;
}

int SolderingHandle::getTipTemperature() {
    if (_status == ConnectionStatus::TipConnected && _tipTemperature[0] < 510) {
        return _tipTemperature[0];
    }
    if (_status == ConnectionStatus::TweezerConncected && _tipTemperature[0] < 510 && _tipTemperature[1] < 510) {
        return (_tipTemperature[0] + _tipTemperature[1]) / 2;
    }
    return -1;
}



void SolderingHandle::calculateTipTemperature() {
    if (_status == ConnectionStatus::TipConnected) {
        _tipTemperature[0] = static_cast<uint32_t>(tipGain * static_cast<float>(_adConverter.getRawData(Channel::Two))
                                                   + getReferenceTemperature(Channel::One));
    }

    if (_status == ConnectionStatus::TweezerConncected) {
        _tipTemperature[0] = static_cast<uint32_t>(tipGain * static_cast<float>(_adConverter.getRawData(Channel::One))
                                                   + getReferenceTemperature(Channel::Zero));

        _tipTemperature[1] = static_cast<uint32_t>(tipGain * static_cast<float>(_adConverter.getRawData(Channel::Two))
                                                   + getReferenceTemperature(Channel::Zero));
    }
}

void SolderingHandle::processControl() {
    _status = _adConverter.readAllChannels() ==
             HAL_OK ? _status : ConnectionStatus::Disconnected;

    calculateTipTemperature();
    _control[0].processData(HAL_GetTick());
    _control[1].processData(HAL_GetTick());
}




