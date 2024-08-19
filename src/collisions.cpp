#include "collisions.h"
bool cube_cube_intersec(glm::vec3 min_a, glm::vec3 max_a, glm::vec3 min_b, glm::vec3 max_b)
{
    // // Imprime os valores de debug
    // std::cout << "Cubo A: min(" << min_a.x << ", " << min_a.y << ", " << min_a.z << "), max(" << max_a.x << ", " << max_a.y << ", " << max_a.z << ")" << std::endl;
    // std::cout << "Cubo B: min(" << min_b.x << ", " << min_b.y << ", " << min_b.z << "), max(" << max_b.x << ", " << max_b.y << ", " << max_b.z << ")" << std::endl;

    // Verifica a sobreposição em cada eixo
    bool colisao = (min_a.x <= max_b.x && max_a.x >= min_b.x &&
                     min_a.y <= max_b.y && max_a.y >= min_b.y &&
                     min_a.z <= max_b.z && max_a.z >= min_b.z);

    // // Imprime o resultado da verificação
    // std::cout << "Colidindo em X: " << (min_a.x <= max_b.x && max_a.x >= min_b.x) << std::endl;
    // std::cout << "Colidindo em Y: " << (min_a.y <= max_b.y && max_a.y >= min_b.y) << std::endl;
    // std::cout << "Colidindo em Z: " << (min_a.z <= max_b.z && max_a.z >= min_b.z) << std::endl;

    return colisao;
}

std::pair<glm::vec3, glm::vec3> transformAABB(const glm::vec3& min, const glm::vec3& max, const glm::mat4& transform)
{
    // Vértices da AABB
    std::vector<glm::vec3> corners = {
        glm::vec3(min.x, min.y, min.z),
        glm::vec3(min.x, min.y, max.z),
        glm::vec3(min.x, max.y, min.z),
        glm::vec3(min.x, max.y, max.z),
        glm::vec3(max.x, min.y, min.z),
        glm::vec3(max.x, min.y, max.z),
        glm::vec3(max.x, max.y, min.z),
        glm::vec3(max.x, max.y, max.z)
    };

    // Aplicar a matriz de transformação a cada vértice
    for (auto& corner : corners) {
        glm::vec4 transformed = transform * glm::vec4(corner, 1.0f);
        corner = glm::vec3(transformed);
    }

    // Encontrar o AABB transformado
    glm::vec3 new_min = corners[0];
    glm::vec3 new_max = corners[0];

    for (const auto& corner : corners) {
        new_min = glm::min(new_min, corner); // Encontra mínimos por eixo
        new_max = glm::max(new_max, corner); // Encontra máximos por eixo
    }

    return std::make_pair(new_min, new_max);
}

bool cube_origincircle_intersec(glm::vec3 min, glm::vec3 max, float radius)
{
    printf("%f\n", min.y);
    printf("%f\n", max.y);
    if((min.y < 0.0f || max.y < 0.0f) && (min.y > -0.1f || max.y > -0.1f))
    {
        float distanceFromCenterMin = std::sqrt(min.x * min.x + min.z * min.z);
        float distanceFromCenterMax = std::sqrt(max.x * max.x + max.z * max.z);
        if((distanceFromCenterMin <= radius) || distanceFromCenterMax <= radius)
            return true;
    }
    return false;
}