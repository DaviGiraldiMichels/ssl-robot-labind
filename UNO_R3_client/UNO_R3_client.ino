#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN

const byte address[5] = {'0', '0', '0', '0', '1'};

// Estrutura idêntica à do receptor
struct RobotCommand {
  float vx;    // -1.0 a 1.0
  float vy;    // -1.0 a 1.0
  float omega; // -1.0 a 1.0
};

RobotCommand command = {0.0f, 0.0f, 0.0f};

void setup() {
  Serial.begin(115200); // Maior velocidade para baixa latência
  Serial.println("Arduino Iniciado e Aguardando controle...");
  
  pinMode(LED_BUILTIN, OUTPUT);
  
  // Blink de inicialização
  for(int i=0; i<3; i++) {
    digitalWrite(LED_BUILTIN, HIGH); delay(100);
    digitalWrite(LED_BUILTIN, LOW);  delay(100);
  }

  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.setChannel(120);
  radio.setDataRate(RF24_1MBPS);
  // Habilita o Auto-Acknowledgement real. Delay de 5*250us, 15 tentativas.
  radio.setRetries(5, 15);
  radio.stopListening();
}

void loop() {
  // Se houver dados da Serial (vidos do script do PC), atualiza o comando
  if (Serial.available() >= 14) { // 2 bytes de sincronia + 12 bytes da struct
    uint8_t h1 = Serial.read();
    if (h1 == 0xAA) {
      uint8_t h2 = Serial.read();
      if (h2 == 0x55) {
        Serial.readBytes((char*)&command, sizeof(RobotCommand));
        
        bool ack = radio.write(&command, sizeof(RobotCommand));
        
        Serial.print("VX: ");
        Serial.print(command.vx);
        Serial.print(" | VY: ");
        Serial.print(command.vy);
        Serial.print(" | OMEGA: ");
        Serial.print(command.omega);
        
        if (ack) {
          Serial.println("  [ACK: OK]");
        } else {
          Serial.println("  [ACK: FALHA]");
        }
      }
    }
  }
}