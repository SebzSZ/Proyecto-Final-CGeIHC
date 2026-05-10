/*
* Proyecto Final - CGeIHC
* Buendía López Sebastián - 320014932
* Hernández Pérez Mariana Daniela - 320180657
* Ortega Novoa Octavio - 317147768
*/

#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "AudioManager.h"
#include "Camera.h"
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "InputManager.h"
#include "Material.h"
#include "Mesh.h"
#include "Model.h"
#include "Shader.h"
#include "Skybox.h"
#include "SpotLight.h"
#include "Texture.h"
#include "Window.h"
#include "GameObject.h"
#include "Train.h"
#include "Castle.h"

// --- INTEGRACION NAVE ---
#include "Nave.h"
// ------------------------

// Rutas de shaders
static const char* VERT_SHADER = "shaders/shader.vert";
static const char* FRAG_SHADER = "shaders/shader.frag";
static const char* SKYBOX_VERT = "shaders/skybox.vert";
static const char* SKYBOX_FRAG = "shaders/skybox.frag";

// Geometría del piso
static GLfloat FLOOR_VERTS[] = {
    //  x      y      z      u      v      nx    ny    nz
       -1.0f,  0.0f, -1.0f,  0.0f, 10.0f,  0.0f, 1.0f, 0.0f,
        1.0f,  0.0f, -1.0f, 10.0f, 10.0f,  0.0f, 1.0f, 0.0f,
        1.0f,  0.0f,  1.0f, 10.0f,  0.0f,  0.0f, 1.0f, 0.0f,
       -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f, 0.0f,
};
static GLuint FLOOR_IDX[] = { 0, 1, 2,  0, 2, 3 };

// CreateAvatar
// Construir la jerarquía del avatar (Ruby)
std::shared_ptr<GameObject> CreateAvatar(
    Material& matOpaco,
    Model& rubyModel,
    Model& rubyLeftArm,
    Model& rubyRightArm,
    Model& rubyLeftLeg,
    Model& rubyRightLeg,
    Model& rubyCape,
    std::shared_ptr<GameObject>& leftArm,
    std::shared_ptr<GameObject>& rightArm,
    std::shared_ptr<GameObject>& leftLeg,
    std::shared_ptr<GameObject>& rightLeg,
    std::shared_ptr<GameObject>& cape)
{
    // Cuerpo raíz
    std::shared_ptr<GameObject> rubyObj = std::make_shared<GameObject>("Ruby", GameObjectType::MODEL);
    rubyObj->setModel(&rubyModel);
    rubyObj->setMaterial(&matOpaco);
    rubyObj->transform.setPosition(20.0f, 1.2f, 1.64f);
    rubyObj->transform.setScale(5.0f);

    // Extremidades — hijos de rubyObj
    leftArm = std::make_shared<GameObject>("LeftArm", GameObjectType::MODEL);
    leftArm->setModel(&rubyLeftArm);
    leftArm->transform.setPosition(0.025f, 0.0611f, 0.0f);
    leftArm->transform.setRotation(0.0f, 0.0f, -65.0f);
    rubyObj->addChild(leftArm);

    rightArm = std::make_shared<GameObject>("RightArm", GameObjectType::MODEL);
    rightArm->setModel(&rubyRightArm);
    rightArm->transform.setPosition(-0.025f, 0.0611f, 0.0f);
    rightArm->transform.setRotation(0.0f, 0.0f, 65.0f);
    rubyObj->addChild(rightArm);

    leftLeg = std::make_shared<GameObject>("LeftLeg", GameObjectType::MODEL);
    leftLeg->setModel(&rubyLeftLeg);
    rubyObj->addChild(leftLeg);

    rightLeg = std::make_shared<GameObject>("RightLeg", GameObjectType::MODEL);
    rightLeg->setModel(&rubyRightLeg);
    rubyObj->addChild(rightLeg);

    cape = std::make_shared<GameObject>("Cape", GameObjectType::MODEL);
    cape->setModel(&rubyCape);
    cape->transform.setPosition(0.0f, 0.065f, -0.01f);
    rubyObj->addChild(cape);

    return rubyObj;
}

// ─────────────────────────────────────────────────────────────────────────────
int main()
{
    // Creación de la ventana
    Window mainWindow("Proyecto Final - CGeIHC");
    if (mainWindow.Initialize() != 0) return -1;

    // Instancia de audio
    AudioManager& audioManager = AudioManager::getInstance();
    if (!audioManager.Initialize()) return -1;

    // Reproducir música
    audioManager.loadMP3("bgMusic", "Sounds/bgMusic.mp3");
    //audioManager.play("bgMusic", true, 0.5f);

    // Configuración inicial de la cámara
    Camera camera(glm::vec3(0.0f, 5.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.1f);
    camera.setCameraMode(CameraMode::THIRD_PERSON);

    // Shader
    Shader shader;
    shader.createFromFiles(VERT_SHADER, FRAG_SHADER);

    // Creación del skybox con texturas de día y noche (ambas con Space)
    Skybox skybox;
    skybox.create(
        // Texturas de día (Space Nebula Blue)
        {
            "Textures/Skybox/jettelly_space_nebulas_blue_LEFT.png",
            "Textures/Skybox/jettelly_space_nebulas_blue_RIGHT.png",
            "Textures/Skybox/jettelly_space_nebulas_blue_UP.png",
            "Textures/Skybox/jettelly_space_nebulas_blue_DOWN.png",
            "Textures/Skybox/jettelly_space_nebulas_blue_FRONT.png",
            "Textures/Skybox/jettelly_space_nebulas_blue_BACK.png"
        },
        // Texturas de noche (Space Nebula Blue)
        {
            "Textures/Skybox/jettelly_space_nebulas_black_LEFT.png",
            "Textures/Skybox/jettelly_space_nebulas_black_RIGHT.png",
            "Textures/Skybox/jettelly_space_nebulas_black_UP.png",
            "Textures/Skybox/jettelly_space_nebulas_black_DOWN.png",
            "Textures/Skybox/jettelly_space_nebulas_black_FRONT.png",
            "Textures/Skybox/jettelly_space_nebulas_black_BACK.png"
        },
        SKYBOX_VERT, SKYBOX_FRAG
    );

    // Configurar duración del ciclo día-noche (30 segundos para el ciclo completo)
    skybox.setDayNightCycleDuration(30.0f);

    // Texturas y materiales
    Texture floorTexture("Textures/naka_yuka_01_52D00-DXT1.png");
    floorTexture.loadWithAlpha();
    Material matOpaco(0.2f, 4.0f);
    Material matBrillante(1.0f, 32.0f);

    // Modelos

    // Ruby — partes del avatar
    Model rubyModel;    if (!rubyModel.load("Models/RubyCuerpo.obj"))      return -1;
    Model rubyLeftArm;  if (!rubyLeftArm.load("Models/RubyBrazoIzq.obj"))  return -1;
    Model rubyRightArm; if (!rubyRightArm.load("Models/RubyBrazoDer.obj")) return -1;
    Model rubyLeftLeg;  if (!rubyLeftLeg.load("Models/RubyPiernaIzq.obj")) return -1;
    Model rubyRightLeg; if (!rubyRightLeg.load("Models/RubyPiernaDer.obj")) return -1;
    Model rubyCape;     if (!rubyCape.load("Models/RubyCapa.obj"))         return -1;

    // Castillo de Hyrule — partes del escenario
    Model mainRoom;  if (!mainRoom.load("Models/HyruleCastle_MainRoom.obj")) return -1;
    Model bigTower;  if (!bigTower.load("Models/HyruleCastle_BigTower.obj")) return -1;
    Model wall;      if (!wall.load("Models/HyruleCastle_Wall.obj"))         return -1;
    Model midTower;  if (!midTower.load("Models/HyruleCastle_MidTower.obj")) return -1;
    Model floor;     if (!floor.load("Models/HyruleCastle_Floor.obj"))       return -1;

    // GameObjects de la escena

    // Piso de la escena
    std::shared_ptr<GameObject> floorObj = std::make_shared<GameObject>("Floor", GameObjectType::MESH);
    MeshData floorData;
    floorData.vertices = std::vector<GLfloat>(std::begin(FLOOR_VERTS), std::end(FLOOR_VERTS));
    floorData.indices = std::vector<GLuint>(std::begin(FLOOR_IDX), std::end(FLOOR_IDX));
    floorObj->loadMesh(floorData);
    floorObj->setTextureID(floorTexture.getID());
    floorObj->setMaterial(&matOpaco);
    floorObj->transform.setScale(50.0f, 1.0f, 50.0f);

    Castle castle;
    if (!castle.Initialize(matOpaco)) return -1;

    Train train;
    if (!train.Initialize(matOpaco)) return -1;

    float trainSpeed = 5.0f;
    float wheelRotationSpeed = 200.0f;

    // --- INTEGRACION NAVE ---
    Nave nave;
    if (!nave.Initialize(matOpaco)) return -1;
    nave.SetKeyframesIniciales();
    nave.DisplayMenu();
    // ------------------------

    // Avatar: Ruby

    // Variables para las extremidades
    std::shared_ptr<GameObject> leftArm;
    std::shared_ptr<GameObject> rightArm;
    std::shared_ptr<GameObject> leftLeg;
    std::shared_ptr<GameObject> rightLeg;
    std::shared_ptr<GameObject> cape;

    // Creación del avatar
    std::shared_ptr<GameObject> rubyObj = CreateAvatar(matOpaco, rubyModel, rubyLeftArm, rubyRightArm, rubyLeftLeg, rubyRightLeg, rubyCape, leftArm, rightArm, leftLeg, rightLeg, cape);

    // Estado de la animación de caminata
    float walkTime = 0.0f;
    float walkSpeed = 10.0f;
    float walkAmplitude = 35.0f;
    float angleThigh = 0.0f;

    // Luces

    // Luz direccional
    DirectionalLight directionalLight(
        1.0f, 0.95f, 0.8f,
        0.3f, 0.8f,
        0.0f, -1.0f, -0.5f
    );

    // Punto de Interés 1: Torre Grande Central
    // Enfoca una de las torres principales del castillo desde una vista dinámica
    glm::vec3 bigTower1Pos(16.84f, 8.92f, -9.0f);
    camera.addInterestPoint(
        bigTower1Pos + glm::vec3(15.0f, 8.0f, 15.0f),
        bigTower1Pos + glm::vec3(0.0f, 5.0f, 0.0f)
    );

    // Punto de Interés 2: Entrada del Castillo (Vista General)
    // Muestra la entrada y la estructura general del castillo
    glm::vec3 castleEntrancePos(20.0f, 5.0f, -8.0f);
    camera.addInterestPoint(
        castleEntrancePos + glm::vec3(-20.0f, 12.0f, 25.0f),
        castleEntrancePos
    );

    // Punto de Interés 3: Torres de Defensa Laterales
    // Enfoca las torres medias de las esquinas del castillo
    glm::vec3 lateralTowerPos(36.09f, 5.44f, -3.64f);
    camera.addInterestPoint(
        lateralTowerPos + glm::vec3(18.0f, 10.0f, 18.0f),
        lateralTowerPos + glm::vec3(0.0f, 8.0f, 0.0f)
    );

    // Proyección
    glm::mat4 projection = glm::perspective(glm::radians(60.0f), (float)mainWindow.getBufferWidth() / (float)mainWindow.getBufferHeight(), 0.1f, 500.0f);

    // Matriz de modelo
    glm::mat4 model(1.0f);
    GLfloat lastTime = (GLfloat)glfwGetTime();

    // Bucle principal
    while (!mainWindow.shouldClose())
    {
        // Delta Time
        GLfloat now = (GLfloat)glfwGetTime();
        GLfloat deltaTime = now - lastTime;
        lastTime = now;

        // Actualizar ciclo día-noche del skybox
        skybox.updateDayNightCycle(deltaTime);

        // Input
        InputManager& input = InputManager::getInstance();
        input.beginFrame();
        glfwPollEvents();

        // Cambio de modo de cámara (C, V, B)
        if (input.isKeyDown(GLFW_KEY_C))
        {
            camera.setCameraMode(CameraMode::THIRD_PERSON);
        }

        if (input.isKeyDown(GLFW_KEY_V))
        {
            camera.setCameraMode(CameraMode::AERIAL);
        }

        if (input.isKeyDown(GLFW_KEY_B))
        {
            camera.setCameraMode(CameraMode::INTEREST_POINT);
        }

        // Avanzar al siguiente punto de interés con SPACE
        if (input.isKeyDown(GLFW_KEY_SPACE) && camera.getCameraMode() == CameraMode::INTEREST_POINT)
            camera.nextInterestPoint();

        // Actualizar cámara según el modo actual
        glm::vec3 rubyPos = rubyObj->transform.getPosition();

        // Destino de tercera persona (usado como target en transiciones hacia/desde ese modo)
        glm::vec3 camDir3P = camera.getDirection();
        glm::vec3 thirdCamPos = rubyPos - glm::normalize(glm::vec3(camDir3P.x, 0.0f, camDir3P.z)) * 2.5f;
        thirdCamPos.y = rubyPos.y + 1.0f;
        glm::vec3 thirdLookAt = rubyPos + glm::vec3(0.0f, 1.0f, 0.0f);

        // Destino aéreo: posición sobre el centro del mapa a aerialHeight
        glm::vec3 aerialTarget(0.0f, 15.0f, 0.0f);
        glm::vec3 aerialLookAt(0.0f, 0.0f, 0.0f);

        switch (camera.getCameraMode())
        {
        case CameraMode::THIRD_PERSON:
            // Durante transición hacia tercera persona, proveer el destino
            if (camera.isInModeTransition())
            {
                camera.setTransitionTarget(thirdCamPos, thirdLookAt);
                camera.updateInterestPointCamera(deltaTime);
                break;
            }
            // Control normal
            if (input.isKeyDown(GLFW_KEY_Q))
                camera.rotateOrbit(90.0f * deltaTime);
            if (input.isKeyDown(GLFW_KEY_E))
                camera.rotateOrbit(-90.0f * deltaTime);
            camera.mouseControl(input.getMouseDeltaX() * 0.5f, input.getMouseDeltaY() * 0.5f);
            camera.updateThirdPersonCamera(rubyPos, deltaTime);
            break;

        case CameraMode::AERIAL:
            // Durante transición hacia aéreo, proveer el destino
            if (camera.isInModeTransition())
            {
                camera.setTransitionTarget(aerialTarget, aerialLookAt);
                camera.updateInterestPointCamera(deltaTime);
                break;
            }
            // Control normal
            camera.updateAerialCamera(input, deltaTime);
            break;

        case CameraMode::INTEREST_POINT:
        {
            // Proveer destino de salida hacia tercera persona
            camera.setTransitionTarget(thirdCamPos, thirdLookAt);
            camera.updateInterestPointCamera(deltaTime);
            break;
        }
        }

        // Movimiento del tren
        train.Update(trainSpeed, deltaTime, wheelRotationSpeed);

        // --- INTEGRACION NAVE ---
        nave.Update(deltaTime);
        // ------------------------

        // Movimiento de Ruby — solo en modo THIRD_PERSON y sin transición activa
        float rubyMoveSpeed = 8.0f;
        glm::vec3 cameraDirection = camera.getDirection();
        glm::vec3 moveDirection = glm::normalize(glm::vec3(cameraDirection.x, 0.0f, cameraDirection.z));
        glm::vec3 rightDirection = glm::normalize(glm::cross(moveDirection, glm::vec3(0.0f, 1.0f, 0.0f)));

        bool isWalking = false;

        if (camera.getCameraMode() == CameraMode::THIRD_PERSON && !camera.isInModeTransition())
        {
            if (input.isKeyDown(GLFW_KEY_W))
            {
                glm::vec3 movement = moveDirection * rubyMoveSpeed * deltaTime;
                rubyObj->transform.translate(movement.x, 0.0f, movement.z);
                isWalking = true;
            }
            if (input.isKeyDown(GLFW_KEY_S))
            {
                glm::vec3 movement = moveDirection * rubyMoveSpeed * deltaTime;
                rubyObj->transform.translate(-movement.x, 0.0f, -movement.z);
                isWalking = true;
            }
            if (input.isKeyDown(GLFW_KEY_A))
            {
                glm::vec3 movement = rightDirection * rubyMoveSpeed * deltaTime;
                rubyObj->transform.translate(-movement.x, 0.0f, -movement.z);
                isWalking = true;
            }
            if (input.isKeyDown(GLFW_KEY_D))
            {
                glm::vec3 movement = rightDirection * rubyMoveSpeed * deltaTime;
                rubyObj->transform.translate(movement.x, 0.0f, movement.z);
                isWalking = true;
            }
        }

        // Animación de caminata de Ruby
        if (isWalking)
        {
            walkTime += deltaTime * walkSpeed;
            angleThigh = walkAmplitude * sin(walkTime);
        }
        else
        {
            walkTime = 0.0f;
            angleThigh += (0.0f - angleThigh) * 8.0f * deltaTime;
        }

        leftArm->transform.setRotation(angleThigh, 0.0f, -65.0f);
        rightArm->transform.setRotation(-angleThigh, 0.0f, 65.0f);
        leftLeg->transform.setRotation(-angleThigh, 0.0f, 0.0f);
        rightLeg->transform.setRotation(angleThigh, 0.0f, 0.0f);
        cape->transform.setRotation((-angleThigh * 0.5f) + 45.0f, 0.0f, 0.0f);

        // Orientar a Ruby hacia donde mira la cámara — solo en modo THIRD_PERSON sin transición
        if (camera.getCameraMode() == CameraMode::THIRD_PERSON && !camera.isInModeTransition())
        {
            glm::vec3 rubyDirection = glm::normalize(glm::vec3(cameraDirection.x, 0.0f, cameraDirection.z));
            float angleY = atan2(rubyDirection.x, rubyDirection.z);
            rubyObj->transform.setRotation(0.0f, glm::degrees(angleY), 0.0f);
        }

        // Luces

        // Luz direccional
        float timeProgress = skybox.getTimeProgress();
        float dayIntensity, nightIntensity;

        if (timeProgress < 0.5f)
        {
            // Primera mitad del ciclo: día -> noche
            float t = timeProgress * 2.0f;
            dayIntensity = 1.0f - t;
            nightIntensity = t;
        }
        else
        {
            // Segunda mitad del ciclo: noche -> día
            float t = (timeProgress - 0.5f) * 2.0f;
            dayIntensity = t;
            nightIntensity = 1.0f - t;
        }

        glm::vec3 dayColor(1.0f, 0.95f, 0.8f);   // amarillo cálido
        glm::vec3 nightColor(0.4f, 0.4f, 0.6f);  // azul oscuro
        glm::vec3 currentColor = glm::mix(nightColor, dayColor, dayIntensity);

        directionalLight.setColor(currentColor);
        directionalLight.setAmbientIntensity(0.3f * (0.5f + dayIntensity * 0.5f));
        directionalLight.setDiffuseIntensity(0.8f * dayIntensity);

        // Renderizado
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Skybox
        skybox.draw(camera.calculateViewMatrix(), projection);

        // Configurar shader principal
        shader.use();

        glm::mat4 view = camera.calculateViewMatrix();
        glUniformMatrix4fv(shader.getProjectionLocation(), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(shader.getViewLocation(), 1, GL_FALSE, glm::value_ptr(view));
        glm::vec3 eye = camera.getPosition();
        glUniform3f(shader.getEyePositionLocation(), eye.x, eye.y, eye.z);
        glUniform3f(shader.getColorLocation(), 1.0f, 1.0f, 1.0f);
        glUniform2f(shader.getTextureOffsetLocation(), 0.0f, 0.0f);

        shader.setDirectionalLight(&directionalLight);

        // Objetos de la escena
        floorObj->draw(shader);

        // Render del castillo
        castle.GetCastleObject()->draw(shader);

        // Render del tren
        train.GetTrainObject()->draw(shader);

        // --- INTEGRACION NAVE ---
        // Render de la nave usando el location del modelo del nuevo shader
		nave.GetNaveObject()->draw(shader);
        // ------------------------

        // Ruby
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        rubyObj->draw(shader);
        glDisable(GL_BLEND);

        glUseProgram(0);
        mainWindow.swapBuffers();
    }

    audioManager.shutdown();
    return 0;
}