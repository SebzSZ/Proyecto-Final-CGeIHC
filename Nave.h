#pragma once

#include <stdio.h>
#include <cmath>
#include <memory>
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include "Model.h"
#include "GameObject.h"
#include "Material.h"

#define MAX_FRAMES 100

typedef struct _frame
{
    float movAvion_x;
    float movAvion_y;
    float movAvion_xInc;
    float movAvion_yInc;
    float giroAvion;
    float giroAvionInc;
} FRAME;

class Nave
{
public:
    Nave();
    ~Nave();

    bool Initialize(Material& material);

    void SetKeyframesIniciales();
    void DisplayMenu();

    void Update(float deltaTime);

    std::shared_ptr<GameObject> GetNaveObject() const { return naveObj; }

private:
    Model Nave_M;
    Model Ala_M;

    std::shared_ptr<GameObject> naveObj;
    std::shared_ptr<GameObject> alaIzqObj;
    std::shared_ptr<GameObject> alaDerObj;

    float posXavion;
    float posYavion;
    float posZavion;
    float movAvion_x;
    float movAvion_y;
    float giroAvion;

    float velocidadAlas;
    float amplitudAlas;
    float tiempoVuelo;

    int i_max_steps;
    int i_curr_steps;
    FRAME KeyFrame[MAX_FRAMES];
    int FrameIndex;
    int playIndex;
    bool play;

    void InputControl();
    void Animate();
    void saveFrame();
    void resetElements();
    void interpolation();
    void GuardarKeyframesArchivo();
    void CargarKeyframesArchivo();
};