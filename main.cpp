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
	audioManager.play("bgMusic", true, 1.0f);

    // Configurar cámara
    Camera camera(glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.1f);

    // Creación de shader
    Shader shader;
    shader.createFromFiles(VERT_SHADER, FRAG_SHADER);

	// Creación del skybox
    Skybox skybox;
    skybox.create(
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

    // Crear piso con Mesh
    Mesh floorMesh;
    floorMesh.create(FLOOR_VERTS, FLOOR_IDX, 32, 6);

    Texture floorTexture("Textures/piso.tga");
    floorTexture.loadWithAlpha();

    Material matOpaco(0.2f, 4.0f);

    // Nave
    Model nave;
    if (!nave.load("Models/nave.obj")) return -1;

    Material matBrillante(1.0f, 32.0f);

    // Estado de la nave
    glm::vec3 navePos(0.0f, 2.0f, -5.0f);
    float     naveSpeed = 8.0f;

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

        // Input
        glfwPollEvents();
        InputManager& input = InputManager::getInstance();
        input.beginFrame();

        // Movimiento de cámara
        camera.keyControl(input, deltaTime);
        camera.mouseControl(input.getMouseDeltaX(), input.getMouseDeltaY());

		// Tecla Z para mover la nave hacia adelante
        if (input.isKeyDown(GLFW_KEY_Z))
            navePos.x -= naveSpeed * deltaTime;

		// Tecla X para mover la nave hacia atrás
        if (input.isKeyDown(GLFW_KEY_X))
			navePos.x += naveSpeed * deltaTime;

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

        // Piso
        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(50.0f, 1.0f, 50.0f));
        glUniformMatrix4fv(shader.getModelLocation(), 1, GL_FALSE, glm::value_ptr(model));

        matOpaco.use(shader.getSpecularIntensityLocation(), shader.getShininessLocation());
        floorTexture.bind();
        floorMesh.render();
        floorTexture.unbind();

        // Nave
        model = glm::mat4(1.0f);
        model = glm::translate(model, navePos);
        model = glm::scale(model, glm::vec3(2.5f));
        glUniformMatrix4fv(shader.getModelLocation(), 1, GL_FALSE, glm::value_ptr(model));

        matBrillante.use(shader.getSpecularIntensityLocation(), shader.getShininessLocation());
        nave.render();

        glUseProgram(0);
        mainWindow.swapBuffers();
    }
    audioManager.shutdown();
    return 0;
}