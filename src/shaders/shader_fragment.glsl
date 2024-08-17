#version 330 core

// Atributos de fragmentos recebidos como entrada ("in") pelo Fragment Shader.
// Neste exemplo, este atributo foi gerado pelo rasterizador como a
// interpolação da posição global e a normal de cada vértice, definidas em
// "shader_vertex.glsl" e "main.cpp".
in vec4 position_world;
in vec4 normal;

// Posição do vértice atual no sistema de coordenadas local do modelo.
in vec4 position_model;

// Coordenadas de textura obtidas do arquivo OBJ (se existirem!)
in vec2 texcoords;

// Matrizes computadas no código C++ e enviadas para a GPU
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Identificador que define qual objeto está sendo desenhado no momento
#define FUSELAGE 0
#define WHEEL_LEFT  1
#define WHEEL_RIGHT  2
#define ROTOR 3
#define TREE 4
uniform int object_id;

// Parâmetros da axis-aligned bounding box (AABB) do modelo
uniform vec4 bbox_min;
uniform vec4 bbox_max;

// Variáveis para acesso das imagens de textura
uniform sampler2D TextureImage0;
uniform sampler2D TextureImage1;

// O valor de saída ("out") de um Fragment Shader é a cor final do fragmento.
out vec4 color;

// Constantes
#define M_PI   3.14159265358979323846
#define M_PI_2 1.57079632679489661923

void main()
{
    // Obtemos a posição da câmera utilizando a inversa da matriz que define o
    // sistema de coordenadas da câmera.
    vec4 origin = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 camera_position = inverse(view) * origin;

    // O fragmento atual é coberto por um ponto que percente à superfície de um
    // dos objetos virtuais da cena. Este ponto, p, possui uma posição no
    // sistema de coordenadas global (World coordinates). Esta posição é obtida
    // através da interpolação, feita pelo rasterizador, da posição de cada
    // vértice.
    vec4 p = position_world;

    // Normal do fragmento atual, interpolada pelo rasterizador a partir das
    // normais de cada vértice.
    vec4 n = normalize(normal);

    vec4 light = vec4(1000.0,1000.0,0.0,0.0); // luz não varia com a posição do ponto, simulando luz solar.
    
    // Vetor que define o sentido da fonte de luz em relação ao ponto atual.
    vec4 l = normalize(light);

    // Vetor que define o sentido da câmera em relação ao ponto atual.
    vec4 v = normalize(camera_position - p);

    // Coordenadas de textura U e V
    float U = 0.0;
    float V = 0.0;

    // Espectro da fonte de iluminação
    vec3 I = vec3(1.0, 1.0, 1.0);

    // Espectro da luz ambiente
    vec3 Ia = vec3(0.2, 0.2, 0.2);

    // Obtemos a refletância difusa a partir da leitura da imagem TextureImage0
    vec3 Kd0;
    if(object_id == FUSELAGE || object_id == WHEEL_LEFT || object_id == WHEEL_RIGHT || object_id == ROTOR)
    {
        float U = texcoords.x;
        float V = texcoords.y;
        Kd0 = texture(TextureImage0, vec2(U,V)).rgb;
    }
    else if(object_id == TREE)
    {
        float U = texcoords.x;
        float V = texcoords.y;
        Kd0 = texture(TextureImage1, vec2(U,V)).rgb;
    }
    
    // Equação de Iluminação de lambert
    vec3 termo_difuso_lambert = max(0,dot(n,l)) * I * Kd0;

    color.a = 1;

    // O avião possui iluminação difusa (de lambert)
    if(object_id == FUSELAGE || object_id == WHEEL_LEFT || object_id == WHEEL_RIGHT || object_id == ROTOR)
    {
        color.rgb = termo_difuso_lambert;
    }
    
    // A árvore possui iluminção blinn-phong
    if(object_id == TREE)
    {
        float q_linha = 80.0; 

        vec4 h = normalize(v + l);

        vec3 termo_ambiente = Kd0 * Ia;
        vec3 termo_especular_blinn_phong = dot(n,h) * I * Kd0;

        color.rgb = termo_difuso_lambert + termo_ambiente + termo_especular_blinn_phong;
    }

    // Cor final com correção gamma, considerando monitor sRGB.
    // Veja https://en.wikipedia.org/w/index.php?title=Gamma_correction&oldid=751281772#Windows.2C_Mac.2C_sRGB_and_TV.2Fvideo_standard_gammas
    color.rgb = pow(color.rgb, vec3(1.0,1.0,1.0)/2.2);
} 

