import math
import numpy as np
import matplotlib.pyplot as plt

def densidade_ar(altitude):
    # Constantes
    rho_0 = 1.225  # Densidade do ar ao nível do mar (kg/m^3)
    T_0 = 288.15   # Temperatura ao nível do mar (K)
    L = 0.0065     # Taxa de lapso de temperatura (K/m)
    g = 9.80665    # Aceleração da gravidade (m/s^2)
    R = 287.05     # Constante específica do ar (J/(kg*K))

    # Cálculo da densidade
    T = T_0 - L * altitude
    expoente = (g / (R * L)) - 1
    rho = rho_0 * (T / T_0) ** expoente

    return rho

# Criar uma série de altitudes de 0 a 20000 metros
altitudes = np.linspace(0, 20000, 1000)

# Calcular as densidades correspondentes
densidades = [densidade_ar(alt) for alt in altitudes]

# Criar o gráfico
plt.figure(figsize=(10, 6))
plt.plot(altitudes, densidades)
plt.title('Densidade do Ar vs. Altitude')
plt.xlabel('Altitude (m)')
plt.ylabel('Densidade do Ar (kg/m³)')
plt.grid(True)

# Adicionar algumas anotações
plt.annotate('Nível do Mar', xy=(0, densidade_ar(0)), xytext=(1000, 1.1),
             arrowprops=dict(facecolor='black', shrink=0.05))

plt.annotate('Altitude de Cruzeiro\ntípica de aviões', xy=(10000, densidade_ar(10000)), 
             xytext=(12000, 0.6), arrowprops=dict(facecolor='black', shrink=0.05))

plt.show()