cmake_minimum_required(VERSION 3.19)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

set(CMAKE_PROJECT_NAME STM32BluePillRadio)
set(RF24_DIR ${PROJECT_SOURCE_DIR}/Drivers/RF24)

project(${CMAKE_PROJECT_NAME}
    VERSION 0.2
    DESCRIPTION "STM32 BluePill project for radio communication with nRF25l01."
    LANGUAGES C CXX ASM
)

enable_language(C CXX ASM)

# include(${PROJECT_SOURCE_DIR}/cmake/arm-none-eabi.cmake)
# include(${PROJECT_SOURCE_DIR}/cmake/cortex-m4f.cmake)

# cmake -G Ninja -DRF24_DRIVER=STM32 -DRF24_LINKED_DRIVER=STM32 -DSOC=cortex-m3 -DCMAKE_TOOLCHAIN_FILE=..\..\..\cmake\gcc-arm-none-eabi.cmake ..

set(RF24_DRIVER "STM32")
set(RF24_LINKED_DRIVER "STM32")
set(SOC "cortex-m3")
include_directories(${PROJECT_SOURCE_DIR}/Drivers/RF24)

set(STM32_HAL_src
    # ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_adc_ex.c
    # ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_adc.c
    # ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_can.c
    # ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cec.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cortex.c
    # ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_crc.c
    # ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dac_ex.c
    # ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dac.c
    # ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dma.c
    # ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_eth.c
    # ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash_ex.c
    # ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash.c
    # ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio_ex.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio.c
    # ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_hcd.c
    # ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_i2c.c
    # ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_i2s.c
    # ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_irda.c
    # ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_iwdg.c
    # ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_mmc.c
    # ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_msp_template.c
    # ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_nand.c
    # ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_nor.c
    # ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pccard.c
    # ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pcd_ex.c
    # ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pcd.c
    # ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pwr.c
    # ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc_ex.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc.c
    # ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rtc_ex.c
    # ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rtc.c
    # ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_sd.c
    # ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_smartcard.c
    # ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_spi_ex.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_spi.c
    # ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_sram.c
    # ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim_ex.c
    # ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim.c
    # ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_timebase_rtc_alarm_template.c
    # ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_timebase_tim_template.c
    # ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_uart.c
    # ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_usart.c
    # ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_wwdg.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal.c
    # ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_adc.c
    # ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_crc.c
    # ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_dac.c
    # ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_dma.c
    # ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_exti.c
    # ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_fsmc.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_gpio.c
    # ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_i2c.c
    # ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_pwr.c
    # ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_rcc.c
    # ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_rtc.c
    # ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_sdmmc.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_spi.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_tim.c
    # ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_usart.c
    # ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_usb.c
    # ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_utils.c
)

set(STM32_BOOT_src
    ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Device/ST/STM32F1xx/Source/Templates/system_stm32f1xx.c
    ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Device/ST/STM32F1xx/Source/Templates/gcc/startup_stm32f103xb.s
)

set(MAIN_src
    ${CMAKE_SOURCE_DIR}/src/main.c
)

set(NRF24_src
    ${CMAKE_SOURCE_DIR}/Drivers/RF24/RF24.cpp
    ${CMAKE_SOURCE_DIR}/Drivers/RF24/utility/STM32/compatibility.cpp
    ${CMAKE_SOURCE_DIR}/Drivers/RF24/utility/STM32/gpio.cpp
    ${CMAKE_SOURCE_DIR}/Drivers/RF24/utility/STM32/spi.cpp
)

add_executable(${CMAKE_PROJECT_NAME}
    ${STM32_BOOT_src}
    ${STM32_HAL_src}
    ${MAIN_src}
)

target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src/app.cpp
    ${CMAKE_SOURCE_DIR}/src/syscalls.c
    ${CMAKE_SOURCE_DIR}/src/sysmem.c
    ${NRF24_src}
)

target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE STM32=1 STM32F1=1)

include_directories(./include)
include_directories(./Drivers/STM32F1xx_HAL_Driver/Inc)
include_directories(./Drivers/STM32F1xx_HAL_Driver/Inc/Legacy)
include_directories(./Drivers/CMSIS/Device/ST/STM32F1xx/Include)
include_directories(./Drivers/CMSIS/Include)

list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE -T${CMAKE_SOURCE_DIR}/linker/STM32F103.ld)
