#ifndef SSL_KINEMATICS_H
#define SSL_KINEMATICS_H

#include "stm32f4xx_hal.h"

/* Definições dos sinais PWM do ESC (em microssegundos) */
#define ESC_PULSE_MIN      1000 // Rotação máxima em um sentido
#define ESC_PULSE_NEUTRAL  1500 // Motor Parado
#define ESC_PULSE_MAX      2000 // Rotação máxima no outro sentido

/* Configuração geométrica do robô */
/* Em um SSL padrão, os motores costumam ficar a 45, 135, 225 e 315 graus.
   O valor 0.7071 é o cos(45º) e sin(45º). Ajuste caso os ângulos do seu projeto sejam outros. */
#define COS_45 0.7071f
#define SIN_45 0.7071f

/**
 * @brief Inicializa o controle dos motores (Inicia os PWMs no ponto neutro)
 * @param htim2 Ponteiro para o Timer dos motores 1, 2 e 3
 * @param htim3 Ponteiro para o Timer do motor 4
 */
void SSL_Motors_Init(TIM_HandleTypeDef *htim2, TIM_HandleTypeDef *htim3);

/**
 * @brief Define os pulsos PWM cruamente para cada motor
 * @param m1 Pulso para o motor 1 (1000 a 2000)
 * @param m2 Pulso para o motor 2 (1000 a 2000)
 * @param m3 Pulso para o motor 3 (1000 a 2000)
 * @param m4 Pulso para o motor 4 (1000 a 2000)
 */
void SSL_Motors_SetPulses(uint16_t m1, uint16_t m2, uint16_t m3, uint16_t m4);

/**
 * @brief Move o robô omnidirecional com base em vetores
 * @param vx Velocidade desejada no eixo X (Direita/Esquerda) [-1.0 a 1.0]
 * @param vy Velocidade desejada no eixo Y (Frente/Trás) [-1.0 a 1.0]
 * @param omega Rotação desejada no próprio eixo (Giro) [-1.0 a 1.0]
 */
void SSL_Robot_Move(float vx, float vy, float omega);

#endif // SSL_KINEMATICS_H
