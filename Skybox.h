#pragma once

#include <string>
#include <vector>

#include <glew.h>
#include <glm.hpp>

#include "Shader.h"

class Skybox
{
public:
	Skybox();

	void create(const std::vector<std::string>& faces, const char* vertexShaderPath, const char* fragmentShaderPath);
	void draw(const glm::mat4& view, const glm::mat4& projection) const;

	~Skybox();

private:
	GLuint cubemapID;
	GLuint VAO, VBO, EBO;

	Shader shader;

	GLuint uniformView, uniformProjection;

	void setUpMesh();

	void loadCubemap(const std::vector<std::string>& faces);
};