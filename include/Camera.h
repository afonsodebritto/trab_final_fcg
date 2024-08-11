#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"shaderClass.h"
#include"Matrices.h"
#include"Airplane.h"

class Camera
{
public:
	// Stores the main vectors of the camera
	glm::vec4 Position;
	glm::vec4 LookAt;
	glm::vec4 Up;
	glm::vec4 View;
	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;

	// Stores the width and height of the window
	float screenRatio;
	float distance;
	float pitch;
	float roll;
	float yaw;



	void Update(float screenRatio, Airplane airplane);
	// Updates and exports the camera matrix to the Vertex Shader
	void Matrix(float fov, float nearPlane, float farPlane, Shader& shader);
};
#endif