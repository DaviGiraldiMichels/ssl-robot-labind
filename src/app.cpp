#include <LedBlinker.hpp>
#include <app.hpp>
#include <string.h>

#include <nRF24L01.h>
#include <RF24.h>

extern "C" {

#include <main.h>
#include <ssl_kinematics.h>

}

SPI_HandleTypeDef spi1_handle;
TIM_HandleTypeDef htim2; // Timer para os motores
TIM_HandleTypeDef htim3; // Timer extra para o motor 4
// UART_HandleTypeDef uart2_handle;

struct RobotCommand {
    float vx;
    float vy;
    float omega;
};

void MX_TIM_Init(void) {
    TIM_OC_InitTypeDef sConfigOC;
    // Usar memset garante que todos os campos sejam zerados sem disparar avisos de inicialização pendente
    memset(&sConfigOC, 0, sizeof(sConfigOC));

    // Ativa o clock do Timer 2 e Timer 3
    __HAL_RCC_TIM2_CLK_ENABLE();
    __HAL_RCC_TIM3_CLK_ENABLE();

    // Configuração para 50Hz (20ms de período) em um clock de 84MHz
    // Frequência = Clock / ((PSC + 1) * (ARR + 1))
    // 50 = 84.000.000 / (84 * 20.000)
    htim2.Instance = TIM2;
    htim2.Init.Prescaler = 84 - 1; // Clock de 1MHz (1 tick = 1us)
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = 20000 - 1; // 20.000 ticks = 20ms
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    if (HAL_TIM_PWM_Init(&htim2) != HAL_OK) Error_Handler();

    // Replicação para o Timer 3
    htim3.Instance = TIM3;
    htim3.Init.Prescaler = 84 - 1;
    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim3.Init.Period = 20000 - 1;
    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    if (HAL_TIM_PWM_Init(&htim3) != HAL_OK) Error_Handler();

    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 1500; // Inicia em neutro (1.5ms)
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    
    HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1);
    HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2);
    HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_3);
    HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3); // Motor 4 no TIM3_CH3
}

const uint8_t address[5] = {'0', '0', '0', '0', '1'};

extern "C" void start(void) {
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

    MX_TIM_Init();
    // Inicializa a cinemática e o PWM passando os dois timers
    SSL_Motors_Init(&htim2, &htim3);

    if(!radio.begin(&radio_spi, RF24_PC7, RF24_PB6)){
        led.off();
        Error_Handler();
    } else {
        radio.openReadingPipe(0, address);
        radio.setPALevel(RF24_PA_MIN);
        radio.setChannel(120);
        radio.setDataRate(RF24_1MBPS);
        radio.startListening();
    }

    uint32_t last_packet_time = HAL_GetTick();
    const uint32_t TIMEOUT_MS = 200; // 200ms de limite sem sinal por segurança

    while(true){
        if (radio.available()) {
            RobotCommand cmd;
            radio.read(&cmd, sizeof(cmd));
            
            // Reseta o timestamp de segurança sempre que um pacote chega
            last_packet_time = HAL_GetTick();
            
            // Executa a movimentação baseada no payload recebido
            SSL_Robot_Move(cmd.vx, cmd.vy, cmd.omega);
            led.toggle();
        }

        // Failsafe: deve rodar fora do if do rádio
        if ((HAL_GetTick() - last_packet_time) > TIMEOUT_MS) {
            SSL_Robot_Move(0.0f, 0.0f, 0.0f); // Para o robô imediatamente
        }
    }
}