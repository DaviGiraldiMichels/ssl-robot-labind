
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

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

void Error_Handler(void);

#ifdef __cplusplus
}
#endif

#endif
