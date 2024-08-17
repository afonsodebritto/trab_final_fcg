#include"Scenario.h"

Scenario::Scenario(float radius, float probability, VirtualScene &VirtualScene, Shader &GpuProgram)
    : radius(radius)
{
    // Definir o tamanho da matriz de árvores com base no raio
    int matrixSize = static_cast<int>(2 * radius / 4);
    treeMatrix.resize(matrixSize, std::vector<Tree>(matrixSize));

    // Inicializar o gerador de números aleatórios com Mersenne Twister
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> probDist(0.0, 1.0);
    std::uniform_real_distribution<> scaleDist(1.0, 3.0);

    // Calcular a probabilidade baseada no número de árvores desejado e o tamanho da matriz
    if (probability > 1.0f) probability = 1.0f;

    // Iterar sobre cada elemento da matriz e preencher com árvores com base na probabilidade
    for (int i = 0; i < matrixSize; ++i)
    {
        for (int j = 0; j < matrixSize; ++j)
        {
            // Calcular a posição do centro do chunk
            float xPos = (i * 4.0f) + 2.0f - radius;
            float zPos = (j * 4.0f) + 2.0f - radius;

            // Verificar se a posição do chunk está dentro do círculo de raio "radius"
            float distanceFromCenter = std::sqrt(xPos * xPos + zPos * zPos);
            if (distanceFromCenter <= radius)
            {
                // Gerar um número aleatório entre 0 e 1 para determinar se uma árvore será colocada
                if (probDist(gen) < probability)
                {
                    Tree tree;

                    // Definir a posição da árvore no centro do chunk
                    tree.Position = glm::vec4(xPos, 0.0f, zPos, 1.0f);

                    // Definir o fator de escala aleatório com as restrições x=z e entre 1.0f e 3.0f
                    float scaleXZ = scaleDist(gen);
                    tree.Scale = glm::vec3(scaleXZ, scaleDist(gen), scaleXZ);

                    // Adicionar a árvore à matriz e ao vetor de árvores
                    treeMatrix[i][j] = tree;
                    treeVector.push_back(tree);
                    numTrees++;
                }
            }
        }
    }
}





std::vector<Tree> Scenario::getAdjascentTrees(glm::vec4 Position)
{
    std::vector<Tree> adjacentTrees;

    // Calcular o índice da posição na matriz
    int matrixSize = static_cast<int>(2 * radius / 4);
    int xIndex = static_cast<int>((Position.x + radius) / 4.0f);
    int zIndex = static_cast<int>((Position.z + radius) / 4.0f);

    // Garantir que os índices estão dentro dos limites da matriz
    xIndex = std::max(0, std::min(xIndex, matrixSize - 1));
    zIndex = std::max(0, std::min(zIndex, matrixSize - 1));

    // Iterar sobre os chunks da vizinhança 8, incluindo o chunk central
    for (int i = -1; i <= 1; ++i)
    {
        for (int j = -1; j <= 1; ++j)
        {
            int neighborX = xIndex + i;
            int neighborZ = zIndex + j;

            // Verificar se os índices vizinhos estão dentro dos limites da matriz
            if (neighborX >= 0 && neighborX < matrixSize && neighborZ >= 0 && neighborZ < matrixSize)
            {
                // Adicionar a árvore do chunk vizinho, se existir
                Tree &tree = treeMatrix[neighborX][neighborZ];
                adjacentTrees.push_back(tree);
            }
        }
    }

    return adjacentTrees;
}

void Scenario::DrawTree(Tree tree, VirtualScene &VirtualScene, Shader &GpuProgram)
{
    glm::mat4 treeModel = Matrix_Translate(tree.Position.x,tree.Position.y - 2.0f,tree.Position.z)
                          * Matrix_Rotate_X(-M_PI_2)
                          * Matrix_Scale(0.003f*tree.Scale.x,0.003f*tree.Scale.y,0.003f*tree.Scale.z);
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