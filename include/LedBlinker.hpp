#pragma once

#if defined(STM32F1)
#include <stm32f1xx_hal.h>
#include <stm32f1xx_hal_conf.h>
#elif defined(STM32F2)
#include <stm32f2xx_hal.h>
#include <stm32f2xx_hal_conf.h>
#elif defined(STM32F3)
#include <stm32f3xx_hal.h>
#include <stm32f3xx_hal_conf.h>
#elif defined(STM32F4)
#include <stm32f4xx_hal.h>
#include <stm32f4xx_hal_conf.h>
#else
#error "No HAL drivers found!"
#endif

class LedBlinker {
    GPIO_TypeDef* port_;
    uint16_t pin_;

public:
    LedBlinker(GPIO_TypeDef* port, uint16_t pin) : port_(port), pin_(pin) {}

    void on(){ HAL_GPIO_WritePin(port_, pin_, GPIO_PIN_RESET); }
    void off(){ HAL_GPIO_WritePin(port_, pin_, GPIO_PIN_SET); }
    void toggle(){ HAL_GPIO_TogglePin(port_, pin_); }

};