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
#include "PointLight.h"
#include "Shader.h"
#include "Skybox.h"
#include "SpotLight.h"
#include "Texture.h"
#include "Window.h"
#include "GameObject.h"
#include "Train.h"
#include "Castle.h"
#include "Malon.h"
#include "Nave.h"
#include "Tree.h"

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

// Luces
unsigned int pointLightCount = 0;
PointLight pointLights[MAX_POINT_LIGHTS];

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
    std::shared_ptr<GameObject> rubyObj = std::make_shared<GameObject>("Ruby");
    rubyObj->setModel(&rubyModel);
    rubyObj->setMaterial(&matOpaco);
    rubyObj->transform.setPosition(20.0f, 1.2f, 1.64f);
    rubyObj->transform.setScale(5.0f);

    // Extremidades — hijos de rubyObj
    leftArm = std::make_shared<GameObject>("LeftArm");
    leftArm->setModel(&rubyLeftArm);
    leftArm->transform.setPosition(0.025f, 0.0611f, 0.0f);
    leftArm->transform.setRotation(0.0f, 0.0f, -65.0f);
    rubyObj->addChild(leftArm);

    rightArm = std::make_shared<GameObject>("RightArm");
    rightArm->setModel(&rubyRightArm);
    rightArm->transform.setPosition(-0.025f, 0.0611f, 0.0f);
    rightArm->transform.setRotation(0.0f, 0.0f, 65.0f);
    rubyObj->addChild(rightArm);

    leftLeg = std::make_shared<GameObject>("LeftLeg");
    leftLeg->setModel(&rubyLeftLeg);
    rubyObj->addChild(leftLeg);

    rightLeg = std::make_shared<GameObject>("RightLeg");
    rightLeg->setModel(&rubyRightLeg);
    rubyObj->addChild(rightLeg);

    cape = std::make_shared<GameObject>("Cape");
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
    audioManager.play("bgMusic", true, 0.5f);

    // Cargar sonido de Malon
    audioManager.loadWAV("malonSound", "Sounds/malonSound.wav");

    // Cargar sonido de Opera
    audioManager.loadMP3("operaSound", "Sounds/operaMusic.mp3");

    // Cargar sonido del Tren (usa un alias del sonido de Malon)
    audioManager.loadWAV("trainSound", "Sounds/trainSound.wav");

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

    // GameObjects de la escena

    // Piso de la escena
    std::shared_ptr<GameObject> floorObj = std::make_shared<GameObject>("Floor");
    Mesh* floorMesh = new Mesh();
    floorMesh->create(FLOOR_VERTS, FLOOR_IDX, sizeof(FLOOR_VERTS) / sizeof(GLfloat), sizeof(FLOOR_IDX) / sizeof(GLuint));
    floorObj->setMesh(floorMesh);
    floorObj->setTextureID(floorTexture.getID());
    floorObj->setMaterial(&matOpaco);
    floorObj->transform.setScale(100.0f, 1.0f, 100.0f);
    floorObj->transform.setPosition(20.0f, 0.0f, 0.0f);

    // Deku Tree
    Model dekuTreeModel; if (!dekuTreeModel.load("Models/DekuTree.obj")) return -1;
    std::shared_ptr<GameObject> dekuTreeObj = std::make_shared<GameObject>("DekuTree");
    dekuTreeObj->setModel(&dekuTreeModel);
    dekuTreeObj->setMaterial(&matOpaco);
    dekuTreeObj->transform.setPosition(-12.9f, 0.0f, 12.9f);
    dekuTreeObj->transform.setRotation(0.0f, 65.6f, 0.0f);
    dekuTreeObj->transform.setScale(0.03f);

    // Columbina — colocada en el interior de la Ópera Sinner's Finale
    Model columbina; if (!columbina.load("Models/Columbina.obj")) return -1;
    std::shared_ptr<GameObject> columbinaObj = std::make_shared<GameObject>("Columbina");
    columbinaObj->setModel(&columbina);
    columbinaObj->setMaterial(&matOpaco);
    columbinaObj->transform.setPosition(90.0f, 0.9f, 42.5f);  // frente al escenario de la ópera
    columbinaObj->transform.setRotation(0.0f, 180.0f, 0.0f);  // mirando hacia el interior
    columbinaObj->transform.setScale(0.5f);

    // Beacon Academy
    Model beaconAcademy; if (!beaconAcademy.load("Models/Beacon.obj")) return -1;
    std::shared_ptr<GameObject> beaconAcademyObj = std::make_shared<GameObject>("BeaconAcademy");
    beaconAcademyObj->setModel(&beaconAcademy);
    beaconAcademyObj->setMaterial(&matOpaco);
    beaconAcademyObj->transform.setPosition(20.5f, -0.2f, 70.0f);
    beaconAcademyObj->transform.setRotation(-90.0f, 0.0f, -90.0f);
    beaconAcademyObj->transform.setScale(0.01f);

    // Opera Sinner's Finale
    Model sinnersFinale; if (!sinnersFinale.load("Models/Opera.obj")) return -1;
    std::shared_ptr<GameObject> sinnersFinaleObj = std::make_shared<GameObject>("SinnersFinale");
    sinnersFinaleObj->setModel(&sinnersFinale);
    sinnersFinaleObj->setMaterial(&matBrillante);
    sinnersFinaleObj->transform.setPosition(90.0f, 0.8f, 45.0f);
    sinnersFinaleObj->transform.setScale(0.2f);

    Malon malon;
    if (!malon.Initialize(matOpaco, matBrillante)) return -1;

    Castle castle;
    if (!castle.Initialize(matOpaco, pointLights, pointLightCount)) return -1;
    castle.GetCastleObject()->transform.setScale(2.0f, 2.0f, 2.0f);
    castle.GetCastleObject()->transform.setPosition(-20.0f, -1.0f, 0.0f);

    Train train;
    if (!train.Initialize(matOpaco)) return -1;

    float trainSpeed = 5.0f;
    float wheelRotationSpeed = 200.0f;

    // Nave Keyframes
    Nave nave;
    if (!nave.Initialize(matOpaco)) return -1;
    nave.GetNaveObject()->transform.setScale(3.0f);
    nave.GetNaveObject()->transform.setPosition(20.0f, 10.0f, 70.0f);
    nave.SetKeyframesIniciales();
    nave.DisplayMenu();

    // Árboles
    Tree tree;
    if (!tree.Initialize(matOpaco)) return -1;

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

    // Punto de Interés 1: Deku Tree
    // Vista frontal ligeramente elevada del Gran Árbol Deku
    glm::vec3 dekuTreePos(-12.9f, 0.0f, 12.9f);
    camera.addInterestPoint(
        dekuTreePos + glm::vec3(14.0f, 12.0f, 14.0f), // cámara: más alejada y elevada
        dekuTreePos + glm::vec3(0.0f, 6.0f, 0.0f)     // mira a la copa del árbol
    );

    // Punto de Interés 2: Ópera Sinner's Finale
    // Vista general de la fachada de la Ópera
    glm::vec3 operaPos(90.0f, 0.8f, 45.0f);
    camera.addInterestPoint(
        operaPos + glm::vec3(-18.0f, 14.0f, 18.0f),  // cámara: diagonal frontal más elevada
        operaPos + glm::vec3(0.0f, 5.0f, 0.0f)       // mira al centro de la fachada
    );

    // Punto de Interés 3: Beacon Academy
    // Vista que muestra la escala monumental del edificio
    glm::vec3 beaconPos(20.5f, -0.2f, 70.0f);
    camera.addInterestPoint(
        beaconPos + glm::vec3(20.0f, 16.0f, -20.0f), // cámara: frente más elevada
        beaconPos + glm::vec3(0.0f, 8.0f, 0.0f)      // mira a la parte central del edificio
    );

    // Proyección
    glm::mat4 projection = glm::perspective(glm::radians(60.0f), (float)mainWindow.getBufferWidth() / (float)mainWindow.getBufferHeight(), 0.1f, 500.0f);

    // Matriz de modelo
    glm::mat4 model(1.0f);
    GLfloat lastTime = (GLfloat)glfwGetTime();

    // Estado de las antorchas
    bool torchesOn = true;

    // Estado de audio de Malon
    bool malonAudioActive = false;
    float MALON_PROXIMITY_RANGE = 15.0f;
    float FADE_DURATION = 1.75f;

    // Estado de audio de Opera
    bool operaAudioActive = false;
    float OPERA_PROXIMITY_RANGE = 20.0f;

    // Estado de audio del Tren
    bool trainAudioActive = false;
    float TRAIN_PROXIMITY_RANGE = 10.0f;

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

        // Toggle para apagar o encender las luces del castillo
        if (input.isKeyPressed(GLFW_KEY_T))
        {
            torchesOn = !torchesOn;

            GLfloat targetAmbient = torchesOn ? 0.2f : 0.0f;
            GLfloat targetDiffuse = torchesOn ? 1.0f : 0.0f;

            for (unsigned int i = 0; i < pointLightCount; i++)
            {
                pointLights[i].setAmbientIntensity(targetAmbient);
                pointLights[i].setDiffuseIntensity(targetDiffuse);
            }
        }

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

        train.Update(trainSpeed, deltaTime, wheelRotationSpeed);
        malon.Update(deltaTime);
        nave.Update(deltaTime);

        // Verificar proximidad a Malon
        glm::vec3 malonPos = malon.GetPosition();
        float distanceToMalon = glm::distance(rubyPos, malonPos);
        bool shouldPlayMalonSound = distanceToMalon < MALON_PROXIMITY_RANGE;

        // Gestionar audio de Malon
        if (shouldPlayMalonSound && !malonAudioActive)
        {
            // Entrar en rango: fade out de música y fade in de sonido de Malon
            audioManager.fadeOut("bgMusic", FADE_DURATION);
            audioManager.fadeIn("malonSound", FADE_DURATION, true);
            malonAudioActive = true;
        }
        else if (!shouldPlayMalonSound && malonAudioActive)
        {
            // Salir de rango: fade out de sonido de Malon y reanudar música
            audioManager.fadeOut("malonSound", FADE_DURATION);
            audioManager.fadeIn("bgMusic", FADE_DURATION, true, true);
            malonAudioActive = false;
        }

        // Actualizar posición 3D del sonido de Malon
        audioManager.setSourcePosition("malonSound", malonPos);

        // Verificar proximidad a Opera Sinner's Finale
        glm::vec3 operaPos = sinnersFinaleObj->transform.getPosition();
        float distanceToOpera = glm::distance(rubyPos, operaPos);
        bool shouldPlayOperaSound = distanceToOpera < OPERA_PROXIMITY_RANGE;

        // Gestionar audio de Opera
        if (shouldPlayOperaSound && !operaAudioActive)
        {
            // Entrar en rango: fade out de música y fade in de sonido de Opera
            audioManager.fadeOut("bgMusic", FADE_DURATION);
            audioManager.fadeIn("operaSound", FADE_DURATION, true, true);
            operaAudioActive = true;
        }
        else if (!shouldPlayOperaSound && operaAudioActive)
        {
            // Salir de rango: fade out de sonido de Opera y reanudar música
            audioManager.fadeOut("operaSound", FADE_DURATION);
            audioManager.fadeIn("bgMusic", FADE_DURATION, true, true);
            operaAudioActive = false;
        }

        // Actualizar posición 3D del sonido de Opera
        audioManager.setSourcePosition("operaSound", operaPos);

        // Verificar proximidad al Tren
        glm::vec3 trainPos = train.GetPosition();
        float distanceToTrain = glm::distance(rubyPos, trainPos);
        bool shouldPlayTrainSound = distanceToTrain < TRAIN_PROXIMITY_RANGE;

        // Gestionar audio del Tren (no interrumpe la música, solo se superpone)
        if (shouldPlayTrainSound && !trainAudioActive)
        {
            audioManager.fadeIn("trainSound", FADE_DURATION, true, true);
            trainAudioActive = true;
        }
        else if (!shouldPlayTrainSound && trainAudioActive)
        {
            audioManager.fadeOut("trainSound", FADE_DURATION);
            trainAudioActive = false;
        }

        // Actualizar posición 3D del sonido del Tren
        audioManager.setSourcePosition("trainSound", trainPos);

        // Actualizar posición del listener (cámara)
        audioManager.setListenerPosition(camera.getPosition(), camera.getDirection(), glm::vec3(0.0f, 1.0f, 0.0f));

        // Actualizar sistema de audio (fade in/out)
        audioManager.update(deltaTime);

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

        // Luz direccional — ciclo día/noche con movimiento solar
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

        // Rotación solar
        {
            float sunAngle = timeProgress * glm::two_pi<float>(); // 0..2π
            // El sol gira en el plano XY: de Este (+X) sube hacia el cenit (-Y) y cae hacia Oeste (-X)
            float sunX = glm::cos(sunAngle);          //  1 al amanecer, -1 al atardecer
            float sunY = -glm::sin(sunAngle);         // -1 al mediodía (apunta hacia abajo)
            float sunZ = -0.3f;                       // leve inclinación Norte-Sur constante
            directionalLight.setDirection(sunX, sunY, sunZ);
        }

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
        shader.setPointLights(pointLights, pointLightCount);

        // Piso
        floorObj->draw(shader);

        // Hyrule Castle
        castle.GetCastleObject()->draw(shader);

        // Tren
        train.GetTrainObject()->draw(shader);

        // Nave
        nave.GetNaveObject()->draw(shader);

        // Deku Tree
        dekuTreeObj->draw(shader);

        // Malon y Nimbus 2000
        malon.GetMalonObject()->draw(shader);

        // Columbina
        columbinaObj->draw(shader);

        // Beacon Academy
        beaconAcademyObj->draw(shader);

        // Opera Sinner's Finale
        sinnersFinaleObj->draw(shader);

        // Árboles
        for (const auto& treeObj : tree.GetTreeObjects())
        {
            treeObj->draw(shader);
        }

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