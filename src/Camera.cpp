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
	if(initializationLookatCamera)
		CreateBezierCurve(), movingThroughBezierCurve = true, initializationLookatCamera = false;
}

void Camera::FreeUpdate(float screenRatio, Inputs inputs)
{
	Camera::inputs = inputs;
	Camera::screenRatio = screenRatio;
	initializationLookatCamera = true;
	if(initializationFreeCamera)
	{
		float r = norm(View);
		Camera::pitch = acos(-View.y/r) - M_PI/2;
		Camera::yaw = -atan2(View.z, View.x) + M_PI/2;
		Camera::roll = airplane->roll;
		Camera::lastCursorXPos = inputs.cursorXPos;
		Camera::lastCursorYPos = inputs.cursorYPos;
		initializationFreeCamera = false;
	}
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
	if(movingThroughBezierCurve)
		Position = MoveThroughBezier(deltaTime);
	else
		Position = Matrix_Rotate_Y(airplane->yaw)
				   * Matrix_Rotate_X(airplane->pitch/1.8)
				   * Matrix_Rotate_Z(airplane->roll/1.8)
				   * glm::vec4(0.0f,1.0f,norm(airplane->Direction) * (airplane->speed/2 + 12) / 2,0.0f)
				   + airplane->Position;

	View = airplane->Position - Position;
	Up = Matrix_Rotate_Y(airplane->yaw)
	     * Matrix_Rotate_X(airplane->pitch/1.8)
		 * Matrix_Rotate_Z(airplane->roll/1.8)
		 * glm::vec4(0.0f,1.0f,0.0f,0.0f);
}

glm::vec4 Camera::MoveThroughBezier(float deltaTime)
{
	bezierTime += deltaTime/bezierDistance*20;

    if (bezierTime > 1.0f)
    {
        bezierTime = 1.0f;
        movingThroughBezierCurve = false;
    }

    // Calcular a posição ao longo da curva de Bézier
    glm::vec4 newPosition = (float)pow((1 - bezierTime), 3) * bezierInicial
               				+ 3.0f * (float)pow((1 - bezierTime), 2) * bezierTime * bezierP1
               				+ 3.0f * (1 - bezierTime) * (float)pow(bezierTime, 2) * bezierP2
               				+ (float)pow(bezierTime, 3) * bezierFinal;
    newPosition.w = 1.0f;

	return newPosition;
}


void Camera::CreateBezierCurve()
{
	float distance = norm(airplane->Direction) * (airplane->speed/2 + 12) / 2;
	bezierFinal = Matrix_Rotate_Y(airplane->yaw)
	              * Matrix_Rotate_X(airplane->pitch/1.8)
				  * Matrix_Rotate_Z(airplane->roll/1.8)
				  * glm::vec4(0.0f, 1.0f, norm(airplane->Direction) * (airplane->speed/2 + 12) / 2, 0.0f)
				  + airplane->Position;
	bezierInicial = Position;

	glm::vec4 v1 = crossproduct(airplane->Direction, View);
	if(v1 == glm::vec4(0.0f, 0.0f, 0.0f, 0.0f))
		v1 = crossproduct(airplane->Direction, glm::vec4(View.x+0.1f,View.y+0.1f,View.z+0.1f,View.w));
	v1 /= norm(v1);
	glm::vec4 v2 = -v1;
	
	glm::vec4 bezierDirection = bezierFinal - bezierInicial;
	bezierDistance = fabs(norm(bezierDirection));

	bezierP1 = bezierInicial + bezierDirection / 3.0f + v1 * bezierDistance * 2.0f;
	bezierP2 = bezierInicial + 2.0f * bezierDirection / 3.0f + v2 * bezierDistance;

	if(bezierP1.y < 0)
		bezierP1.y = 0.0f;
	if(bezierP2.y < 0)
		bezierP2.y = 0.0f;

	bezierTime = 0.0f;
}

void Camera::FreeMatrix(float fov, float nearPlane, float farPlane, Shader& shader, float deltaTime)
{
	View = RotateView(deltaTime);
	Position = ChangePosition(deltaTime);
	Up = glm::vec4(0.0f,1.0f,0.0f,0.0f);

	if(roll != 0)
		Up = Matrix_Rotate_Y(airplane->yaw)
		   * Matrix_Rotate_X(airplane->pitch/1.8)
		   * Matrix_Rotate_Z(roll/1.8 )
		   * Up;
}

glm::vec4 Camera::RotateView(float deltaTime)
{
	float dx = inputs.cursorXPos - lastCursorXPos;
    float dy = inputs.cursorYPos - lastCursorYPos;

    yaw -= deltaTime*dx/2;
    pitch -= deltaTime*dy/2;

    float pitchMax = M_PI/2;
    float pitchMin = -pitchMax;

    if (pitch > pitchMax)
        pitch = pitchMax;

    if (pitch < pitchMin)
        pitch = pitchMin;

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

	yaw = fmod(yaw, 2 * M_PI);
    if (yaw > M_PI) yaw -= 2 * M_PI;
    else if (yaw < -M_PI) yaw += 2 * M_PI;

	lastCursorXPos = inputs.cursorXPos;
	lastCursorYPos = inputs.cursorYPos;

	float y = sin(pitch);
	float z = cos(pitch)*cos(yaw);
	float x = cos(pitch)*sin(yaw);

	glm::vec4 newView = glm::vec4(x,y,z,0.0f);
   
	return newView;
}

glm::vec4 Camera::ChangePosition(float deltaTime)
{
	float speed = 10 * deltaTime;

	glm::vec4 w = -View;
    glm::vec4 u = crossproduct(Up,w);
    w /= norm(w);
    u /= norm(u);
	glm::vec4 newPosition = Position;

	if(inputs.keyPressedW)
		newPosition -= w*speed;
	if(inputs.keyPressedA)
		newPosition -= u*speed;
	if(inputs.keyPressedS)
		newPosition += w*speed;
	if(inputs.keyPressedD)
		newPosition += u*speed;
	
	if(newPosition.y < 0)
		newPosition.y = 0;

	return newPosition;
}