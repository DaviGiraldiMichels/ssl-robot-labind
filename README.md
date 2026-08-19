# Sistema de Controle e Comunicação via nRF24L01 (Robô Móvel SSL)

Este projeto demonstra o controle remoto e em tempo real de um robô omnidirecional da categoria *Small Size League* (SSL). O sistema utiliza comunicação via rádio frequência (RF) estruturada em múltiplas etapas, indo desde um controlador no PC até o acionamento de motores Brushless.

Originalmente concebido para validar a comunicação básica entre um Arduino Uno e um STM32F103 via transceivers <small>_[nRF24l01](https://cdn.sparkfun.com/datasheets/Wireless/Nordic/nRF24L01_Product_Specification_v2_0.pdf)_</small>, o projeto foi expandido para atuar como o sistema de tração e cinemática de uma plataforma robótica.

<div style="display:flex; gap:12px; align-items:center;">
    <img src="./images/STM32F401RE.jpg" alt="STM32F401RE Nucleo" width="150" />
    <img src="./images/ARDUINO_R3.png" alt="Arduino Uno R3" width="150" />
    <img src="./images/nRF24l01.png" alt="nRF24l01" width="150" />
</div>

## 🚀 Arquitetura do Sistema

O fluxo de comando bidirecional foi projetado para garantir baixa latência e segurança de hardware:

1. **Controle via PC (`pc_controller.py`):** Um script em Python lê os comandos de velocidade/direção (joystick/teclado) e os envia via serial para o Arduino, utilizando um cabeçalho de sincronia (`0xAA`, `0x55`).
2. **Nó Transmissor (Arduino Uno):** Recebe os pacotes seriais do PC e os transmite via rádio (nRF24L01). Também gerencia o status de *ACK* da comunicação e retorna logs de debug para o Python.
3. **Nó Receptor e Controlador (STM32F401RE):** Recebe os vetores de velocidade desejados via rádio.
4. **Cinemática e Atuação (`ssl_kinematics.c`):** O STM32 calcula a matriz cinemática para as rodas omnidirecionais e gera sinais PWM (50Hz) isolados via optoacopladores (PC817) para comandar 4 ESCs bidirecionais (LittleBee 30A).

### 🛡️ Failsafe de Comunicação
Implementado nativamente no firmware do STM32: a ausência de pacotes RF válidos por mais de **200ms** aciona uma parada de emergência automática (motores enviados para 1500us - neutro).

---

## 🔌 Configuração de Hardware e Pinos

### Nó Receptor: STM32F401RE (Placa Nucleo-64)
*Os pinos de sinal dos motores estão isolados da lógica via optoacopladores PC817 para evitar ruído elétrico dos ESCs.*

| Função / Periférico | Pino STM32F401RE | Observação |
|---------------------|------------------|------------|
| **SPI (nRF24)**     | D9 a D13         | Comunicação com o rádio |
| **PWM Motor 1**     | PA0 (TIM2/3)     | Sinal para ESC 1 |
| **PWM Motor 2**     | PA1 (TIM2/3)     | Sinal para ESC 2 |
| **PWM Motor 3**     | PB10 (TIM2/3)    | Sinal para ESC 3 |
| **PWM Motor 4**     | PB0 (TIM2/3)     | Sinal para ESC 4 |

### Nó Transmissor: Arduino Uno
| Função SPI    | Pino Arduino  |
|---------------|---------------|
| CE            | 7             |
| CSN           | 8             |
| MOSI          | 11            |
| MISO          | 12            |
| CSK           | 13            |

---

## 🛠️ Ferramentas Necessárias e Build

- [Python 3.x](https://www.python.org/) (Para rodar o `pc_controller.py`)
- [Arm GNU Toolchain](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads)
- [CMake](https://cmake.org/)
- [STLINK Tools](https://github.com/stlink-org/stlink)

### Compilação do Firmware (STM32)

A biblioteca base de RF utiliza um wrapper em C. Os arquivos `gcc-arm-none-eabi.cmake` e o script de linker configuram a arquitetura do firmware.

```shell
# Gerar arquivos para a build
cmake -preset=F1 ..

# Compilar e gerar aquivo .elf.
cmake --build .

# Converter para binário.
arm-none-eabi-objcopy -O binary STM32_RobotController.elf firmware.bin

# Apagar firmware antigo e flashear novo.
st-flash erase
st-flash --reset write firmware.bin 0x08000000