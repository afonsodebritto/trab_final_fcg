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
#include <GLFW/glfw3.h>

#include "Obj.h"

struct SceneObject
{
    std::string  name;
    size_t       first_index;
    size_t       num_indices;
    GLenum       rendering_mode;
    GLuint       vertex_array_object_id;
    glm::vec3    bbox_min;
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