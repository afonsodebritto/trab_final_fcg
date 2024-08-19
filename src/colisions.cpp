#include "colisions.h"
bool cubo_cubo_intersc(glm::vec3 min_a, glm::vec3 max_a, glm::vec3 min_b, glm::vec3 max_b) {
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

// Função para verificar a colisão entre um cubo e uma meia elipse 3D que só existe para baixo
bool cubo_meia_elipse_intersc(glm::vec3 min_a, glm::vec3 max_a, glm::vec3 min_b, glm::vec3 max_b) {
    // Calcula o centro e os raios da elipse a partir da AABB
    glm::vec3 centro_b = (min_b + max_b) / 2.0f;
    glm::vec3 raios_b = (max_b - min_b) / 2.0f;

    // Ajusta o centro da elipse para considerar apenas a meia elipse inferior
    centro_b.y = max_b.y;

    // Calcula o ponto mais próximo do centro da meia elipse na AABB do cubo
    glm::vec3 ponto_mais_proximo = glm::clamp(centro_b, min_a, max_a);

    // Calcula o vetor da elipse até o ponto mais próximo
    glm::vec3 delta = ponto_mais_proximo - centro_b;

    // Ajusta a verificação da colisão para considerar apenas a meia elipse
    if (delta.y > 0) {
        return false; // Fora da meia elipse
    }

    // Calcula o valor de colisão (deve ser <= 1 para colisão)
    float valor_colisao = 
        (delta.x * delta.x) / (raios_b.x * raios_b.x) +
        (delta.y * delta.y) / (raios_b.y * raios_b.y) +
        (delta.z * delta.z) / (raios_b.z * raios_b.z);

    // Verifica a colisão (se valor_colisao <= 1, há colisão)
    bool colisao = valor_colisao <= 1.0f;

    // Imprime os valores de debug
    std::cout << "Cubo A: min(" << min_a.x << ", " << min_a.y << ", " << min_a.z << "), max(" << max_a.x << ", " << max_a.y << ", " << max_a.z << ")" << std::endl;
    std::cout << "Meia Elipse B: centro(" << centro_b.x << ", " << centro_b.y << ", " << centro_b.z << "), raios(" << raios_b.x << ", " << raios_b.y << ", " << raios_b.z << ")" << std::endl;
    std::cout << "Colisão: " << colisao << std::endl;

    return colisao;
}

// Função que aplica a model a bbox
std::pair<glm::vec3, glm::vec3> aplica_model_bbox(const glm::vec3& min, const glm::vec3& max, const glm::mat4& model) {
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

    // Aplicar a matriz model a cada vértice
    for (auto& corner : corners) {
        glm::vec4 bbox_model = model * glm::vec4(corner, 1.0f);
        corner = glm::vec3(bbox_model);
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
