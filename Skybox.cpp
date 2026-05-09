#include "stb_image.h"

#include <cstdio>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Skybox.h"

// Vertices del cubo unitario
static const GLfloat SKYBOX_VERTICES[] = {
	-1.0f,  1.0f, -1.0f,   -1.0f, -1.0f, -1.0f,    1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,    1.0f,  1.0f, -1.0f,   -1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,   -1.0f, -1.0f, -1.0f,   -1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,   -1.0f,  1.0f,  1.0f,   -1.0f, -1.0f,  1.0f,

	 1.0f, -1.0f, -1.0f,    1.0f, -1.0f,  1.0f,    1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,    1.0f,  1.0f, -1.0f,    1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,   -1.0f,  1.0f,  1.0f,    1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,    1.0f, -1.0f,  1.0f,   -1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,    1.0f,  1.0f, -1.0f,    1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,   -1.0f,  1.0f,  1.0f,   -1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,   -1.0f, -1.0f,  1.0f,    1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,   -1.0f, -1.0f,  1.0f,    1.0f, -1.0f,  1.0f
};

Skybox::Skybox()
	: dayCubemapID(0), nightCubemapID(0), VAO(0), VBO(0), EBO(0), uniformView(0), uniformProjection(0)
{}

void Skybox::create(const std::vector<std::string>& dayFaces, const std::vector<std::string>& nightFaces,
					  const char* vertexShaderPath, const char* fragmentShaderPath)
{
	shader.createFromFiles(vertexShaderPath, fragmentShaderPath);
	uniformProjection = shader.getProjectionLocation();
	uniformView = shader.getViewLocation();

	loadCubemap(dayFaces, dayCubemapID);
	loadCubemap(nightFaces, nightCubemapID);
	setUpMesh();
}

void Skybox::setUpMesh()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(SKYBOX_VERTICES), SKYBOX_VERTICES, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

void Skybox::loadCubemap(const std::vector<std::string>& faces, GLuint& cubemapID)
{
	glGenTextures(1, &cubemapID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapID);

	stbi_set_flip_vertically_on_load(false);

	int width, height, channels;
	for (GLuint i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &channels, 0);

		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			printf("[Skybox] Error: No se pudo cargar la textura %u: '%s' - %s\n", i, faces[i].c_str(), stbi_failure_reason());
			stbi_image_free(data);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Skybox::draw(const glm::mat4& view, const glm::mat4& projection) const
{
	drawWithBlending(view, projection);
}

void Skybox::drawWithBlending(const glm::mat4& view, const glm::mat4& projection) const
{
	glm::mat4 viewNoTranslation = glm::mat4(glm::mat3(view));

	GLint prevDepthFunc;
	glGetIntegerv(GL_DEPTH_FUNC, &prevDepthFunc);
	glDepthFunc(GL_LEQUAL);

	shader.use();
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewNoTranslation));

	glBindVertexArray(VAO);

	// Determinar qué skybox mostrar basado en el progreso del día-noche
	GLuint activeCubemap;
	float alpha;

	if (timeProgress < 0.5f)
	{
		// Primera mitad: día a noche
		activeCubemap = dayCubemapID;
		alpha = 1.0f - (timeProgress * 2.0f); // 1.0 -> 0.0
	}
	else
	{
		// Segunda mitad: noche a día
		activeCubemap = nightCubemapID;
		alpha = (timeProgress - 0.5f) * 2.0f; // 0.0 -> 1.0
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, activeCubemap);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glBindVertexArray(0);

	glDepthFunc(prevDepthFunc);
	glUseProgram(0);
}

void Skybox::updateDayNightCycle(float deltaTime)
{
	timeProgress += deltaTime / dayNightCycleDuration;

	// Hacer que el ciclo sea infinito
	if (timeProgress >= 1.0f)
	{
		timeProgress = 0.0f;
	}

	// Determinar si es día o noche
	if (timeProgress < 0.5f)
	{
		currentTimeOfDay = TimeOfDay::DAY;
	}
	else
	{
		currentTimeOfDay = TimeOfDay::NIGHT;
	}
}

Skybox::~Skybox()
{
	if (dayCubemapID) glDeleteTextures(1, &dayCubemapID);
	if (nightCubemapID) glDeleteTextures(1, &nightCubemapID);
	if (VBO) glDeleteBuffers(1, &VBO);
	if (VAO) glDeleteVertexArrays(1, &VAO);
}