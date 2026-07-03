import pygame
import serial
import struct
import time
import serial.tools.list_ports

# --- CONFIGURAÇÕES ---
# No Linux geralmente é /dev/ttyUSB0 ou /dev/ttyACM0. No Windows é 'COMx'
SERIAL_PORT = '/dev/ttyACM0' 
BAUD_RATE = 115200
SEND_FREQ = 50  # Frequência de envio em Hz (50Hz = 20ms, ideal para SSL)

def main():
    # Inicializa o Pygame e o Joystick
    pygame.init()
    pygame.joystick.init()

    if pygame.joystick.get_count() == 0:
        print("Erro: Nenhum controle encontrado!")
        return

    joystick = pygame.joystick.Joystick(0)
    joystick.init()
    print(f"Controle conectado: {joystick.get_name()}")

    # Inicializa a conexão Serial
    try:
        ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=0.1)
        print(f"Conectado ao Arduino na porta {SERIAL_PORT}")
        time.sleep(2)  # Aguarda o reset automático do Arduino ao abrir a porta
    except Exception as e:
        print(f"\nErro ao abrir porta serial {SERIAL_PORT}: {e}")
        print("\nPortas seriais disponíveis no seu sistema:")
        ports = serial.tools.list_ports.comports()
        for port in ports:
            print(f"- {port.device} ({port.description})")
        print("\nVerifique se o cabo do ARDUINO está bom e ajuste a variável SERIAL_PORT no script.")
        return

    clock = pygame.time.Clock()
    run = True

    print("Enviando comandos... Pressione Ctrl+C para parar.")

    try:
        while run:
            # Verifica se o controle ainda está conectado
            if pygame.joystick.get_count() == 0:
                print("Controle desconectado!")
                break

            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    run = False

            # Captura dos eixos (Mapeamento padrão Xbox/PS4)
            # Axis 1: Left Stick Y (Frente/Trás) -> Invertemos pois 'cima' no pygame é negativo
            # Axis 0: Left Stick X (Esquerda/Direita)
            # Axis 3: Right Stick X (Giro/Omega)
            vx = -joystick.get_axis(1) 
            vy = joystick.get_axis(0)
            omega = -joystick.get_axis(3)

            # Deadzone simples para evitar drift (pequenos movimentos involuntários)
            if abs(vx) < 0.1: vx = 0.0
            if abs(vy) < 0.1: vy = 0.0
            if abs(omega) < 0.1: omega = 0.0

            # --- Cálculo Espelho da Cinemática ---
            # Replica a lógica do STM32 (ssl_kinematics.c) para exibir no terminal
            COS_45 = 0.7071
            SIN_45 = 0.7071
            
            v1 =  (vx * COS_45) + (vy * SIN_45) + omega
            v2 = -(vx * COS_45) + (vy * SIN_45) - omega
            v3 = -(vx * COS_45) - (vy * SIN_45) - omega
            v4 =  (vx * COS_45) - (vy * SIN_45) + omega

            max_val = max(abs(v1), abs(v2), abs(v3), abs(v4))
            if max_val > 1.0:
                v1 /= max_val
                v2 /= max_val
                v3 /= max_val
                v4 /= max_val

            m1_pwm = 1500 + int(v1 * 125.0)
            m2_pwm = 1500 + int(v2 * 125.0)
            m3_pwm = 1500 + int(v3 * 125.0)
            m4_pwm = 1500 + int(v4 * 125.0)

            # Empacota com Cabeçalho de Sincronia (0xAA, 0x55)
            # '<BBfff' significa: < (Little Endian), 2 unsigned chars, 3 floats = 14 bytes
            packet = struct.pack('<BBfff', 0xAA, 0x55, float(vx), float(vy), float(omega))
            
            ser.write(packet)

            # Lê a resposta do Arduino e imprime junto com os valores lidos pelo Python
            while ser.in_waiting > 0:
                try:
                    linha_arduino = ser.readline().decode('utf-8').strip()
                    if linha_arduino:
                        # Formata a saída para mostrar os dados do Python e a resposta do Arduino
                        py_telemetry = f"PY [VX:{vx:5.2f} VY:{vy:5.2f} W:{omega:5.2f}] | PWM [M1:{m1_pwm:4} M2:{m2_pwm:4} M3:{m3_pwm:4} M4:{m4_pwm:4}]"
                        print(f"{py_telemetry} ==> ARD: {linha_arduino}", end='\r')
                except Exception:
                    pass # Ignora erros de decodificação de caracteres soltos

            # Garante a frequência de 50Hz
            clock.tick(SEND_FREQ)

    except KeyboardInterrupt:
        print("\nEncerrando...")
    finally:
        ser.close()
        pygame.quit()

if __name__ == "__main__":
    main()