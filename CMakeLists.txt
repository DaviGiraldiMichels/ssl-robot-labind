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

if(NOT DEFINED STM32_MODEL)
	message(FATAL_ERROR "Modelo de stm32 não selecionado.\nSelecione o modelo com 'STM32_MODEL'.")
endif()

message(STATUS "Usando modelo de stm32: '${STM32_MODEL}'")

if(${STM32_MODEL} STREQUAL "F1")
	include(${PROJECT_SOURCE_DIR}/cmake/f103_sources.cmake)
# elseif(${STM32_MODEL} STREQUAL "F2")
# elseif(${STM32_MODEL} STREQUAL "F3")
elseif(${STM32_MODEL} STREQUAL "F4")
	include(${PROJECT_SOURCE_DIR}/cmake/f401xe_sources.cmake)
else()
	message(FATAL_ERROR "Modelo escolhido '${STM32_MODEL}' sem suporte.\nUse F1 ou F4.")
endif()

set(RF24_DRIVER "STM32")
set(RF24_LINKED_DRIVER "STM32")
set(SOC "${STM32_CORE}")
include_directories(${PROJECT_SOURCE_DIR}/Drivers/RF24)

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

if(${STM32_MODEL} STREQUAL "F1")
	target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE STM32=1 STM32F1=1)
	target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src/setup_f1.c)
	target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE -T${CMAKE_SOURCE_DIR}/linker/STM32F103.ld)
	include_directories(./Drivers/STM32F1xx_HAL_Driver/Inc)
	include_directories(./Drivers/STM32F1xx_HAL_Driver/Inc/Legacy)
	include_directories(./Drivers/CMSIS/Device/ST/STM32F1xx/Include)
elseif(${STM32_MODEL} STREQUAL "F2")
	# target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE STM32=1 STM32F2=1)
	# target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src/setup_f2.c)
	#target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE -T${CMAKE_SOURCE_DIR}/linker/STM32F103.ld)
	# include_directories(./Drivers/STM32F2xx_HAL_Driver/Inc)
	# include_directories(./Drivers/STM32F2xx_HAL_Driver/Inc/Legacy)
	# include_directories(./Drivers/CMSIS/Device/ST/STM32F2xx/Include)
elseif(${STM32_MODEL} STREQUAL "F3")
	# target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src/setup_f3.c)
	# target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE STM32=1 STM32F3=1)
	#target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE -T${CMAKE_SOURCE_DIR}/linker/STM32F103.ld)
	# include_directories(./Drivers/STM32F3xx_HAL_Driver/Inc)
	# include_directories(./Drivers/STM32F3xx_HAL_Driver/Inc/Legacy)
	# include_directories(./Drivers/CMSIS/Device/ST/STM32F3xx/Include)
elseif(${STM32_MODEL} STREQUAL "F4")
	target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE STM32=1 STM32F4=1 STM32F401xE=1)
	target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -g)
	target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src/setup_f4.c)
	target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE -T${CMAKE_SOURCE_DIR}/linker/STM32F401RETX_FLASH.ld)
	include_directories(./Drivers/STM32F4xx_HAL_Driver/Inc)
	include_directories(./Drivers/STM32F4xx_HAL_Driver/Inc/Legacy)
	include_directories(./Drivers/CMSIS/Device/ST/STM32F4xx/Include)
endif()


include_directories(./include)
include_directories(./Drivers/CMSIS/Include)
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)
