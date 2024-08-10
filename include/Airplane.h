#ifndef AIRPLANE_CLASS_H
#define AIRPLANE_CLASS_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

#include"Matrices.h"

class Airplane
{
public:
	// Stores the main vectors of the camera
	glm::vec4 Position;
    glm::vec4 Direction = glm::vec4(0.0f,0.0f,-1.0f,0.0f);
    glm::mat4 Matrix;

	float rotationAngleX;
	float rotationAngleZ;
    float speed = 0;
    float max_speed;
    float acceleration;
    float speedGravity = 0;


	Airplane(glm::vec4 position, float rotationAngleX, float rotationAngleZ, float acceleration, float max_speed);

    void Movimentation(bool accelerate, bool rotateRight, bool rotateLeft, bool rotateUp, bool rotateDown, bool brake, float deltaTime);

};
#endif