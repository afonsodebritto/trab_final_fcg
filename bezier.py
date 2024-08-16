import numpy as np

def normalize(v):
    norm = np.linalg.norm(v)
    if norm == 0:
        return v
    return v / norm

def calculate_bezier_curve(pv, p1, p2, pd, num_points=100):
    t_values = np.linspace(0, 1, num_points)
    bezier_points = []
    
    for t in t_values:
        point = (1-t)**3 * pv + 3 * (1-t)**2 * t * p1 + 3 * (1-t) * t**2 * p2 + t**3 * pd
        bezier_points.append(point)
        
    return np.array(bezier_points)

def create_bezier_curve(v, d, pv, pd, num_points=100):
    # Verificando se v e d são linearmente dependentes
    if np.allclose(np.cross(v, d), [0, 0, 0]):
        v = v + 0.1
    
    # Calculando os produtos vetoriais
    vd = np.cross(v, d)
    dv = -vd
    
    # Normalizando os vetores vd e dv
    vd = normalize(vd)
    dv = normalize(dv)
    
    # Calculando a distância entre pv e pd
    distance = np.linalg.norm(pd - pv)
    
    # Escalando os vetores vd e dv pela distância
    vd = vd * distance/2
    dv = dv * distance/2
    
    # Calculando a reta entre pv e pd
    direction = pd - pv
    p1 = pv + direction / 3
    p2 = pv + 2 * direction / 3
    
    # Deslocando p1 e p2 pelos vetores vd e dv
    p1 = p1 + vd
    p2 = p2 + dv
    
    # Calculando a curva de Bézier
    bezier_curve = calculate_bezier_curve(pv, p1, p2, pd, num_points)
    
    return bezier_curve

# Exemplo de uso
v = np.array([1, 2, 3])
d = np.array([1, 2, 3])
pd = np.array([0, 0, 0])
pv = np.array([10, 10, 10])

bezier_curve = create_bezier_curve(v, d, pv, pd, num_points=100)

# Agora você pode, por exemplo, plotar a curva
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.plot(bezier_curve[:,0], bezier_curve[:,1], bezier_curve[:,2])
ax.scatter([pv[0], pd[0]], [pv[1], pd[1]], [pv[2], pd[2]], color='red')  # Marcando os pontos pv e pd

ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')
plt.show()
