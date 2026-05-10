#define _CRT_SECURE_NO_WARNINGS
#include "Nave.h"
#include "InputManager.h"

Nave::Nave()
{
    posXavion = -10.0f;
    posYavion = 4.0f;
    posZavion = -10.0f;
    movAvion_x = 0.0f;
    movAvion_y = 0.0f;
    giroAvion = 0.0f;

    velocidadAlas = 10.0f;
    amplitudAlas = 25.0f;
    tiempoVuelo = 0.0f;

    i_max_steps = 600;
    i_curr_steps = 6;
    FrameIndex = 6;
    playIndex = 0;
    play = false;
}

Nave::~Nave() {}

bool Nave::Initialize(Material& material)
{
    if (!Nave_M.load("Models/nave.obj")) return false;
    if (!Ala_M.load("Models/ala.obj")) return false;

    naveObj = std::make_shared<GameObject>("Nave", GameObjectType::MODEL);
    naveObj->setModel(&Nave_M);
    naveObj->setMaterial(&material);


    alaIzqObj = std::make_shared<GameObject>("AlaIzquierda", GameObjectType::MODEL);
    alaIzqObj->setModel(&Ala_M);
    alaIzqObj->setMaterial(&material);

    alaIzqObj->transform.setPosition(0.0f, 0.0f, -0.3f);
    naveObj->addChild(alaIzqObj);


    alaDerObj = std::make_shared<GameObject>("AlaDerecha", GameObjectType::MODEL);
    alaDerObj->setModel(&Ala_M);
    alaDerObj->setMaterial(&material);

    alaDerObj->transform.setPosition(0.0f, 0.0f, 0.3f);
    alaDerObj->transform.setScale(1.0f, 1.0f, -1.0f);
    naveObj->addChild(alaDerObj);

    return true;
}

void Nave::SetKeyframesIniciales()
{
    KeyFrame[0].movAvion_x = 0.0f;
    KeyFrame[0].movAvion_y = 0.0f;
    KeyFrame[0].giroAvion = 0.0f;

    KeyFrame[1].movAvion_x = 1.0f;
    KeyFrame[1].movAvion_y = 2.0f;
    KeyFrame[1].giroAvion = 0.0f;

    KeyFrame[2].movAvion_x = 2.0f;
    KeyFrame[2].movAvion_y = 0.0f;
    KeyFrame[2].giroAvion = 0.0f;

    KeyFrame[3].movAvion_x = 3.0f;
    KeyFrame[3].movAvion_y = -2.0f;
    KeyFrame[3].giroAvion = 0.0f;

    KeyFrame[4].movAvion_x = 3.0f;
    KeyFrame[4].movAvion_y = -2.0f;
    KeyFrame[4].giroAvion = 180.0f;

    KeyFrame[5].movAvion_x = 0.0f;
    KeyFrame[5].movAvion_y = 0.0f;
    KeyFrame[5].giroAvion = 180.0f;
}

void Nave::DisplayMenu()
{
    printf("\n--- CONTROLES DE ANIMACION POR KEYFRAMES ---\n");

    printf("MOVIMIENTO MANUAL:\n");
    printf(" [Flechas Izq/Der] Mover nave en el eje X.\n");
    printf(" [Flechas Arr/Aba] Mover nave en el eje Y.\n");
    printf(" [Q / E] Girar la nave hacia la izquierda/derecha.\n\n");

    printf("GUARDADO DE FRAMES EN MEMORIA:\n");
    printf(" [L] Guardar el Keyframe actual.\n\n");

    printf("GUARDADO/CARGADO EN ARCHIVO TXT:\n");
    printf(" [G] Guardar todos los Keyframes generados en 'animacion_keyframes.txt'.\n");
    printf(" [C] Cargar Keyframes desde el archivo TXT.\n\n");

    printf("REPRODUCCION:\n");
    printf(" [Barra Espaciadora] Iniciar o pausar la animacion guardada.\n");
    printf("--------------------------------------------\n\n");
}

void Nave::saveFrame()
{
    printf("FrameIndex guardado: %d\n", FrameIndex);
    KeyFrame[FrameIndex].movAvion_x = movAvion_x;
    KeyFrame[FrameIndex].movAvion_y = movAvion_y;
    KeyFrame[FrameIndex].giroAvion = giroAvion;
    FrameIndex++;
}

void Nave::resetElements()
{
    movAvion_x = KeyFrame[0].movAvion_x;
    movAvion_y = KeyFrame[0].movAvion_y;
    giroAvion = KeyFrame[0].giroAvion;
}

void Nave::interpolation()
{
    KeyFrame[playIndex].movAvion_xInc = (KeyFrame[playIndex + 1].movAvion_x - KeyFrame[playIndex].movAvion_x) / i_max_steps;
    KeyFrame[playIndex].movAvion_yInc = (KeyFrame[playIndex + 1].movAvion_y - KeyFrame[playIndex].movAvion_y) / i_max_steps;
    KeyFrame[playIndex].giroAvionInc = (KeyFrame[playIndex + 1].giroAvion - KeyFrame[playIndex].giroAvion) / i_max_steps;
}

void Nave::GuardarKeyframesArchivo()
{
    FILE* archivo;
    fopen_s(&archivo, "animacion_keyframes.txt", "w");

    if (archivo == NULL)
    {
        printf("Error al abrir el archivo para guardar.\n");
        return;
    }

    fprintf(archivo, "%d\n", FrameIndex);
    for (int i = 0; i < FrameIndex; i++)
    {
        fprintf(archivo, "%f %f %f\n", KeyFrame[i].movAvion_x, KeyFrame[i].movAvion_y, KeyFrame[i].giroAvion);
    }

    fclose(archivo);
    printf("EXITO: Keyframes guardados en 'animacion_keyframes.txt'. (Total de frames: %d)\n", FrameIndex);
}

void Nave::CargarKeyframesArchivo()
{
    FILE* archivo;
    fopen_s(&archivo, "animacion_keyframes.txt", "r");

    if (archivo == NULL)
    {
        printf("ERROR: No se encontro el archivo 'animacion_keyframes.txt'.\n");
        return;
    }

    fscanf_s(archivo, "%d", &FrameIndex);
    for (int i = 0; i < FrameIndex; i++)
    {
        fscanf_s(archivo, "%f %f %f", &KeyFrame[i].movAvion_x, &KeyFrame[i].movAvion_y, &KeyFrame[i].giroAvion);
    }

    fclose(archivo);
    printf("EXITO: Keyframes cargados correctamente. (Total de frames: %d)\n", FrameIndex);
    resetElements();
}

void Nave::InputControl()
{
    InputManager& input = InputManager::getInstance();

    if (input.isKeyPressed(GLFW_KEY_SPACE))
    {
        if (play == false && (FrameIndex > 1))
        {
            resetElements();
            interpolation();
            play = true;
            playIndex = 0;
            i_curr_steps = 0;
            printf("Animacion iniciada.\n");
        }
        else
        {
            play = false;
            printf("Animacion detenida.\n");
        }
    }

    if (input.isKeyPressed(GLFW_KEY_L))
    {
        saveFrame();
    }

    if (input.isKeyPressed(GLFW_KEY_RIGHT))
    {
        movAvion_x += 1.0f;
        printf("X: %f, Y: %f, Giro: %f\n", movAvion_x, movAvion_y, giroAvion);
    }

    if (input.isKeyPressed(GLFW_KEY_LEFT))
    {
        movAvion_x -= 1.0f;
        printf("X: %f, Y: %f, Giro: %f\n", movAvion_x, movAvion_y, giroAvion);
    }

    if (input.isKeyPressed(GLFW_KEY_UP))
    {
        movAvion_y += 1.0f;
        printf("X: %f, Y: %f, Giro: %f\n", movAvion_x, movAvion_y, giroAvion);
    }

    if (input.isKeyPressed(GLFW_KEY_DOWN))
    {
        movAvion_y -= 1.0f;
        printf("X: %f, Y: %f, Giro: %f\n", movAvion_x, movAvion_y, giroAvion);
    }

    if (input.isKeyPressed(GLFW_KEY_E))
    {
        giroAvion -= 45.0f;
        printf("X: %f, Y: %f, Giro: %f\n", movAvion_x, movAvion_y, giroAvion);
    }

    if (input.isKeyPressed(GLFW_KEY_Q))
    {
        giroAvion += 45.0f;
        printf("X: %f, Y: %f, Giro: %f\n", movAvion_x, movAvion_y, giroAvion);
    }

    if (input.isKeyPressed(GLFW_KEY_G))
    {
        GuardarKeyframesArchivo();
    }

    if (input.isKeyPressed(GLFW_KEY_C))
    {
        CargarKeyframesArchivo();
    }
}

void Nave::Animate()
{
    if (play)
    {
        if (i_curr_steps >= i_max_steps)
        {
            playIndex++;
            if (playIndex > FrameIndex - 2)
            {
                printf("Termina animacion.\n");
                playIndex = 0;
                play = false;
            }
            else
            {
                i_curr_steps = 0;
                interpolation();
            }
        }
        else
        {
            movAvion_x += KeyFrame[playIndex].movAvion_xInc;
            movAvion_y += KeyFrame[playIndex].movAvion_yInc;
            giroAvion += KeyFrame[playIndex].giroAvionInc;
            i_curr_steps++;
        }
    }
}

void Nave::Update(float deltaTime)
{
    InputControl();
    Animate();

    glm::vec3 posblackhawk = glm::vec3(posXavion + movAvion_x, posYavion + movAvion_y, posZavion);
    naveObj->transform.setPosition(posblackhawk);
    naveObj->transform.setRotation(0.0f, giroAvion, 0.0f);

    if (play) 
    {
        tiempoVuelo += deltaTime;
    }
    else 
    {
        tiempoVuelo = 0.0f;
    }

    float anguloAnimacion = sin(tiempoVuelo * velocidadAlas) * amplitudAlas;

    alaIzqObj->transform.setRotation(0.0f, 90.0f + anguloAnimacion, 0.0f);
    alaDerObj->transform.setRotation(0.0f, -90.0f + anguloAnimacion, 0.0f);
}