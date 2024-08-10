#include "Airplane.h"

void Airplane::Movimentation(bool accelerate, bool rotateRight, bool rotateLeft, bool rotateUp, bool rotateDown, bool brake, float delta_time)
{
    // pitts special s2
    // 10m = 1m = 1.0f -> valores adaptados para essa restrição
    // Atualiza a direção do avião considerando as rotações atuais
    Direction = Matrix_Rotate_Z(-rotationAngleZ) * Matrix_Rotate_X(-rotationAngleX) * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);

    // Coeficientes e variáveis físicas
    float drag_coefficient = 0.02f;   // Coeficiente de arrasto ajustado para maior realismo
    float air_density = 1.225f;       // Densidade do ar ao nível do mar (kg/m^3)
    float reference_area = 0.12f;     // Área frontal de referência do avião (m^2)
    float mass = 0.54f;               // Massa do avião (kg)
    float lift_coefficient = 0.3f;    // Coeficiente de sustentação básico
    float gravity = 0.981f;           // Aceleração devido à gravidade (m/s^2)

    // Cálculo das forças aerodinâmicas
    float drag_force = 0.5f * drag_coefficient * air_density * pow(speed, 2) * reference_area;
    float air_deceleration = drag_force / mass;

    // Sustentação (lift) que contrabalança a gravidade, mantendo o avião em voo
    float lift_force = 0.5f * lift_coefficient * air_density * pow(speed, 2) * reference_area;
    float lift_acceleration = lift_force / mass;

    // Controla a taxa de rotação baseada na velocidade atual
    float delta_rotate = (M_PI / 2) * delta_time * (speed + gravity) / 24;


    // Aceleração e desaceleração
    if (accelerate && speed < max_speed)
        if(Position.y <= 0)
            speed += acceleration/2 * delta_time;
        else
            speed += acceleration * delta_time;

    if (brake && speed > 0)
        if(Position.y <= 0)
            speed -= acceleration * delta_time;
        else
            speed -= acceleration/2 * delta_time;

    if (!accelerate && speed > 0)
        if(Position.y <= 0)
            speed -= 100 * air_deceleration * delta_time;
        else
            speed -= air_deceleration * delta_time;

    // Controle de rotação: rolagem e arfagem
    if (rotateLeft && !rotateRight)
    {
        if(Position.y > 0.1)
            rotationAngleZ += delta_rotate;
        else
            rotationAngleZ = 0;
    }

    if (rotateRight && !rotateLeft)
    {
        if(Position.y > 0.1)
            rotationAngleZ -= delta_rotate;
        else
            rotationAngleZ = 0;
    }

    if (rotateUp && !rotateDown)
    {
        if(Position.y > 0.1)
            rotationAngleX -= delta_rotate;
        else
            rotationAngleX = 0;
    }

    if (rotateDown && !rotateUp)
    {
        if(Position.y > 0.1)
            rotationAngleX += delta_rotate;
        else
            rotationAngleX = 0;
    }

    if (speed < 0){
        speed = 0;
    }

    if(speed * Direction.y + lift_acceleration < speedGravity && Position.y > 0)
        speedGravity += gravity * delta_time;
    else
        speedGravity = gravity;

    printf("lift: %f\n", lift_acceleration);
    printf("speed gravity: %f\n", speedGravity);

    // Movimentação considerando a velocidade e direção
    Position.x += speed * Direction.x * delta_time;
    Position.y += (speed * Direction.y + lift_acceleration - speedGravity) * delta_time; // Ajusta pela sustentação
    Position.z += speed * Direction.z * delta_time;

    if (Position.y < 0){
        Position.y = 0;
    }

    // Normaliza os ângulos de rotação para manter dentro do intervalo [0, 2*PI]
    rotationAngleX = fmod(rotationAngleX, 2 * M_PI);
    rotationAngleZ = fmod(rotationAngleZ, 2 * M_PI);

    // Atualiza a matriz de transformação do avião
    Matrix = Matrix_Translate(Position.x, Position.y, Position.z) *
             Matrix_Rotate_Z(-rotationAngleZ) *
             Matrix_Rotate_X(-rotationAngleX);
}

Airplane::Airplane(glm::vec4 position, float rotationAngleX, float rotationAngleZ, float acceleration, float max_speed)
{
    Airplane::Position = position;
    Airplane::rotationAngleX = rotationAngleX;
    Airplane::rotationAngleZ = rotationAngleZ;
    Airplane::acceleration = acceleration;
    Airplane::max_speed = max_speed;
    Airplane::speed = 0.0f;  // Inicializa a velocidade do avião em 0
}
