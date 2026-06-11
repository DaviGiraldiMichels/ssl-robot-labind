#include <LedBlinker.hpp>
#include <app.hpp>

#include <nRF24L01.h>
#include <RF24.h>

extern "C" {

#include <main.h>

}

SPI_HandleTypeDef spi1_handle;
// TIM_HandleTypeDef tim1_handle;
// UART_HandleTypeDef uart2_handle;

const uint8_t address[5] = {'0', '0', '0', '0', '1'};

void start(void) {
    LedBlinker led(GPIOA, GPIO_PIN_4);

    __HAL_RCC_SPI1_CLK_ENABLE();
    HAL_Delay(1000);
    spi1_handle.Instance = SPI1;
    spi1_handle.Init.Mode = SPI_MODE_MASTER;
    spi1_handle.Init.Direction = SPI_DIRECTION_2LINES;
    spi1_handle.Init.DataSize = SPI_DATASIZE_8BIT;
    spi1_handle.Init.CLKPolarity = SPI_POLARITY_LOW;
    spi1_handle.Init.CLKPhase = SPI_PHASE_1EDGE;
    spi1_handle.Init.NSS = SPI_NSS_SOFT;
    spi1_handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
    spi1_handle.Init.FirstBit = SPI_FIRSTBIT_MSB;
    spi1_handle.Init.TIMode = SPI_TIMODE_DISABLE;
    spi1_handle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    spi1_handle.Init.CRCPolynomial = 10;
    if (HAL_SPI_Init(&spi1_handle) != HAL_OK) Error_Handler();

    RF24_SPI radio_spi;
    radio_spi.begin(&spi1_handle);
    RF24 radio;
    if(!radio.begin(&radio_spi, RF24_PC7, RF24_PB6)){
        led.off();
        Error_Handler();
    } else {
        radio.openReadingPipe(0, address);
        radio.setPALevel(RF24_PA_MIN);
        radio.setChannel(120);
        radio.setDataRate(RF24_1MBPS);
        radio.setAutoAck(true);
        radio.startListening();
    }

    while(true){
        if (radio.available()) {
            char text[32] = "";
            radio.read(&text, sizeof(text));
            HAL_Delay(100);
            led.toggle();
        }
    }
}