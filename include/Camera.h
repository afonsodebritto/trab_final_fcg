#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"shaderClass.h"
#include"Matrices.h"
#include"Airplane.h"
#include"Inputs.h"

#define FREE_CAMERA 1
#define LOOKAT_CAMERA 0

class Camera
{
public:
	// Stores the main vectors of the camera
	glm::vec4 Position;
	glm::vec4 Up;
	glm::vec4 View;
	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;

	float screenRatio;
	Airplane *airplane;

	bool type;

	bool initializationFreeCamera = true;
	bool initializationLookatCamera = false;
	bool movingThroughBezierCurve = false;
	float cursorXPos;
	float cursorYPos;
	float lastCursorXPos;
	float lastCursorYPos;
	float yaw;
	float pitch;
	float roll;

	Inputs inputs;

	// Bezier Variables

	glm::vec4 bezierInicial;
	glm::vec4 bezierFinal;
	glm::vec4 bezierP1;
	glm::vec4 bezierP2;
	float bezierTime = 0.0f;
	float bezierDistance;

	void Update(float screenRatio, Airplane& airplane, Inputs inputs, bool cameraType);
	void Matrix(float fov, float nearPlane, float farPlane, Shader& shader, float deltaTime);
	
private:
	void LookatUpdate(float screenRatio, Airplane& airplane);
	void FreeUpdate(float screenRatio, Inputs inputs);

	void LookatMatrix(float fov, float nearPlane, float farPlane, Shader& shader, float deltaTime);
	void FreeMatrix(float fov, float nearPlane, float farPlane, Shader& shader, float deltaTime);

	// Free Camera Methods
	glm::vec4 RotateView(float deltaTime); 
	glm::vec4 ChangePosition(float deltaTime);

	void CreateBezierCurve();
	glm::vec4 MoveThroughBezier(float deltaTime);
};
#endif