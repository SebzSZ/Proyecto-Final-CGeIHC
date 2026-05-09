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

// Rutas de recursos
static const char* VERT_SHADER = "shaders/shader.vert";
static const char* FRAG_SHADER = "shaders/shader.frag";
static const char* SKYBOX_VERT = "shaders/skybox.vert";
static const char* SKYBOX_FRAG = "shaders/skybox.frag";

// Plano del piso
static GLfloat FLOOR_VERTS[] = {
    //  x      y      z      u      v      nx    ny    nz
       -1.0f,  0.0f, -1.0f,  0.0f, 10.0f,  0.0f, 1.0f, 0.0f,
        1.0f,  0.0f, -1.0f, 10.0f, 10.0f,  0.0f, 1.0f, 0.0f,
        1.0f,  0.0f,  1.0f, 10.0f,  0.0f,  0.0f, 1.0f, 0.0f,
       -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f, 0.0f,
};
static GLuint FLOOR_IDX[] = { 0, 1, 2,  0, 2, 3 };

std::shared_ptr<GameObject> CreateCastle(Material& matOpaco, Model& floor, Model& mainRoom, Model& bigTower, Model& wall, Model& midTower)
{
    // Container
    std::shared_ptr<GameObject> castleObj = std::make_shared<GameObject>("Castle Container", GameObjectType::MODEL);

    // Main Room
    std::shared_ptr<GameObject> mainRoomObj = std::make_shared<GameObject>("Main Room", GameObjectType::MODEL);
    mainRoomObj->setModel(&mainRoom);
    mainRoomObj->setMaterial(&matOpaco);
    mainRoomObj->transform.setPosition(20.0f, 0.0f, 1.64f);
    mainRoomObj->transform.setRotation(0.0f, 180.0f, 0.0f);
    mainRoomObj->transform.setScale(120.0f);
	castleObj->addChild(mainRoomObj);

    // Wall 1
    std::shared_ptr<GameObject> wall1Obj = std::make_shared<GameObject>("Wall 1", GameObjectType::MODEL);
    wall1Obj->setModel(&wall);
    wall1Obj->setMaterial(&matOpaco);
    wall1Obj->transform.setPosition(6.9f, 0.0f, 0.0f);
    wall1Obj->transform.setRotation(0.0f, -90.0f, 0.0f);
    wall1Obj->transform.setScale(60.0f);
	castleObj->addChild(wall1Obj);

	// Wall 2
	std::shared_ptr<GameObject> wall2Obj = std::make_shared<GameObject>("Wall 2", GameObjectType::MODEL);
	wall2Obj->setModel(&wall);
	wall2Obj->setMaterial(&matOpaco);
	wall2Obj->transform.setPosition(33.21f, 0.0f, 0.0f);
	wall2Obj->transform.setRotation(0.0f, -90.0f, 0.0f);
	wall2Obj->transform.setScale(60.0f);
	castleObj->addChild(wall2Obj);

	// Wall 3
	std::shared_ptr<GameObject> wall3Obj = std::make_shared<GameObject>("Wall 3", GameObjectType::MODEL);
	wall3Obj->setModel(&wall);
	wall3Obj->setMaterial(&matOpaco);
	wall3Obj->transform.setPosition(40.9f, 0.0f, -7.55f);
	wall3Obj->transform.setRotation(0.0f, 0.0f, 0.0f);
	wall3Obj->transform.setScale(60.0f);
	castleObj->addChild(wall3Obj);

	// Wall 4
	std::shared_ptr<GameObject> wall4Obj = std::make_shared<GameObject>("Wall 4", GameObjectType::MODEL);
	wall4Obj->setModel(&wall);
	wall4Obj->setMaterial(&matOpaco);
	wall4Obj->transform.setPosition(20.27f, 0.0f, -15.21f);
	wall4Obj->transform.setRotation(0.0f, 90.0f, 0.0f);
	wall4Obj->transform.setScale(glm::vec3(60.0f, 60.0f, 165.0f));
	castleObj->addChild(wall4Obj);

	// Wall 5
	std::shared_ptr<GameObject> wall5Obj = std::make_shared<GameObject>("Wall 5", GameObjectType::MODEL);
	wall5Obj->setModel(&wall);
	wall5Obj->setMaterial(&matOpaco);
	wall5Obj->transform.setPosition(-0.73f, 0.0f, -7.66f);
	wall5Obj->transform.setRotation(0.0f, 180.0f, 0.0f);
	wall5Obj->transform.setScale(60.0f);
	castleObj->addChild(wall5Obj);

	// Wall 6
	std::shared_ptr<GameObject> wall6Obj = std::make_shared<GameObject>("Wall 6", GameObjectType::MODEL);
	wall6Obj->setModel(&wall);
	wall6Obj->setMaterial(&matOpaco);
	wall6Obj->transform.setPosition(20.0f, 3.96f, -6.58f);
	wall6Obj->transform.setRotation(0.0f, -90.0f, 0.0f);
	wall6Obj->transform.setScale(60.0f);
	castleObj->addChild(wall6Obj);

	// Wall 7
	std::shared_ptr<GameObject> wall7Obj = std::make_shared<GameObject>("Wall 7", GameObjectType::MODEL);
	wall7Obj->setModel(&wall);
	wall7Obj->setMaterial(&matOpaco);
	wall7Obj->transform.setPosition(20.1f, 3.96f, -12.91f);
	wall7Obj->transform.setRotation(0.0f, 90.0f, 0.0f);
	wall7Obj->transform.setScale(60.0f);
	castleObj->addChild(wall7Obj);

	// Wall 8
	std::shared_ptr<GameObject> wall8Obj = std::make_shared<GameObject>("Wall 8", GameObjectType::MODEL);
	wall8Obj->setModel(&wall);
	wall8Obj->setMaterial(&matOpaco);
	wall8Obj->transform.setPosition(27.7f, 3.96f, -9.8f);
	wall8Obj->transform.setRotation(0.0f, 0.0f, 0.0f);
	wall8Obj->transform.setScale(60.0f);
	wall8Obj->transform.setScale(glm::vec3(60.0f, 60.0f, 24.89f));
	castleObj->addChild(wall8Obj);

	// Wall 9
	std::shared_ptr<GameObject> wall9Obj = std::make_shared<GameObject>("Wall 9", GameObjectType::MODEL);
	wall9Obj->setModel(&wall);
	wall9Obj->setMaterial(&matOpaco);
	wall9Obj->transform.setPosition(12.4f, 3.96f, -9.8f);
	wall9Obj->transform.setRotation(0.0f, 180.0f, 0.0f);
	wall9Obj->transform.setScale(glm::vec3(60.0f, 60.0f, 24.89f));
	castleObj->addChild(wall9Obj);

	// Big Tower 1
	std::shared_ptr<GameObject> bigTower1Obj = std::make_shared<GameObject>("Big Tower 1", GameObjectType::MODEL);
	bigTower1Obj->setModel(&bigTower);
	bigTower1Obj->setMaterial(&matOpaco);
	bigTower1Obj->transform.setPosition(16.84f, 8.92f, -9.0f);
	bigTower1Obj->transform.setScale(60.0f);
	castleObj->addChild(bigTower1Obj);

	// Big Tower 2
	std::shared_ptr<GameObject> bigTower2Obj = std::make_shared<GameObject>("Big Tower 2", GameObjectType::MODEL);
	bigTower2Obj->setModel(&bigTower);
	bigTower2Obj->setMaterial(&matOpaco);
	bigTower2Obj->transform.setPosition(24.65f, 0.53f, -9.47f);
	bigTower2Obj->transform.setScale(60.0f);
	castleObj->addChild(bigTower2Obj);

	// Mid Tower 1
	std::shared_ptr<GameObject> midTower1Obj = std::make_shared<GameObject>("Mid Tower 1", GameObjectType::MODEL);
	midTower1Obj->setModel(&midTower);
	midTower1Obj->setMaterial(&matOpaco);
	midTower1Obj->transform.setPosition(36.09f, 5.44f, -3.64f);
	midTower1Obj->transform.setRotation(0.0f, -90.0f, 0.0f);
	midTower1Obj->transform.setScale(50.0f);
	castleObj->addChild(midTower1Obj);

	// Mid Tower 2
	std::shared_ptr<GameObject> midTower2Obj = std::make_shared<GameObject>("Mid Tower 2", GameObjectType::MODEL);
	midTower2Obj->setModel(&midTower);
	midTower2Obj->setMaterial(&matOpaco);
	midTower2Obj->transform.setPosition(36.09f, 5.44f, -11.0f);
	midTower2Obj->transform.setRotation(0.0f, -90.0f, 0.0f);
	midTower2Obj->transform.setScale(50.0f);
	castleObj->addChild(midTower2Obj);

	// Mid Tower 3
	std::shared_ptr<GameObject> midTower3Obj = std::make_shared<GameObject>("Mid Tower 3", GameObjectType::MODEL);
	midTower3Obj->setModel(&midTower);
	midTower3Obj->setMaterial(&matOpaco);
	midTower3Obj->transform.setPosition(3.69f, 5.44f, -3.64f);
	midTower3Obj->transform.setRotation(0.0f, -90.0f, 0.0f);
	midTower3Obj->transform.setScale(50.0f);
	castleObj->addChild(midTower3Obj);

	// Mid Tower 4
	std::shared_ptr<GameObject> midTower4Obj = std::make_shared<GameObject>("Mid Tower 4", GameObjectType::MODEL);
	midTower4Obj->setModel(&midTower);
	midTower4Obj->setMaterial(&matOpaco);
	midTower4Obj->transform.setPosition(3.69f, 5.44f, -11.0f);
	midTower4Obj->transform.setRotation(0.0f, -90.0f, 0.0f);
	midTower4Obj->transform.setScale(50.0f);
	castleObj->addChild(midTower4Obj);

	// Floor 1
	std::shared_ptr<GameObject> floor1Obj = std::make_shared<GameObject>("Floor 1", GameObjectType::MODEL);
	floor1Obj->setModel(&floor);    
	floor1Obj->setMaterial(&matOpaco);
	floor1Obj->transform.setPosition(33.59f, 5.28f, -8.34f);
	floor1Obj->transform.setScale(58.3f, 1.0f, 76.2f);
	castleObj->addChild(floor1Obj);

	// Floor 2
	std::shared_ptr<GameObject> floor2Obj = std::make_shared<GameObject>("Floor 2", GameObjectType::MODEL);
	floor2Obj->setModel(&floor);
	floor2Obj->setMaterial(&matOpaco);
	floor2Obj->transform.setPosition(6.43f, 5.28f, -8.34f);
	floor2Obj->transform.setScale(58.3f, 1.0f, 76.2f);
	castleObj->addChild(floor2Obj);

	// Floor 3
	std::shared_ptr<GameObject> floor3Obj = std::make_shared<GameObject>("Floor 3", GameObjectType::MODEL);
	floor3Obj->setModel(&floor);
	floor3Obj->setMaterial(&matOpaco);
	floor3Obj->transform.setPosition(20.0f, 5.28f, -10.2f);
	floor3Obj->transform.setScale(50.1f, 1.0f, 55.15f);
	castleObj->addChild(floor3Obj);

	// Floor 4
	std::shared_ptr<GameObject> floor4Obj = std::make_shared<GameObject>("Floor 4", GameObjectType::MODEL);
	floor4Obj->setModel(&floor);
	floor4Obj->setMaterial(&matOpaco);
	floor4Obj->transform.setPosition(19.75f, 9.28f, -10.0f);
	floor4Obj->transform.setScale(60.0f, 1.0f, 31.1f);
	castleObj->addChild(floor4Obj);

    return castleObj;
}

int main()
{
	// Crear ventana
    Window mainWindow("Proyecto Final - CGeIHC");
    if (mainWindow.Initialize() != 0) return -1;

	AudioManager& audioManager = AudioManager::getInstance();
	if (!audioManager.Initialize()) return -1;

	audioManager.loadMP3("bgMusic", "Sounds/bgMusic.mp3");
	audioManager.play("bgMusic", true, 0.5f);

    // Configurar cámara
    Camera camera(glm::vec3(0.0f, 5.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.1f);

    // Iniciar en modo 3era persona
    camera.setCameraMode(CameraMode::THIRD_PERSON);

    // Creación de shader
    Shader shader;
    shader.createFromFiles(VERT_SHADER, FRAG_SHADER);

	// Creación del skybox con texturas de día y noche
	Skybox skybox;
	skybox.create(
		// Texturas de día
		{
			"Textures/Skybox/cupertin-lake_rt.tga",
			"Textures/Skybox/cupertin-lake_lf.tga",
			"Textures/Skybox/cupertin-lake_up.tga",
			"Textures/Skybox/cupertin-lake_dn.tga",
			"Textures/Skybox/cupertin-lake_bk.tga",
			"Textures/Skybox/cupertin-lake_ft.tga"
		},
		// Texturas de noche (las mismas por ahora, puedes reemplazarlas con texturas nocturnas)
		{
			"Textures/Skybox/cupertin-lake_rt.tga",
			"Textures/Skybox/cupertin-lake_lf.tga",
			"Textures/Skybox/cupertin-lake_up.tga",
			"Textures/Skybox/cupertin-lake_dn.tga",
			"Textures/Skybox/cupertin-lake_bk.tga",
			"Textures/Skybox/cupertin-lake_ft.tga"
		},
		SKYBOX_VERT, SKYBOX_FRAG
	);

	// Configurar duración del ciclo día-noche (30 segundos para el ciclo completo)
	skybox.setDayNightCycleDuration(30.0f);

    // Crear piso con Mesh
    Mesh floorMesh;
    floorMesh.create(FLOOR_VERTS, FLOOR_IDX, 32, 6);

    Texture floorTexture("Textures/piso.tga");
    floorTexture.loadWithAlpha();

	Material matOpaco(0.2f, 4.0f);

	// Ruby
	Model rubyModel;
	if (!rubyModel.load("Models/RubyCuerpo.obj")) return -1;
	Model rubyLeftArm;
	if (!rubyLeftArm.load("Models/RubyBrazoIzq.obj")) return -1;
	Model rubyRightArm;
	if (!rubyRightArm.load("Models/RubyBrazoDer.obj")) return -1;
	Model rubyLeftLeg;
	if (!rubyLeftLeg.load("Models/RubyPiernaIzq.obj")) return -1;
	Model rubyRightLeg;
	if (!rubyRightLeg.load("Models/RubyPiernaDer.obj")) return -1;

    Model mainRoom;
	if (!mainRoom.load("Models/HyruleCastle_MainRoom.obj")) return -1;

    Model bigTower;
	if (!bigTower.load("Models/HyruleCastle_BigTower.obj")) return -1;

    Model wall;
	if (!wall.load("Models/HyruleCastle_Wall.obj")) return -1;

    Model midTower;
	if (!midTower.load("Models/HyruleCastle_MidTower.obj")) return -1;

    Model floor;
	if (!floor.load("Models/HyruleCastle_Floor.obj")) return -1;

    Material matBrillante(1.0f, 32.0f);
    
    // Objeto Piso
    std::shared_ptr<GameObject> floorObj = std::make_shared<GameObject>("Floor", GameObjectType::MESH);
    MeshData floorData;
    floorData.vertices = std::vector<GLfloat>(std::begin(FLOOR_VERTS), std::end(FLOOR_VERTS));
    floorData.indices = std::vector<GLuint>(std::begin(FLOOR_IDX), std::end(FLOOR_IDX));
    floorObj->loadMesh(floorData);
    floorObj->setTextureID(floorTexture.getID());
    floorObj->setMaterial(&matOpaco);
    floorObj->transform.setScale(50.0f, 1.0f, 50.0f);

    // Objeto Ruby
    std::shared_ptr<GameObject> rubyObj = std::make_shared<GameObject>("Ruby", GameObjectType::MODEL);
    rubyObj->setModel(&rubyModel);
    rubyObj->setMaterial(&matOpaco);
    rubyObj->transform.setPosition(0.0f, 1.0f, 0.0f);
    rubyObj->transform.setScale(5.0f);

	// Objeto Castillo
    std::shared_ptr<GameObject> castleObj = CreateCastle(matOpaco, floor, mainRoom, bigTower, wall, midTower);

	// Brazos y piernas de Ruby como hijos del objeto principal
	std::shared_ptr<GameObject> leftArm = std::make_shared<GameObject>("LeftArm", GameObjectType::MODEL);
	leftArm->setModel(&rubyLeftArm);
	leftArm->transform.setPosition(0.025f, 0.0611f, 0.0f);
	leftArm->transform.setRotation(0.0f, 0.0f, -65.0f);
	rubyObj->addChild(leftArm);

	std::shared_ptr<GameObject> rightArm = std::make_shared<GameObject>("RightArm", GameObjectType::MODEL);
	rightArm->setModel(&rubyRightArm);
	rightArm->transform.setPosition(-0.025f, 0.0611f, 0.0f);
	rightArm->transform.setRotation(0.0f, 0.0f, 65.0f);
	rubyObj->addChild(rightArm);

	std::shared_ptr<GameObject> leftLeg = std::make_shared<GameObject>("LeftLeg", GameObjectType::MODEL);
	leftLeg->setModel(&rubyLeftLeg);
	rubyObj->addChild(leftLeg);

	std::shared_ptr<GameObject> rightLeg = std::make_shared<GameObject>("RightLeg", GameObjectType::MODEL);
	rightLeg->setModel(&rubyRightLeg);
	rubyObj->addChild(rightLeg);

	// Atributo para animar los brazos de Ruby
	float walkTime = 0.0f;
	float walkSpeed = 10.0f;
	float walkAmplitude = 35.0f;
	float angleThigh = 0.0f;

    // Luz direccional
    DirectionalLight directionalLight(
        1.0f, 0.95f, 0.8f,
        0.3f, 0.8f,
        0.0f, -1.0f, -0.5f
    );

	// Linterna que sigue a la cámara
    SpotLight spotLights[MAX_SPOT_LIGHTS];
    unsigned int spotLightCount = 0;
    spotLights[0] = SpotLight(
        1.0f, 1.0f, 1.0f,
        0.4f, 1.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        1.0f, 0.005f, 0.0f,
        15.0f
	);
	spotLightCount++;

	// Configurar puntos de interés (después de crear rubyObj)
	glm::vec3 rubyPos = rubyObj->transform.getPosition();
	// Punto 1: Vista frontal de Ruby
	camera.addInterestPoint(rubyPos + glm::vec3(0.0f, 2.0f, 5.0f), rubyPos + glm::vec3(0.0f, 1.0f, 0.0f));

	// Punto 2: Vista aérea general del escenario
	camera.addInterestPoint(glm::vec3(0.0f, 25.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	// Punto 3: Vista lateral de Ruby
	camera.addInterestPoint(rubyPos + glm::vec3(8.0f, 3.0f, 0.0f), rubyPos + glm::vec3(0.0f, 1.0f, 0.0f));

	// Proyección
    glm::mat4 projection = glm::perspective(
        glm::radians(60.0f),
        (float)mainWindow.getBufferWidth() / (float)mainWindow.getBufferHeight(),
        0.1f, 500.0f
    );

	// Matriz de modelo
	glm::mat4 model(1.0f);

    GLfloat lastTime = (GLfloat)glfwGetTime();

    // Loop principal
    while (!mainWindow.shouldClose())
    {
		// Tiempo
		GLfloat now = (GLfloat)glfwGetTime();
		GLfloat deltaTime = now - lastTime;
		lastTime = now;

		// Actualizar ciclo día-noche del skybox
		skybox.updateDayNightCycle(deltaTime);

		// Input
		glfwPollEvents();
		InputManager& input = InputManager::getInstance();
		input.beginFrame();

		// Cambio de modo de cámara
		if (input.isKeyPressed(GLFW_KEY_1))
		{
			camera.setCameraMode(CameraMode::THIRD_PERSON);
			printf("[Camera] Modo: 3era Persona (Sigue el personaje)\n");
			printf("[Orbita] Usa Q/E o el raton para rotar la camara alrededor de Ruby\n");
		}
		if (input.isKeyPressed(GLFW_KEY_2))
		{
			camera.setCameraMode(CameraMode::AERIAL);
			printf("[Camera] Modo: Aéreo (Exploración libre)\n");
		}
		if (input.isKeyPressed(GLFW_KEY_3))
		{
			camera.setCameraMode(CameraMode::INTEREST_POINT);
			printf("[Camera] Modo: Puntos de Interés\n");
		}

		// Cambiar punto de interés con SPACE
		if (input.isKeyPressed(GLFW_KEY_SPACE) && camera.getCameraMode() == CameraMode::INTEREST_POINT)
		{
			camera.nextInterestPoint();
		}

		// Actualizar cámara según modo
		switch (camera.getCameraMode())
		{
			case CameraMode::THIRD_PERSON:
				camera.mouseControl(input.getMouseDeltaX() * 0.5f, input.getMouseDeltaY() * 0.5f);
				camera.updateThirdPersonCamera(rubyObj->transform.getPosition(), deltaTime);
				break;
			case CameraMode::AERIAL:
				// TODO: Implementar cámara aérea
				break;
			case CameraMode::INTEREST_POINT:
				// TODO: Implementar cámara de puntos de interés
				break;
		}

		// Controles de Ruby 
		// Se mueve en la dirección hacia donde apunta la cámara
		float rubyMoveSpeed = 8.0f;
		glm::vec3 cameraDirection = camera.getDirection();

		// Proyectar la dirección de la cámara al plano XZ (ignorar Y)
		glm::vec3 moveDirection = glm::normalize(glm::vec3(cameraDirection.x, 0.0f, cameraDirection.z));

		// Calcular dirección perpendicular (izquierda/derecha)
		glm::vec3 rightDirection = glm::normalize(glm::cross(moveDirection, glm::vec3(0.0f, 1.0f, 0.0f)));

		bool isWalking = false;

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

		// Animar brazos de Ruby al caminar
		if (isWalking)
		{
			walkTime += deltaTime * walkSpeed;
			angleThigh = walkAmplitude * sin(walkTime);
		}
		else
		{
			walkTime = 0.0f;
			// Regreso suave: disminuir el ángulo hacia 0 gradualmente usando interpolación lineal
			angleThigh += (0.0f - angleThigh) * 8.0f * deltaTime;
		}

		leftArm->transform.setRotation(angleThigh, 0.0f, -65.0f);
		rightArm->transform.setRotation(-angleThigh, 0.0f, 65.0f);
		leftLeg->transform.setRotation(-angleThigh, 0.0f, 0.0f);
		rightLeg->transform.setRotation(angleThigh, 0.0f, 0.0f);

		// Hacer girar a Ruby hacia donde está mirando la cámara
		glm::vec3 rubyDirection = glm::normalize(glm::vec3(cameraDirection.x, 0.0f, cameraDirection.z));
		float angleY = atan2(rubyDirection.x, rubyDirection.z);
		rubyObj->transform.setRotation(0.0f, glm::degrees(angleY), 0.0f);

		// Actualizar linterna con posición y dirección de la cámara
		spotLights[0].setFlash(camera.getPosition(), camera.getDirection());

        // Ajustar intensidad de luces según ciclo día-noche
        float timeProgress = skybox.getTimeProgress();
        float dayIntensity, nightIntensity;

        if (timeProgress < 0.5f)
        {
            // Transición: día -> noche
            float t = timeProgress * 2.0f; // 0 a 1
            dayIntensity = 1.0f - t;      // 1 -> 0
            nightIntensity = t;            // 0 -> 1
        }
        else
        {
            // Transición: noche -> día
            float t = (timeProgress - 0.5f) * 2.0f; // 0 a 1
            dayIntensity = t;              // 0 -> 1
            nightIntensity = 1.0f - t;     // 1 -> 0
        }

        // Ajustar luz direccional (luz del sol/luna)
        // De día: luz amarilla brillante, de noche: luz azul oscura
        glm::vec3 dayColor(1.0f, 0.95f, 0.8f);
        glm::vec3 nightColor(0.4f, 0.4f, 0.6f);
        glm::vec3 currentColor = glm::mix(nightColor, dayColor, dayIntensity);

        directionalLight.setColor(currentColor);
        directionalLight.setAmbientIntensity(0.3f * (0.5f + dayIntensity * 0.5f));
        directionalLight.setDiffuseIntensity(0.8f * dayIntensity);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Skybox
        skybox.draw(camera.calculateViewMatrix(), projection);

        // Activar shader principal
        shader.use();

        glm::mat4 view = camera.calculateViewMatrix();
        glUniformMatrix4fv(shader.getProjectionLocation(), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(shader.getViewLocation(), 1, GL_FALSE, glm::value_ptr(view));
        glm::vec3 eye = camera.getPosition();
        glUniform3f(shader.getEyePositionLocation(), eye.x, eye.y, eye.z);
        glUniform3f(shader.getColorLocation(), 1.0f, 1.0f, 1.0f);
        glUniform2f(shader.getTextureOffsetLocation(), 0.0f, 0.0f);

        // Enviar luces
        shader.setDirectionalLight(&directionalLight);
        shader.setSpotLights(spotLights, spotLightCount);

		// Renderizar objetos
		floorObj->draw(shader);

		//naveObj->draw(shader);

		// Ruby con alpha blend
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		rubyObj->draw(shader);
		glDisable(GL_BLEND);

        // Render del castillo
		castleObj->draw(shader);

		glUseProgram(0);
        mainWindow.swapBuffers();
    }
    audioManager.shutdown();
    return 0;
}