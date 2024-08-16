#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include<glad/glad.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>

std::string get_file_contents(const char* filename);

class Shader
{
public:
	// Reference ID of the Shader Program
	GLuint ID = 0;
	GLuint model_uniform;
    GLuint view_uniform;
    GLuint projection_uniform;
    GLuint object_id_uniform;
    GLuint bbox_min_uniform;
    GLuint bbox_max_uniform;
	
	// Constructor that build the Shader Program from 2 different shaders
	Shader(const char* vertexFile, const char* fragmentFile);
	GLuint CreateGpuProgram(GLuint vertex_shader_id, GLuint fragment_shader_id);

	// Activates the Shader Program
	void Activate();
	// Deletes the Shader Program
	void Delete();

private:	
	void Load(const char* filename, GLuint shader_id);
};
#endif