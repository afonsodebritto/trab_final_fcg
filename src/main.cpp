#include <cmath>
#include <cstdio>
#include <cstdlib>

// Headers abaixo são específicos de C++
#include <map>
#include <string>
#include <limits>
#include <fstream>
#include <sstream>

// Headers das bibliotecas OpenGL
#include <glad/glad.h>   // Criação de contexto OpenGL 3.3
#include <GLFW/glfw3.h>  // Criação de janelas do sistema operacional

// Headers da biblioteca GLM: criação de matrizes e vetores.
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

// Headers locais, definidos na pasta "include/"
#include "utils.h"
#include "Matrices.h"
#include "shaderClass.h"
#include "Camera.h"
#include "Airplane.h"
#include "Inputs.h"
#include "Obj.h"
#include "VirtualScene.h"
#include "Scenario.h"
#include "textures.h"

// Declaração de funções auxiliares para renderizar texto dentro da janela
// OpenGL. Estas funções estão definidas no arquivo "textrendering.cpp".
void TextRendering_Init();
float TextRendering_LineHeight(GLFWwindow* window);
float TextRendering_CharWidth(GLFWwindow* window);
void TextRendering_PrintString(GLFWwindow* window, const std::string &str, float x, float y, float scale = 1.0f);

// Funções abaixo renderizam como texto na janela OpenGL algumas matrizes e
// outras informações do programa. Definidas após main().
void TextRendering(GLFWwindow* window, Airplane& airplane, Scenario &Scenario);

// Funções callback para comunicação com o sistema operacional e interação do
// usuário. Veja mais comentários nas definições das mesmas, abaixo.
void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void ErrorCallback(int error, const char* description);
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);

float g_ScreenRatio = 1.0f;

Inputs g_Inputs;

// Variável que controla se o texto informativo será mostrado na tela.
bool g_ShowInfoText = true;
bool g_Reset = false;

int main()
{
    int success = glfwInit();
    if (!success)
    {
        fprintf(stderr, "ERROR: glfwInit() failed.\n");
        std::exit(EXIT_FAILURE);
    }

    glfwSetErrorCallback(ErrorCallback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window;
    window = glfwCreateWindow(800, 800, "Low Poly Flight", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        fprintf(stderr, "ERROR: glfwCreateWindow() failed.\n");
        std::exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, KeyCallback);

    // Definimos a função de callback que será chamada sempre que a janela for
    // redimensionada, por consequência alterando o tamanho do "framebuffer"
    // (região de memória onde são armazenados os pixels da imagem).
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
    glfwSetWindowSize(window, 800, 800); // Forçamos a chamada do callback acima, para definir g_ScreenRatio.

    // Indicamos que as chamadas OpenGL deverão renderizar nesta janela
    glfwMakeContextCurrent(window);

    // Carregamento de todas funções definidas por OpenGL 3.3, utilizando a
    // biblioteca GLAD.
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

    // Imprimimos no terminal informações sobre a GPU do sistema
    const GLubyte *vendor      = glGetString(GL_VENDOR);
    const GLubyte *renderer    = glGetString(GL_RENDERER);
    const GLubyte *glversion   = glGetString(GL_VERSION);
    const GLubyte *glslversion = glGetString(GL_SHADING_LANGUAGE_VERSION);

    printf("GPU: %s, %s, OpenGL %s, GLSL %s\n", vendor, renderer, glversion, glslversion);

    // Carregamos os shaders de vértices e de fragmentos que serão utilizados
    // para renderização. Veja slides 180-200 do documento Aula_03_Rendering_Pipeline_Grafico.pdf.
    //
    LoadTextureImage("../../data/11804_Airplane_diff.jpg");      // TextureImage0
    LoadTextureImage("../../data/Palette_Gradient.png");
    LoadTextureImage("../../data/chao.png");

    // Inicializamos o código para renderização de texto.

    Shader GpuProgram("../../src/shaders/shader_vertex.glsl", "../../src/shaders/shader_fragment.glsl");

    TextRendering_Init();

    // Habilitamos o Z-buffer. Veja slides 104-116 do documento Aula_09_Projecoes.pdf.
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    // Variáveis auxiliares utilizadas para chamada à função
    // TextRendering_ShowModelViewProjection(), armazenando matrizes 4x4.
    glm::mat4 the_projection;
    glm::mat4 the_model;
    glm::mat4 the_view;
    glm::mat4 model;

    Airplane Airplane(glm::vec4(0.0f,1.0f,0.0f,1.0f), 10.0f, 50.0f);

    float nearPlane = -0.1f;
    float farPlane  = -2000.0f;
    float fov = M_PI / 3.0f;
    Camera Camera;

    bool cameraType = LOOKAT_CAMERA;

    float deltaTime, lastFrame, currentFrame;

    VirtualScene VirtualScene;

    ObjModel airplaneModel("../../data/aviao.obj");
    VirtualScene.BuildTriangles(airplaneModel);

    ObjModel treeGreenModel("../../data/TreeGreen.obj");
    VirtualScene.BuildTriangles(treeGreenModel);
    
    ObjModel treeRedModel("../../data/TreeRed.obj");
    VirtualScene.BuildTriangles(treeRedModel);
    
    ObjModel treeBlueModel("../../data/TreeBlue.obj");
    VirtualScene.BuildTriangles(treeBlueModel);

    ObjModel chaoModel("../../data/chao.obj");
    VirtualScene.BuildTriangles(chaoModel);

    Scenario Scenario(300.0f, 0.03f, VirtualScene, GpuProgram);


    // Ficamos em um loop infinito, renderizando, até que o usuário feche a janela
    while (!glfwWindowShouldClose(window))
    {
        
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(GpuProgram.ID);

        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwGetCursorPos(window, &g_Inputs.cursorXPos, &g_Inputs.cursorYPos);

        if(g_Inputs.keyPressedSpace)
            cameraType = FREE_CAMERA;
        else
            cameraType = LOOKAT_CAMERA;

        Camera.Update(g_ScreenRatio, Airplane, g_Inputs, cameraType);
        Camera.Matrix(fov, nearPlane, farPlane, GpuProgram, deltaTime);

        if(cameraType == LOOKAT_CAMERA && Camera.movingThroughBezierCurve == false)
            Airplane.Movimentation(g_Inputs, deltaTime, VirtualScene, Scenario);

        if(g_Reset)
        {
            Airplane.Position = glm::vec4(0.0f,1.0f,0.0f,1.0f);
            g_Reset = false;
        }

        Airplane.Draw(VirtualScene, GpuProgram, deltaTime);

        Scenario.DrawAllTrees(VirtualScene, GpuProgram);

        Scenario.DrawFloor(VirtualScene, GpuProgram);

        TextRendering(window, Airplane, Scenario);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    // Finalizamos o uso dos recursos do sistema operacional
    glfwTerminate();

    // Fim do programa
    return 0;
}

// Definição da função que será chamada sempre que a janela do sistema
// operacional for redimensionada, por consequência alterando o tamanho do
// "framebuffer" (região de memória onde são armazenados os pixels da imagem).
void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    // Indicamos que queremos renderizar em toda região do framebuffer. A
    // função "glViewport" define o mapeamento das "normalized device
    // coordinates" (NDC) para "pixel coordinates".  Essa é a operação de
    // "Screen Mapping" ou "Viewport Mapping" vista em aula ({+ViewportMapping2+}).
    glViewport(0, 0, width, height);

    // Atualizamos também a razão que define a proporção da janela (largura /
    // altura), a qual será utilizada na definição das matrizes de projeção,
    // tal que não ocorra distorções durante o processo de "Screen Mapping"
    // acima, quando NDC é mapeado para coordenadas de pixels. Veja slides 205-215 do documento Aula_09_Projecoes.pdf.
    //
    // O cast para float é necessário pois números inteiros são arredondados ao
    // serem divididos!
    g_ScreenRatio = (float)width / height;
}

// Definição da função que será chamada sempre que o usuário pressionar alguma
// tecla do teclado. Veja http://www.glfw.org/docs/latest/input_guide.html#input_key
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mod)
{
    // ====================
    // Não modifique este loop! Ele é utilizando para correção automatizada dos
    // laboratórios. Deve ser sempre o primeiro comando desta função KeyCallback().
    for (int i = 0; i < 10; ++i)
        if (key == GLFW_KEY_0 + i && action == GLFW_PRESS && mod == GLFW_MOD_SHIFT)
            std::exit(100 + i);
    // ====================

    // Se o usuário pressionar a tecla ESC, fechamos a janela.
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key == GLFW_KEY_W)
        g_Inputs.keyPressedW = (action != GLFW_RELEASE);
    if (key == GLFW_KEY_A)
        g_Inputs.keyPressedA = (action != GLFW_RELEASE);
    if (key == GLFW_KEY_S)
        g_Inputs.keyPressedS = (action != GLFW_RELEASE);
    if (key == GLFW_KEY_D)
        g_Inputs.keyPressedD = (action != GLFW_RELEASE);
    if (key == GLFW_KEY_LEFT_SHIFT)
        g_Inputs.keyPressedLeftShift = (action != GLFW_RELEASE);
    if (key == GLFW_KEY_LEFT_CONTROL)
        g_Inputs.keyPressedLeftControl = (action != GLFW_RELEASE);
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        g_Inputs.keyPressedSpace = !g_Inputs.keyPressedSpace;


    // Se o usuário apertar a tecla H, fazemos um "toggle" do texto informativo mostrado na tela.
    if (key == GLFW_KEY_H && action == GLFW_PRESS)
    {
        g_ShowInfoText = !g_ShowInfoText;
    }

    if (key == GLFW_KEY_R && action == GLFW_PRESS)
    {
        g_Reset = !g_Reset;
    }
}

// Definimos o callback para impressão de erros da GLFW no terminal
void ErrorCallback(int error, const char* description)
{
    fprintf(stderr, "ERROR: GLFW: %s\n", description);
}

void TextRendering(GLFWwindow* window, Airplane& airplane, Scenario &Scenario)
{
    if ( !g_ShowInfoText )
        return;

    // Variáveis estáticas (static) mantém seus valores entre chamadas
    // subsequentes da função!
    static char  bufferSpeed[20] = "0.00 km/h";

    static char  bufferAltitude[20] = "y: ??? m";

    static char  bufferPitch[20] = "pitch: 0.00 deg";

    static char  bufferYaw[20] = "yaw: 0.00 deg";

    static char  bufferRoll[20] = "roll: 0.00 deg";

    static float old_seconds = (float)glfwGetTime();
    static int   ellapsed_frames = 0;
    static char  bufferFPS[20] = "??? fps";
    static int   numcharsFPS = 7;

    static char  bufferTrees[20] = "??? trees";
    static int   numcharsTrees = 10;

    static char  bufferX[20] = "x: ??? m";

    static char  bufferZ[20] = "z: ??? m";

    ellapsed_frames += 1;

    // Recuperamos o número de segundos que passou desde a execução do programa
    float seconds = (float)glfwGetTime();

    // Número de segundos desde o último cálculo do fps
    float ellapsed_seconds = seconds - old_seconds;

    if ( ellapsed_seconds > 1.0f )
    {
        numcharsFPS = snprintf(bufferFPS, 20, "%.2f fps", ellapsed_frames / ellapsed_seconds);
    
        old_seconds = seconds;
        ellapsed_frames = 0;
    }


    snprintf(bufferSpeed, 20, "%.2f km/h", fabs(airplane.speed)*3.6f);
    
    snprintf(bufferAltitude, 20, "y: %.2f m", airplane.Position.y);
    
    snprintf(bufferPitch, 20, "pitch: %.2f deg", airplane.pitch*180/M_PI);

    snprintf(bufferYaw, 20, "yaw: %.2f deg", airplane.yaw*180/M_PI);

    snprintf(bufferRoll, 20, "roll: %.2f deg", airplane.roll*180/M_PI);

    numcharsTrees = snprintf(bufferTrees, 20, "%d trees", Scenario.numTrees);

    snprintf(bufferX, 20, "x: %.2f m", airplane.Position.x);

    snprintf(bufferZ, 20, "z: %.2f m", airplane.Position.z);

    float lineheight = TextRendering_LineHeight(window);
    float charwidth = TextRendering_CharWidth(window);

    TextRendering_PrintString(window, bufferSpeed, -1.0f+charwidth, 1.0f-lineheight, 1.0f);

    TextRendering_PrintString(window, bufferPitch, -1.0f+charwidth, -1.0f+3*lineheight, 1.0f);

    TextRendering_PrintString(window, bufferYaw, -1.0f+charwidth, -1.0f+2*lineheight, 1.0f);

    TextRendering_PrintString(window, bufferRoll, -1.0f+charwidth, -1.0f+lineheight, 1.0f);

    TextRendering_PrintString(window, bufferFPS, 1.0f-(numcharsFPS + 1)*charwidth, 1.0f-lineheight, 1.0f);

    TextRendering_PrintString(window, bufferTrees, 1.0f-(numcharsTrees + 1)*charwidth, -1.0f+lineheight, 1.0f);

    TextRendering_PrintString(window, bufferX, -1.0f+charwidth, 1.0f-2*lineheight, 1.0f);

    TextRendering_PrintString(window, bufferAltitude, -1.0f+charwidth, 1.0f-3*lineheight, 1.0f);

    TextRendering_PrintString(window, bufferZ, -1.0f+charwidth, 1.0f-4*lineheight, 1.0f);

}


// set makeprg=cd\ ..\ &&\ make\ run\ >/dev/null
// vim: set spell spelllang=pt_br :
