#ifndef LED_BLINKER_HPP
#define LED_BLINKER_HPP

#include "stm32f4xx_hal.h"

class LedBlinker {
public:
    LedBlinker(GPIO_TypeDef* port, uint16_t pin) : _port(port), _pin(pin) {}

    void on() {
        HAL_GPIO_WritePin(_port, _pin, GPIO_PIN_SET);
    }

    void off() {
        HAL_GPIO_WritePin(_port, _pin, GPIO_PIN_RESET);
    }

    void toggle() {
        HAL_GPIO_TogglePin(_port, _pin);
    }

private:
    GPIO_TypeDef* _port;
    uint16_t _pin;
};

#endif