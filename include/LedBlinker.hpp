#pragma once

#include "stm32f1xx_hal.h"

class LedBlinker {
    GPIO_TypeDef* port_;
    uint16_t pin_;

public:
    LedBlinker(GPIO_TypeDef* port, uint16_t pin) : port_(port), pin_(pin) {}

    void on(){ HAL_GPIO_WritePin(port_, pin_, GPIO_PIN_RESET); }
    void off(){ HAL_GPIO_WritePin(port_, pin_, GPIO_PIN_SET); }
    void toggle(){ HAL_GPIO_TogglePin(port_, pin_); }

};