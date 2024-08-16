#ifndef _VIRTUALSCENE_H
#define _VIRTUALSCENE_H


#include "shaderClass.h"
// Headers abaixo são específicos de C++
#include <string>
#include <vector>
#include <stdexcept>
#include <map>

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>  // Criação de janelas do sistema operacional

#include "Obj.h"

struct SceneObject
{
    std::string  name;        // Nome do objeto
    size_t       first_index; // Índice do primeiro vértice dentro do vetor indices[] definido em BuildTrianglesAndAddToVirtualScene()
    size_t       num_indices; // Número de índices do objeto dentro do vetor indices[] definido em BuildTrianglesAndAddToVirtualScene()
    GLenum       rendering_mode; // Modo de rasterização (GL_TRIANGLES, GL_TRIANGLE_STRIP, etc.)
    GLuint       vertex_array_object_id; // ID do VAO onde estão armazenados os atributos do modelo
    glm::vec3    bbox_min; // Axis-Aligned Bounding Box do objeto
    glm::vec3    bbox_max;
};

class VirtualScene
{
    public:

        std::map<std::string, SceneObject> Objects;

        void DrawVirtualObject(std::string object_name, Shader &GpuProgram);
        void BuildTriangles(ObjModel &model);

};

#endif