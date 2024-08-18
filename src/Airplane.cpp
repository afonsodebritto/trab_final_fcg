#include "Airplane.h"

void Airplane::Movimentation(Inputs inputs, float delta_time)
{
    if(speed<0) speed = 0;
    // pitts special s2
    // 10m = 1m = 1.0f -> valores adaptados para essa restrição
    // Atualiza a direção do avião considerando as rotações atuais
    Direction = Matrix_Rotate_Y(yaw) * Matrix_Rotate_X(pitch) * Matrix_Rotate_Z(roll) * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
    Direction = Direction / norm(Direction);
    const bool FLYING = Position.y > 0;

    // Sustentação (lift) que contrabalança a gravidade, mantendo o avião em voo

    UpdateRotation(inputs.keyPressedD, inputs.keyPressedA, inputs.keyPressedS, inputs.keyPressedW, delta_time);
    UpdateSpeed(inputs.keyPressedLeftShift, inputs.keyPressedLeftControl, delta_time);
    UpdateAirDensity();

    
    glm::vec4 Velocity = Direction * speed * delta_time;

    // Sustentação (lift) que contrabalança a gravidade, mantendo o avião em voo
    float lift_force = 0.5 * lift_coefficient * air_density * reference_area * pow(speed, 2);
    float lift_acceleration = lift_force / mass;

    float accelerationZ = lift_acceleration - gravity;

    Velocity.y += accelerationZ * delta_time / 10;

    float drag_force = 0.5 * drag_coefficient * air_density * reference_area * pow(speed, 2);

    glm::vec4 DragAcceleration = -Direction * drag_force / mass;

    Velocity += DragAcceleration * delta_time;

    Position += Velocity;

    if (Position.y < 0)
        Position.y = 0;

    // Atualiza a matriz de transformação do avião
    Matrix = Matrix_Translate(Position.x, Position.y, Position.z) *
             Matrix_Rotate_Y(yaw) *
             Matrix_Rotate_X(pitch) *
             Matrix_Rotate_Z(roll);
}

void Airplane::UpdateSpeed(bool accelerate, bool brake, float delta_time)
{
    float drag_force = 0.5f * drag_coefficient * air_density * pow(speed, 2) * reference_area;
    float air_deceleration = drag_force / mass;

    const bool FLYING = Position.y > 0;

    if (accelerate && speed < max_speed)
        if(!FLYING)
            speed += acceleration/2 * delta_time;
        else
            speed += acceleration * delta_time;

    if (brake && speed > 0)
        if(!FLYING)
            speed -= acceleration * delta_time;
        else
            speed -= acceleration/2 * delta_time;

    if (!accelerate && speed > 0)
        if(!FLYING)
            speed -= acceleration/50 * delta_time;
        else
            speed -= air_deceleration * delta_time;
}

void Airplane::UpdateRotation(bool rotateRight, bool rotateLeft, bool rotateUp, bool rotateDown, float delta_time)
{
    float acceleration_rotate = delta_time * (speed + 5) / 2;
    float delta_rotate_pitch = acceleration_rotate * (fabs(pitch) + 1) / 100;
    float delta_rotate_roll = acceleration_rotate * (fabs(roll) + 1) / 50;
    float delta_rotate_yaw = acceleration_rotate * (fabs(roll) + 1) / 50;

    // Aceleração e desaceleração
    const bool FLYING = Position.y > 0;
    const float MAX_ROLL = M_PI/3;
    const float MAX_PITCH = M_PI/3;

    // Controle de rotação: rolagem e arfagem
    if (rotateLeft)
    {
        if(roll > 0)
            yaw += delta_rotate_yaw;
        else if(!FLYING && speed > 0)
            yaw += 1/(35*pow(speed+2,2)); 
        if(FLYING && roll < MAX_ROLL)
            roll += delta_rotate_roll;
    }

    if (rotateRight)
    {
        if(roll < 0)
            yaw -= delta_rotate_yaw;
        else if(!FLYING && speed > 0)
            yaw -= 1/(35*pow(speed+2,2));
        if(FLYING && roll > -MAX_ROLL)
            roll -= delta_rotate_roll;
    }

    if (rotateUp && FLYING && pitch <= MAX_PITCH)
        pitch += delta_rotate_pitch;

    if (rotateDown && FLYING && pitch >= -MAX_PITCH)
        pitch -= delta_rotate_pitch;

    if (!rotateRight && !rotateLeft)
        if(roll < MAX_ROLL+1 && roll > 0)
        {
            roll -= delta_rotate_roll;
            if(roll < 0) roll = 0;
        }
        else if(roll > -MAX_ROLL-1 && roll < 0)
        {
            roll += delta_rotate_roll;
            if(roll > 0) roll = 0;
        }

    if (!FLYING) {
        if(pitch > -MAX_PITCH-1 && pitch < 0)
        {
            pitch += delta_rotate_pitch*2;
            if(pitch > 0) pitch = 0;
        }
        else if(pitch < MAX_PITCH+1 && pitch > 0)
        {
            pitch -= delta_rotate_pitch*2;
            if(pitch < 0) pitch = 0;
        }
        if(roll < MAX_ROLL+1 && roll > 0)
        {
            roll -= delta_rotate_roll*2;
            if(roll < 0) roll = 0;
        }
        else if(roll > -MAX_ROLL-1 && roll < 0)
        {
            roll += delta_rotate_roll*2;
            if(roll > 0) roll = 0;
        }
    }

    // Normaliza os ângulos de rotação para manter dentro do intervalo [-PI, PI]
    pitch = fmod(pitch, 2 * M_PI);
    if (pitch > M_PI) pitch -= 2 * M_PI;
    else if (pitch < -M_PI) pitch += 2 * M_PI;

    yaw = fmod(yaw, 2 * M_PI);
    if (yaw > M_PI) yaw -= 2 * M_PI;
    else if (yaw < -M_PI) yaw += 2 * M_PI;

    roll = fmod(roll, 2 * M_PI);
    if (roll > M_PI) roll -= 2 * M_PI;
    else if (roll < -M_PI) roll += 2 * M_PI;
}

Airplane::Airplane(glm::vec4 position, float acceleration, float max_speed)
{
    Airplane::Position = position;
    Airplane::pitch = 0.0f;
    Airplane::yaw = 0.0f;
    Airplane::roll = 0.0f;
    Airplane::acceleration = acceleration;
    Airplane::max_speed = max_speed;
    Airplane::speed = 0.0f;  // Inicializa a velocidade do avião em 0
}

void Airplane::UpdateAirDensity()
{
    float rho_0 = 1.225f;  // Densidade do ar ao nível do mar (kg/m^3)
    float T_0 = 288.15f;   // Temperatura ao nível do mar (K)
    float L = 0.0065f;     // Taxa de lapso de temperatura (K/m)
    float g = 9.80665f;    // Aceleração da gravidade (m/s^2)
    float R = 287.05f;     // Constante específica do ar (J/(kg*K))

    // Cálculo da densidade
    float T = T_0 - L * (Position.y);
    float expoente = (g / (R * L)) - 1;
    float rho = rho_0 * pow((T / T_0), expoente);

    air_density = rho;
}

void Airplane::Draw(VirtualScene &VirtualScene, Shader &GpuProgram, float delta_time)
{
    // Desenhamos a fusilagem do avião
    glm::mat4 bodyMatrix = Matrix;
    glUniformMatrix4fv(GpuProgram.model_uniform, 1, GL_FALSE, glm::value_ptr(bodyMatrix));
    glUniform1i(GpuProgram.object_id_uniform, FUSELAGE);
    VirtualScene.DrawVirtualObject("body", GpuProgram);

    rotorSpeed += delta_time*speed;

    rotorSpeed = fmod(rotorSpeed, 2 * M_PI);

    glm::mat4 rotorMatrix = Matrix * Matrix_Translate(0.0f, 0.39f, -0.73f) * Matrix_Rotate_Z(rotorSpeed);

    glUniformMatrix4fv(GpuProgram.model_uniform, 1, GL_FALSE, glm::value_ptr(rotorMatrix));
    glUniform1i(GpuProgram.object_id_uniform, FUSELAGE);
    VirtualScene.DrawVirtualObject("rotor", GpuProgram);
}
