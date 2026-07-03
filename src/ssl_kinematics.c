#include "ssl_kinematics.h"
#include <math.h>

static TIM_HandleTypeDef *esc_timer2;
static TIM_HandleTypeDef *esc_timer3;

void SSL_Motors_Init(TIM_HandleTypeDef *htim2, TIM_HandleTypeDef *htim3) {
    esc_timer2 = htim2;
    esc_timer3 = htim3;
    
    // Garante que todos os motores iniciem parados por segurança
    SSL_Motors_SetPulses(ESC_PULSE_NEUTRAL, ESC_PULSE_NEUTRAL, ESC_PULSE_NEUTRAL, ESC_PULSE_NEUTRAL);
    
    // Inicia a geração de PWM em cada canal do timer especificado
    HAL_TIM_PWM_Start(esc_timer2, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(esc_timer2, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(esc_timer2, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(esc_timer3, TIM_CHANNEL_3); // Motor 4
}

void SSL_Motors_SetPulses(uint16_t m1, uint16_t m2, uint16_t m3, uint16_t m4) {
    // Define o Compare Register de cada canal com o valor do pulso em microssegundos
    __HAL_TIM_SET_COMPARE(esc_timer2, TIM_CHANNEL_1, m1);
    __HAL_TIM_SET_COMPARE(esc_timer2, TIM_CHANNEL_2, m2);
    __HAL_TIM_SET_COMPARE(esc_timer2, TIM_CHANNEL_3, m3);
    __HAL_TIM_SET_COMPARE(esc_timer3, TIM_CHANNEL_3, m4);
}

void SSL_Robot_Move(float vx, float vy, float omega) {
    /*
     * Matriz de Cinemática para base Omnidirecional (4 Rodas a 45º)
     * Convenção: 
     * M1 = Frente-Direita
     * M2 = Frente-Esquerda
     * M3 = Trás-Esquerda
     * M4 = Trás-Direita
     */
     
    float v1 =  (vx * COS_45) + (vy * SIN_45) + omega;
    float v2 = -(vx * COS_45) + (vy * SIN_45) - omega;
    float v3 = -(vx * COS_45) - (vy * SIN_45) - omega;
    float v4 =  (vx * COS_45) - (vy * SIN_45) + omega;

    // Normalização: se algum valor extrapolar o limite (-1 a 1), reduzimos proporcionalmente todos
    float max_val = fmzaxf(fmaxf(fabsf(v1), fabsf(v2)), fmaxf(fabsf(v3), fabsf(v4)));
    if (max_val > 1.0f) {
        v1 /= max_val;
        v2 /= max_val;
        v3 /= max_val;
        v4 /= max_val;
    }

    /* 
     * Mapeamento de -1.0 a 1.0 para pulsos do ESC limitados (1350 a 1650us)
     * 0.0 é o neutro (1500us).
     * Multiplicamos a velocidade (que agora vai de -1 a 1) por 150 para limitar a velocidade.
     */
    uint16_t pulse_m1 = ESC_PULSE_NEUTRAL + (int16_t)(v1 * 150.0f);
    uint16_t pulse_m2 = ESC_PULSE_NEUTRAL + (int16_t)(v2 * 150.0f);
    uint16_t pulse_m3 = ESC_PULSE_NEUTRAL + (int16_t)(v3 * 150.0f);
    uint16_t pulse_m4 = ESC_PULSE_NEUTRAL + (int16_t)(v4 * 150.0f);

    SSL_Motors_SetPulses(pulse_m1, pulse_m2, pulse_m3, pulse_m4);
}
