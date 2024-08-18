#ifndef AIRPLANE_CLASS_H
#define AIRPLANE_CLASS_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include <map>

// Headers da biblioteca GLM: criação de matrizes e vetores.
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include"Matrices.h"
#include"Inputs.h"
#include"Obj.h"
#include"VirtualScene.h"

#define FUSELAGE 0
#define WHEEL_LEFT  1
#define WHEEL_RIGHT  2
#define ROTOR 3
#define TREE 4

class Airplane
{
public:
	// Stores the main vectors of the camera
	glm::vec4 Position;
    glm::vec4 Direction = glm::vec4(0.0f,0.0f,-1.0f,0.0f);
    glm::mat4 Matrix;

    // Variávels para movimentação do avião
	float pitch = 0.0f;
	float yaw = 0.0f;
    float roll = 0.0f;
    float speed = 0.0f;
    float speedGravity = 0.0f;
    float acceleration;
    float max_speed;
    float rotorSpeed = 0;

    // Variáveis do ambiente
    float air_density = 1.225f;       // Densidade do ar ao nível do mar (kg/m^3)
    const float gravity = 9.81f;            // Aceleração devido à gravidade (m/s^2)

    // Constantes do avião
    const float drag_coefficient = 0.025f;   // Coeficiente de arrasto ajustado para maior realismo
    const float lift_coefficient = 1.2f;    // Coeficiente de sustentação básico
    const float reference_area = 16.2f;     // Área frontal de referência do avião (m^2)
    const float mass = 743.0f;               // Massa do avião (kg)


	Airplane(glm::vec4 position, float acceleration, float max_speed);
    void UpdateRotation(bool rotateRight, bool rotateLeft, bool rotateUp, bool rotateDown, float delta_time);
    void UpdateSpeed(bool accelerate, bool brake, float delta_time);
    void UpdateAirDensity();
    

    void Movimentation(Inputs inputs, float delta_time);
    void Draw(VirtualScene &VirtualScene, Shader &GpuProgram, float delta_time);
};
#endif