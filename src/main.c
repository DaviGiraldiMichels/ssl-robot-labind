#include "main.h"
#include "app.hpp"

void Error_Handler(){
    while(1){
    }
}

void SysTick_Handler(void){
    HAL_IncTick();
    HAL_SYSTICK_IRQHandler();
}

void SystemClock_Config();
void MX_GPIO_Init();

int main(){
    HAL_Init();

    SystemClock_Config();

    MX_GPIO_Init();

    start();
}
