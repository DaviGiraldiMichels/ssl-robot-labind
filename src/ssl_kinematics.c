#include "ssl_kinematics.h"
#include <math.h>

// Macro para silenciar avisos de variáveis não utilizadas durante os testes
// Apenas define UNUSED se não for definido por um cabeçalho do sistema
#ifndef UNUSED
  #define UNUSED(x) (void)(x)
#endif
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
     
    // Invertendo o sinal de v1 para corrigir a direção do Motor 1
    float v1 = -((vx * COS_45) - (vy * SIN_45) + omega);
    float v2 =  (vx * COS_45) + (vy * SIN_45) - omega;
    float v3 = -(vx * COS_45) + (vy * SIN_45) - omega;
    float v4 = -(vx * COS_45) - (vy * SIN_45) + omega;

    // Normalização: se algum valor extrapolar o limite (-1 a 1), reduzimos proporcionalmente todos
    float max_val = fmaxf(fmaxf(fabsf(v1), fabsf(v2)), fmaxf(fabsf(v3), fabsf(v4)));
    if (max_val > 1.0f) {
        v1 /= max_val;
        v2 /= max_val;
        v3 /= max_val;
        v4 /= max_val;
    }

    /*
     * Mapeamento de -1.0 a 1.0 para pulsos do ESC com compensação de Deadzone.
     * Com base nos testes, o neutro real é ~1490us e a deadzone vai de 1465 a 1515.
     */
    #define REAL_NEUTRAL 1490
    #define DEADZONE_OFFSET 60 // Ponto onde o motor começa a girar (1490 + 25 = 1515)
    #define KICK_START_OFFSET 200 // "Chute" extra para vencer o atrito (1490 + 80 = 1570)
    #define GAIN 150.0f

    int16_t dev1 = (int16_t)(v1 * GAIN);
    int16_t dev2 = (int16_t)(v2 * GAIN);
    int16_t dev3 = (int16_t)(v3 * GAIN);
    int16_t dev4 = (int16_t)(v4 * GAIN);
    
    // Variáveis para guardar o estado anterior de cada motor (0=parado, 1=frente, -1=ré)
    static int8_t last_state[4] = {0};

    // Função auxiliar para aplicar a lógica de Kick-Start
    int16_t apply_kick_start(int16_t current_dev, int8_t *last_motor_state) {
        int8_t current_state = (current_dev > 0) ? 1 : ((current_dev < 0) ? -1 : 0);
        
        // Se o motor estava parado e agora vai se mover...
        if (*last_motor_state == 0 && current_state != 0) {
            // ...aplica o "chute" inicial
            if (current_state > 0) return current_dev + KICK_START_OFFSET;
            else return current_dev - KICK_START_OFFSET;
        } else {
            // ...senão, aplica apenas a compensação normal da deadzone
            if (current_state > 0) return current_dev + DEADZONE_OFFSET;
            else if (current_state < 0) return current_dev - DEADZONE_OFFSET;
            else return 0; // Garante que o motor pare em 0
        }
        *last_motor_state = current_state; // Atualiza o estado para a próxima chamada
    }

    uint16_t pulse_m1 = REAL_NEUTRAL + apply_kick_start(dev1, &last_state[0]);
    uint16_t pulse_m2 = REAL_NEUTRAL + apply_kick_start(dev2, &last_state[1]);
    uint16_t pulse_m3 = REAL_NEUTRAL + apply_kick_start(dev3, &last_state[2]);
    uint16_t pulse_m4 = REAL_NEUTRAL + apply_kick_start(dev4, &last_state[3]);

    // Silencia os avisos de 'variável não utilizada' para os motores que não estão sendo testados
    // UNUSED(pulse_m1);
    // UNUSED(pulse_m2);
    // UNUSED(pulse_m4);

    /* --- MODO DE TESTE PARA UM ÚNICO MOTOR ---
     * Para testar um motor de cada vez, comente a linha original e descomente
     * uma das linhas abaixo. Apenas o motor especificado se moverá.
     */
    // SSL_Motors_SetPulses(pulse_m1, ESC_PULSE_NEUTRAL, ESC_PULSE_NEUTRAL, ESC_PULSE_NEUTRAL); // Testa apenas o Motor 1
    // SSL_Motors_SetPulses(ESC_PULSE_NEUTRAL, pulse_m2, ESC_PULSE_NEUTRAL, ESC_PULSE_NEUTRAL); // Testa apenas o Motor 2
    // SSL_Motors_SetPulses(ESC_PULSE_NEUTRAL, ESC_PULSE_NEUTRAL, pulse_m3, ESC_PULSE_NEUTRAL); // Testa apenas o Motor 3
    // SSL_Motors_SetPulses(ESC_PULSE_NEUTRAL, ESC_PULSE_NEUTRAL, ESC_PULSE_NEUTRAL, pulse_m4); // Testa apenas o Motor 4
    SSL_Motors_SetPulses(pulse_m1, pulse_m2, pulse_m3, pulse_m4); // Linha original para operação normal

}
