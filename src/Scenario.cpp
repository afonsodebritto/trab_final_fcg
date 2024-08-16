#include"Scenario.h"

Scenario::Scenario(float radius, int numTrees, VirtualScene &VirtualScene, Shader &GpuProgram)
    : radius(radius), numTrees(numTrees), gridSizeX(radius), gridSizeY(radius)
{
    // Inicializa a semente para geração de números aleatórios
    std::srand(static_cast<unsigned int>(std::time(0)));

    // Redimensiona a matriz de chunks para o tamanho desejado
    treeMatrix.resize(gridSizeX, std::vector<std::vector<Tree>>(gridSizeY));

    for (int i = 0; i < numTrees; ++i)
    {
        // Gera ângulo e raio aleatórios para a posição dentro do círculo
        float angle = static_cast<float>(std::rand()) / RAND_MAX * 2 * M_PI; // Ângulo em radianos
        float r = static_cast<float>(std::rand()) / RAND_MAX * radius; // Raio aleatório até o limite do círculo

        // Converte para coordenadas x e y
        float x = r * std::cos(angle);
        float y = r * std::sin(angle);

        // Definir z e w como 0 para simplificação, ou usar outro valor conforme necessidade
        glm::vec4 position(x, y, 0.0f, 1.0f);

        // Gera a escala aleatória, com x = z e y aleatório entre 1 e 3
        float scaleXZ = 1.0f + static_cast<float>(std::rand()) / (RAND_MAX / 2.0f);
        float scaleY = 1.0f + static_cast<float>(std::rand()) / (RAND_MAX / 2.0f);
        glm::vec3 scale(scaleXZ, scaleY, scaleXZ);

        // Calcula a posição do chunk (4x4)
        int chunkX = static_cast<int>(x) / 4;
        int chunkY = static_cast<int>(y) / 4;

        // Verifica se a posição do chunk está dentro dos limites da matriz
        if (chunkX >= 0 && chunkX < gridSizeX && chunkY >= 0 && chunkY < gridSizeY)
        {
            Tree tree = Tree{position, scale};
            // Adiciona a árvore ao chunk correspondente
            treeMatrix[chunkX][chunkY].push_back(tree);

            treeVector.push_back(tree);
        }
    }
}


std::vector<Tree> Scenario::getAdjascentTrees(glm::vec4 Position)
{
    std::vector<Tree> adjacentTrees;

    // Calcula em qual chunk a posição atual se encontra
    int chunkX = static_cast<int>(Position.x) / 4;
    int chunkY = static_cast<int>(Position.y) / 4;

    // Percorre o chunk atual e os 8 chunks vizinhos
    for (int dx = -1; dx <= 1; ++dx)
    {
        for (int dy = -1; dy <= 1; ++dy)
        {
            int neighborX = chunkX + dx;
            int neighborY = chunkY + dy;

            // Verifica se o chunk vizinho está dentro dos limites da matriz
            if (neighborX >= 0 && neighborX < gridSizeX && neighborY >= 0 && neighborY < gridSizeY)
            {
                // Adiciona todas as árvores do chunk vizinho ao vetor de árvores adjacentes
                const std::vector<Tree>& treesInChunk = treeMatrix[neighborX][neighborY];
                adjacentTrees.insert(adjacentTrees.end(), treesInChunk.begin(), treesInChunk.end());
            }
        }
    }

    return adjacentTrees;
}


void Scenario::DrawTree(Tree tree, VirtualScene &VirtualScene, Shader &GpuProgram)
{
    glm::mat4 treeModel = Matrix_Scale(0.003f*tree.Scale.x,0.003f*tree.Scale.y,0.003f*tree.Scale.z)
                          * Matrix_Rotate_X(-M_PI_2)
                          * Matrix_Translate(tree.Position.x,tree.Position.y - 2.0f,tree.Position.z);
    glUniformMatrix4fv(GpuProgram.model_uniform, 1 , GL_FALSE , glm::value_ptr(treeModel));
    glUniform1i(GpuProgram.object_id_uniform, 4);
    VirtualScene.DrawVirtualObject("10445_Oak_tree_v1_SG", GpuProgram);
}

void Scenario::DrawAllTrees(VirtualScene &VirtualScene, Shader &GpuProgram)
{
    for (int i = 0; i < numTrees; ++i)
    {
        DrawTree(treeVector[i], VirtualScene, GpuProgram);
    }
}