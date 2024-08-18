#ifndef _SCENARIO_H
#define _SCENARIO_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include <map>

#include <iostream>
#include <unordered_map> // Troque std::map por std::unordered_map
#include <vector>
#include <glm/glm.hpp>   // Certifique-se de incluir a biblioteca GLM
#include <cstdlib>       // Para rand() e srand()
#include <ctime>         // Para time()
#include <functional>
#include <utility>
#include <random>

// Headers da biblioteca GLM: criação de matrizes e vetores.
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include"Matrices.h"
#include"Inputs.h"
#include"Obj.h"
#include"VirtualScene.h"

struct Tree
{
    glm::vec4 Position;
    glm::vec3 Scale;
};

class Scenario
{
public:
    float radius;
    int numTrees = 0;
    std::vector<std::vector<Tree>> treeMatrix;
    std::vector<Tree> treeVector;

    Scenario(float radius, float probability, VirtualScene &VirtualScene, Shader &GpuProgram);
    std::vector<Tree> getAdjascentTrees(glm::vec4 Position);
    void DrawTree(Tree tree, VirtualScene &VirtualScene, Shader &GpuProgram, int color);
    void DrawAllTrees(VirtualScene &VirtualScene, Shader &GpuProgram);
};

#endif