#pragma once

#include <string>
#include <vector>

#include <glew.h>
#include <glm.hpp>

#include "Shader.h"

enum class TimeOfDay
{
	DAY,
	NIGHT
};

class Skybox
{
public:
	Skybox();

	void create(const std::vector<std::string>& dayFaces, const std::vector<std::string>& nightFaces,
				const char* vertexShaderPath, const char* fragmentShaderPath);
	void draw(const glm::mat4& view, const glm::mat4& projection) const;

	// Control del ciclo día-noche
	void updateDayNightCycle(float deltaTime);
	void setDayNightCycleDuration(float duration) { dayNightCycleDuration = duration; }
	TimeOfDay getCurrentTimeOfDay() const { return currentTimeOfDay; }
	float getTimeProgress() const { return timeProgress; }

	~Skybox();

private:
	GLuint dayCubemapID;
	GLuint nightCubemapID;
	GLuint VAO, VBO, EBO;

	Shader shader;

	GLuint uniformView, uniformProjection;

	// Control del ciclo día-noche
	float timeProgress = 0.0f;					// 0 = día, 1 = noche
	float dayNightCycleDuration = 30.0f;		// Duración total del ciclo en segundos
	TimeOfDay currentTimeOfDay = TimeOfDay::DAY;

	void setUpMesh();

	void loadCubemap(const std::vector<std::string>& faces, GLuint& cubemapID);
	void drawWithBlending(const glm::mat4& view, const glm::mat4& projection) const;
};