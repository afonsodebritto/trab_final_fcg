#include"Camera.h"

void Camera::Update(float screenRatio, Airplane airplane)
{
	Camera::screenRatio = screenRatio;
	LookAt = airplane.Position;
	Camera::distance = norm(airplane.Direction) * (airplane.speed/2 + 12) / 2;
	Camera::pitch = airplane.pitch;
	Camera::yaw = airplane.yaw;
	Camera::roll = airplane.roll;
}

void Camera::Matrix(float fov, float nearPlane, float farPlane, Shader& shader)
{
	Position = Matrix_Rotate_Y(-yaw) * Matrix_Rotate_X(-pitch/1.8) * Matrix_Rotate_Z(-roll/1.8) * glm::vec4(0.0f,1.0f,distance,0.0f) + LookAt;
	View = LookAt - Position;
	Up = Matrix_Rotate_Y(-yaw) * Matrix_Rotate_X(-pitch/1.8) * Matrix_Rotate_Z(-roll/1.8) * glm::vec4(0.0f,1.0f,-1.0f,0.0f);

	ViewMatrix = Matrix_Camera_View(Position, View, Up);		
	ProjectionMatrix = Matrix_Perspective(fov, screenRatio, nearPlane, farPlane);

	GLint view_uniform            = glGetUniformLocation(shader.ID, "view"); // Variável da matriz "view" em shader_vertex.glsl
    GLint projection_uniform      = glGetUniformLocation(shader.ID, "projection"); // Variável da matriz "projection" em shader_vertex.glsl
	// Exports the camera matrix to the Vertex Shader
	glUniformMatrix4fv(view_uniform, 1 , GL_FALSE , glm::value_ptr(ViewMatrix));
	glUniformMatrix4fv(projection_uniform , 1 , GL_FALSE , glm::value_ptr(ProjectionMatrix));
}