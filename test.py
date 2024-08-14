import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

# Parâmetros do avião
massa = 1000  # Massa do avião em kg
C_L = 1.2  # Coeficiente de sustentação
C_D = 0.3  # Coeficiente de arrasto
densidade_ar = 1.225  # Densidade do ar em kg/m^3 ao nível do mar
area_asa = 16.2  # Área da asa em m^2
gravidade = np.array([0, 0, -9.8]).astype(np.float64)  # Vetor gravidade em m/s^2 (apenas direção Z)
aceleracao_inicial = np.array([5, 0, 0]).astype(np.float64)  # Aceleração inicial em m/s^2 (direção X)

tempo_total = 50  # Tempo total da simulação em segundos
dt = 0.1  # Intervalo de tempo para cada passo da simulação em segundos
metade_tempo = tempo_total / 2  # Tempo em que o avião para de acelerar

# Condições iniciais
velocidade = np.array([0, 0, 0]).astype(np.float64)  # Velocidade inicial em m/s (X, Y, Z)
posicao = np.array([0, 0, 0]).astype(np.float64)  # Posição inicial em metros (X, Y, Z)
tempo = np.arange(0, tempo_total, dt).astype(np.float64)

# Listas para armazenar as posições ao longo do tempo
posicoes = []

# Simulação
for t in tempo:
    # Verifica se o tempo é maior que metade do tempo total, e para a aceleração
    if t <= metade_tempo:
        aceleracao = aceleracao_inicial
    elif aceleracao[0] > 2.5:
        aceleracao[0] = aceleracao[0] - 0.05
    
    # Atualiza a velocidade horizontal (X, Y)
    velocidade += aceleracao * dt
    
    # Calcula a velocidade total horizontal
    velocidade_horizontal = np.linalg.norm(velocidade[:2])
    
    # Calcula a força de sustentação (apenas direção Z)
    forca_sustentacao_z = 0.5 * C_L * densidade_ar * area_asa * velocidade_horizontal**2
    
    # Calcula a força da gravidade (já está incluída como um vetor)
    forca_gravidade_z = massa * gravidade[2]
    
    # Calcula a aceleração vertical (Z)
    aceleracao_z = (forca_sustentacao_z + forca_gravidade_z) / massa
    
    # Atualiza a velocidade na direção Z
    velocidade[2] += aceleracao_z * dt
    
    # Calcula a força de arrasto
    forca_arrasto = 0.5 * C_D * densidade_ar * area_asa * np.linalg.norm(velocidade)**2
    
    # Calcula a direção da força de arrasto (oposta à direção da velocidade)
    direcao_arrasto = -velocidade / np.linalg.norm(velocidade)
    
    # Aplica a força de arrasto à aceleração
    aceleracao_arrasto = forca_arrasto / massa * direcao_arrasto
    
    # Atualiza a velocidade considerando a força de arrasto
    velocidade += aceleracao_arrasto * dt
    
    # Atualiza a posição
    posicao += velocidade * dt
    
    # Não permite altura negativa (simulação do contato com o solo)
    if posicao[2] < 0:
        posicao[2] = 0
        velocidade[2] = 0  # Reseta a velocidade vertical ao tocar o solo
    
    # Armazena a posição
    posicoes.append(posicao.copy())

# Convertendo a lista de posições para um array NumPy para facilitar o plot
posicoes = np.array(posicoes)

# Plotando a trajetória do avião em 3D
fig = plt.figure(figsize=(10, 7))
ax = fig.add_subplot(111, projection='3d')
ax.plot(posicoes[:, 0], posicoes[:, 1], posicoes[:, 2], label='Trajetória do Avião')
ax.set_xlabel('Posição X (m)')
ax.set_ylabel('Posição Y (m)')
ax.set_zlabel('Altura Z (m)')
ax.set_title('Simulação da Trajetória do Avião em 3D')
ax.legend()
plt.show()
