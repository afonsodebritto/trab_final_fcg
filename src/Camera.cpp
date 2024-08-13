#include"Camera.h"
#include"Matrices.h"

void Camera::Update(float screenRatio, Airplane& airplane, Inputs inputs, bool cameraType)
{
	Camera::type = cameraType;
	if(cameraType == LOOKAT_CAMERA)
		LookatUpdate(screenRatio, airplane);
	else if(cameraType == FREE_CAMERA)
		FreeUpdate(screenRatio, inputs);
}

void Camera::LookatUpdate(float screenRatio, Airplane& airplane)
{
	Camera::screenRatio = screenRatio;
	Camera::airplane = &airplane;
	initializationFreeCamera = true;
}

void Camera::FreeUpdate(float screenRatio, Inputs inputs)
{
	initializationLookatCamera = true;
	if(initializationFreeCamera)
	{
		float r = norm(View);
		Camera::yaw = acos(View.y/r) -M_PI/2;
		Camera::pitch = atan2(-View.z, View.x) - M_PI/2;
		Camera::roll = airplane->roll;
		Camera::cursorXPos = inputs.cursorXPos;
		Camera::cursorYPos = inputs.cursorYPos;
		initializationFreeCamera = false;
	}
	Camera::lastCursorXPos = Camera::cursorXPos;
	Camera::lastCursorYPos = Camera::cursorYPos;
	Camera::cursorXPos = cursorXPos;
	Camera::cursorYPos = cursorYPos;
	Camera::inputs = inputs;
}

void Camera::Matrix(float fov, float nearPlane, float farPlane, Shader& shader, float deltaTime)
{
	if(type == LOOKAT_CAMERA)
		LookatMatrix(fov, nearPlane, farPlane, shader, deltaTime);
	else if(type == FREE_CAMERA)
		FreeMatrix(fov, nearPlane, farPlane, shader, deltaTime);

	ViewMatrix = Matrix_Camera_View(Position, View, Up);		
	ProjectionMatrix = Matrix_Perspective(fov, screenRatio, nearPlane, farPlane);

	GLint view_uniform            = glGetUniformLocation(shader.ID, "view"); // Variável da matriz "view" em shader_vertex.glsl
    GLint projection_uniform      = glGetUniformLocation(shader.ID, "projection"); // Variável da matriz "projection" em shader_vertex.glsl
	// Exports the camera matrix to the Vertex Shader
	glUniformMatrix4fv(view_uniform, 1 , GL_FALSE , glm::value_ptr(ViewMatrix));
	glUniformMatrix4fv(projection_uniform , 1 , GL_FALSE , glm::value_ptr(ProjectionMatrix));
}


void Camera::LookatMatrix(float fov, float nearPlane, float farPlane, Shader& shader, float deltaTime)
{
	float distance = norm(airplane->Direction) * (airplane->speed/2 + 12) / 2;
	Position = Matrix_Rotate_Y(airplane->yaw) * Matrix_Rotate_X(airplane->pitch/1.8) * Matrix_Rotate_Z(airplane->roll/1.8) * glm::vec4(0.0f,1.0f,distance,0.0f) + airplane->Position;
	View = airplane->Position - Position;
	Up = Matrix_Rotate_Y(airplane->yaw) * Matrix_Rotate_X(airplane->pitch/1.8) * Matrix_Rotate_Z(airplane->roll/1.8) * glm::vec4(0.0f,1.0f,0.0f,0.0f);
}

void Camera::FreeMatrix(float fov, float nearPlane, float farPlane, Shader& shader, float deltaTime)
{
	float cameraSpeed = 10 * deltaTime;

	float y = sin(yaw);
	float z = cos(yaw)*cos(pitch);
	float x = cos(yaw)*sin(pitch);

	View = -glm::vec4(x,y,z,0.0f);

	UpdateAngles(deltaTime);

	if(roll != 0)
		Up = Matrix_Rotate_Y(airplane->yaw) * Matrix_Rotate_X(airplane->pitch/1.8) * Matrix_Rotate_Z(roll/1.8 ) * glm::vec4(0.0f,1.0f,0.0f,0.0f);
	else
		Up = glm::vec4(0.0f,1.0f,0.0f,0.0f);

	UpdatePosition(deltaTime);
}

void Camera::UpdateAngles(float deltaTime)
{
	float dx = cursorXPos - lastCursorXPos;
    float dy = cursorYPos - lastCursorYPos;

    pitch -= deltaTime*dx/2;
    yaw   += deltaTime*dy/2;

    float yawmax = M_PI/2;
    float yawmin = -yawmax;

    if (yaw > yawmax)
        yaw = yawmax;

    if (yaw < yawmin)
        yaw = yawmin;

	if(roll > 0)
	{
		roll -= deltaTime * (fabs(roll) + 1);
		if(roll < 0) roll = 0;
	}
	else if(roll < 0)
	{
		roll += deltaTime * (fabs(roll) + 1);
		if(roll > 0) roll = 0;
	}

	pitch = fmod(pitch, 2 * M_PI);
    if (pitch > M_PI) pitch -= 2 * M_PI;
    else if (pitch < -M_PI) pitch += 2 * M_PI;
}

void Camera::UpdatePosition(float deltaTime)
{
	float speed = 10 * deltaTime;
	printf("oi");

	glm::vec4 w = -View;
    glm::vec4 u = crossproduct(Up,w);
    w /= norm(w);
    u /= norm(u);

	if(inputs.keyPressedW)
		Position -= w*speed;
	if(inputs.keyPressedA)
		Position -= u*speed;
	if(inputs.keyPressedS)
		Position += w*speed;
	if(inputs.keyPressedD)
		Position += u*speed;
}