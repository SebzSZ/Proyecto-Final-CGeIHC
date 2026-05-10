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

int main()
{
	// Crear ventana
	Window mainWindow("Proyecto Final - CGeIHC");
	if (mainWindow.Initialize() != 0) return -1;

	AudioManager& audioManager = AudioManager::getInstance();
	if (!audioManager.Initialize()) return -1;

	audioManager.loadMP3("bgMusic", "Sounds/bgMusic.mp3");
	//audioManager.play("bgMusic", true, 0.5f);

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
		// Texturas de noche
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
	Material matBrillante(1.0f, 32.0f);

	// Nave
	Model nave;
	if (!nave.load("Models/nave.obj")) return -1;

	// Ruby
	Model rubyModel;
	if (!rubyModel.load("Models/ruby.obj")) return -1;

	// Objeto Piso
	std::shared_ptr<GameObject> floorObj = std::make_shared<GameObject>("Floor", GameObjectType::MESH);
	MeshData floorData;
	floorData.vertices = std::vector<GLfloat>(std::begin(FLOOR_VERTS), std::end(FLOOR_VERTS));
	floorData.indices = std::vector<GLuint>(std::begin(FLOOR_IDX), std::end(FLOOR_IDX));
	floorObj->loadMesh(floorData);
	floorObj->setTextureID(floorTexture.getID());
	floorObj->setMaterial(&matOpaco);
	floorObj->transform.setScale(50.0f, 1.0f, 50.0f);

	// Objeto Nave
	std::shared_ptr<GameObject> naveObj = std::make_shared<GameObject>("Nave", GameObjectType::MODEL);
	naveObj->setModel(&nave);
	naveObj->setMaterial(&matBrillante);
	naveObj->transform.setPosition(0.0f, 2.0f, -5.0f);
	naveObj->transform.setScale(2.5f);

	// Objeto Ruby
	std::shared_ptr<GameObject> rubyObj = std::make_shared<GameObject>("Ruby", GameObjectType::MODEL);
	rubyObj->setModel(&rubyModel);
	rubyObj->setMaterial(&matOpaco);
	rubyObj->transform.setPosition(0.0f, 1.0f, 0.0f);
	rubyObj->transform.setScale(5.0f);

	Castle castle;
	if (!castle.Initialize(matOpaco)) return -1;

	Train train;
	if (!train.Initialize(matOpaco)) return -1;

	float naveSpeed = 8.0f;
	float trainSpeed = 5.0f;
	float wheelRotationSpeed = 200.0f;

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

	// Faro rojo de la nave
	spotLights[1] = SpotLight(
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.1f, 0.03f,
		20.0f
	);
	spotLightCount++;

	std::shared_ptr<GameObject> naveFaroDir = std::make_shared<GameObject>("NaveFaro", GameObjectType::SPOT_LIGHT);
	naveFaroDir->setSpotLight(&spotLights[1]);
	naveFaroDir->transform.setPosition(0.0f, -0.5f, 0.0f);
	naveObj->addChild(naveFaroDir);

	// Configurar puntos de interés
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
			camera.updateThirdPersonCamera(rubyObj->transform.getPosition(), deltaTime);

			// Controles de órbita en modo 3era persona
			if (input.isKeyDown(GLFW_KEY_Q))
				camera.rotateOrbit(-150.0f * deltaTime); // Rotar izquierda
			if (input.isKeyDown(GLFW_KEY_E))
				camera.rotateOrbit(150.0f * deltaTime);  // Rotar derecha

			// Control con ratón (movimiento horizontal)
			camera.rotateOrbit(input.getMouseDeltaX() * 0.5f);

			break;
		case CameraMode::AERIAL:
			camera.updateAerialCamera(input, deltaTime);
			break;
		case CameraMode::INTEREST_POINT:
			camera.updateInterestPointCamera(deltaTime);
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

		if (input.isKeyDown(GLFW_KEY_W))
		{
			glm::vec3 movement = moveDirection * rubyMoveSpeed * deltaTime;
			rubyObj->transform.translate(movement.x, 0.0f, movement.z);
		}

		if (input.isKeyDown(GLFW_KEY_S))
		{
			glm::vec3 movement = moveDirection * rubyMoveSpeed * deltaTime;
			rubyObj->transform.translate(-movement.x, 0.0f, -movement.z);
		}

		if (input.isKeyDown(GLFW_KEY_A))
		{
			glm::vec3 movement = rightDirection * rubyMoveSpeed * deltaTime;
			rubyObj->transform.translate(-movement.x, 0.0f, -movement.z);
		}

		if (input.isKeyDown(GLFW_KEY_D))
		{
			glm::vec3 movement = rightDirection * rubyMoveSpeed * deltaTime;
			rubyObj->transform.translate(movement.x, 0.0f, movement.z);
		}

		// Hacer girar a Ruby hacia donde está mirando la cámara
		glm::vec3 rubyDirection = glm::normalize(glm::vec3(cameraDirection.x, 0.0f, cameraDirection.z));
		float angleY = atan2(rubyDirection.x, rubyDirection.z);
		rubyObj->transform.setRotation(0.0f, glm::degrees(angleY), 0.0f);

		// Tecla Z para mover la nave hacia adelante
		if (input.isKeyDown(GLFW_KEY_Z))
			naveObj->transform.translate(-naveSpeed * deltaTime, 0.0f, 0.0f);

		// Tecla X para mover la nave hacia atrás
		if (input.isKeyDown(GLFW_KEY_X))
			naveObj->transform.translate(naveSpeed * deltaTime, 0.0f, 0.0f);

		// Movimiento del tren
		train.Update(trainSpeed, deltaTime, wheelRotationSpeed);

		// Ajustar intensidad de luces según ciclo día-noche
		float timeProgress = skybox.getTimeProgress();
		float dayIntensity, nightIntensity;

		if (timeProgress < 0.5f)
		{
			// Transición: día -> noche
			float t = timeProgress * 2.0f; // 0 a 1
			dayIntensity = 1.0f - t;       // 1 -> 0
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
		glm::vec3 dayColor(1.0f, 0.95f, 0.8f);
		glm::vec3 nightColor(0.4f, 0.4f, 0.6f);
		glm::vec3 currentColor = glm::mix(nightColor, dayColor, dayIntensity);

		directionalLight.setColor(currentColor);
		directionalLight.setAmbientIntensity(0.3f * (0.5f + dayIntensity * 0.5f));
		directionalLight.setDiffuseIntensity(0.8f * dayIntensity);

		// Actualizar linterna con posición y dirección de la cámara
		spotLights[0].setFlash(camera.getPosition(), camera.getDirection());

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

		// Render del castillo y del tren (usando los getters de tus clases)
		castle.GetCastleObject()->draw(shader);
		train.GetTrainObject()->draw(shader);

		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	audioManager.shutdown();
	return 0;
}