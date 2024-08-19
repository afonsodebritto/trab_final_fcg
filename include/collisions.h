#ifndef COLISIONS
#define COLISIONS

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>

// Headers abaixo são específicos de C++
#include <map>
#include <stack>
#include <string>
#include <vector>
#include <limits>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>

// Headers das bibliotecas OpenGL
#include <glad/glad.h>   // Criação de contexto OpenGL 3.3
#include <GLFW/glfw3.h>  // Criação de janelas do sistema operacional

// Headers da biblioteca GLM: criação de matrizes e vetores.
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

// Headers da biblioteca para carregar modelos obj
#include <tiny_obj_loader.h>

#include <stb_image.h>

bool cube_cube_intersec(glm::vec3 min_a, glm::vec3 max_a, glm::vec3 min_b, glm::vec3 max_b);
std::pair<glm::vec3, glm::vec3> transformAABB(const glm::vec3& min, const glm::vec3& max, const glm::mat4& transform);
bool cube_origincircle_intersec(glm::vec3 min, glm::vec3 max, float radius);
bool cube_elipsoid_intersec(glm::vec3 min, glm::vec3 max, float radius_x, float radius_y, float radius_z);

#endif