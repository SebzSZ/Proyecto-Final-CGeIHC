#include <cstdio>
#include <cstring>

#include "Shader.h"

Shader::Shader()
	: shaderID(0), uniformModel(0), uniformProjection(0), uniformView(0),
	  uniformEyePosition(0), uniformColor(0), uniformTextureOffset(0),
	  uniformSpecularIntensity(0), uniformShininess(0),
	  uniformPointLightCount(0), uniformSpotLightCount(0)
{}

// Creación del shader
void Shader::createFromString(const char* vertexCode, const char* fragmentCode)
{
	compile(vertexCode, fragmentCode);
}

void Shader::createFromFiles(const char* vertexPath, const char* fragmentPath)
{
	std::string vertexSrc = readFile(vertexPath);
	std::string fragmentSrc = readFile(fragmentPath);
	createFromString(vertexSrc.c_str(), fragmentSrc.c_str());
}

// Activación del shader
void Shader::use() const
{
	glUseProgram(shaderID);
}

void Shader::clear()
{
	if (shaderID != 0)
	{
		glDeleteProgram(shaderID);
		shaderID = 0;
	}
	uniformModel = uniformProjection = uniformView = 0;
	uniformColor = uniformTextureOffset = 0;
}

// Envio de luces
void Shader::setDirectionalLight(DirectionalLight* light)
{
	light->use(uniformDirectionalLight.ambientIntensity,
			   uniformDirectionalLight.color,
			   uniformDirectionalLight.diffuseIntensity,
			   uniformDirectionalLight.direction);
}

void Shader::setPointLights(PointLight* lights, unsigned int lightCount)
{
	if (lightCount > MAX_POINT_LIGHTS) lightCount = MAX_POINT_LIGHTS;
	glUniform1i(uniformPointLightCount, lightCount);

	for (unsigned int i = 0; i < lightCount; ++i)
	{
		lights[i].use(uniformPointLights[i].ambientIntensity,
					  uniformPointLights[i].color,
					  uniformPointLights[i].diffuseIntensity,
					  uniformPointLights[i].position,
					  uniformPointLights[i].constant,
					  uniformPointLights[i].linear,
					  uniformPointLights[i].quadratic);
	}
}

void Shader::setSpotLights(SpotLight* lights, unsigned int lightCount)
{
	if (lightCount > MAX_SPOT_LIGHTS) lightCount = MAX_SPOT_LIGHTS;
	glUniform1i(uniformSpotLightCount, lightCount);

	for (unsigned int i = 0; i < lightCount; ++i)
	{
		lights[i].use(uniformSpotLights[i].ambientIntensity,
					  uniformSpotLights[i].color,
					  uniformSpotLights[i].diffuseIntensity,
					  uniformSpotLights[i].position,
					  uniformSpotLights[i].direction,
					  uniformSpotLights[i].constant,
					  uniformSpotLights[i].linear,
					  uniformSpotLights[i].quadratic,
					  uniformSpotLights[i].edge);
	}
}

// Helpers
std::string Shader::readFile(const char* path)
{
	std::string content;
	std::ifstream file(path, std::ios::in);

	if (!file.is_open())
	{
		printf("[Shader] Error: No se pudo abrir: %s\n", path);
		return "";
	}

	std::string line;
	while (std::getline(file, line))
	{
		content += line + "\n";
	}

	return content;
}

void Shader::compile(const char* vertexCode, const char* fragmentCode)
{
	shaderID = glCreateProgram();
	if (!shaderID)
	{
		printf("[Shader] Error: No se pudo crear el shader.\n");
		return;
	}

	addShader(shaderID, vertexCode, GL_VERTEX_SHADER);
	addShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar log[1024] = { 0 };

	glLinkProgram(shaderID);
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(log), nullptr, log);
		printf("[Shader] Error: No se pudo enlazar el shader: %s\n", log);
		return;
	}

	glValidateProgram(shaderID);
	glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(log), nullptr, log);
		printf("[Shader] Error: No se pudo validar el shader: %s\n", log);
		return;
	}

	fetchUniformLocations();
}

void Shader::addShader(GLuint program, const char* code, GLenum type)
{
	GLuint shader = glCreateShader(type);

	const GLchar* src[1] = { code };
	GLint len[1] = { (GLint)strlen(code) };

	glShaderSource(shader, 1, src, len);
	glCompileShader(shader);

	GLint result = 0;
	GLchar log[1024] = { 0 };

	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
		printf("[Shader] Error: No se pudo compilar el shader (%u): %s\n", type, log);
		return;
	}

	glAttachShader(program, shader);
}

void Shader::fetchUniformLocations()
{
	uniformProjection = glGetUniformLocation(shaderID, "projection");
	uniformModel = glGetUniformLocation(shaderID, "model");
	uniformView = glGetUniformLocation(shaderID, "view");
	uniformEyePosition = glGetUniformLocation(shaderID, "eyePosition");
	uniformColor = glGetUniformLocation(shaderID, "color");
	uniformTextureOffset = glGetUniformLocation(shaderID, "textureOffset");
	uniformSpecularIntensity = glGetUniformLocation(shaderID, "material.specularIntensity");
	uniformShininess = glGetUniformLocation(shaderID, "material.shininess");

	// Luz direccional
	uniformDirectionalLight.color = glGetUniformLocation(shaderID, "directionalLight.base.color");
	uniformDirectionalLight.ambientIntensity = glGetUniformLocation(shaderID, "directionalLight.base.ambientIntensity");
	uniformDirectionalLight.diffuseIntensity = glGetUniformLocation(shaderID, "directionalLight.base.diffuseIntensity");
	uniformDirectionalLight.direction = glGetUniformLocation(shaderID, "directionalLight.direction");

	// Point lights
	uniformPointLightCount = glGetUniformLocation(shaderID, "pointLightCount");
	for (int i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		char buffer[100];
		snprintf(buffer, sizeof(buffer), "pointLights[%d].base.color", i);
		uniformPointLights[i].color = glGetUniformLocation(shaderID, buffer);

		snprintf(buffer, sizeof(buffer), "pointLights[%d].base.ambientIntensity", i);
		uniformPointLights[i].ambientIntensity = glGetUniformLocation(shaderID, buffer);

		snprintf(buffer, sizeof(buffer), "pointLights[%d].base.diffuseIntensity", i);
		uniformPointLights[i].diffuseIntensity = glGetUniformLocation(shaderID, buffer);

		snprintf(buffer, sizeof(buffer), "pointLights[%d].position", i);
		uniformPointLights[i].position = glGetUniformLocation(shaderID, buffer);

		snprintf(buffer, sizeof(buffer), "pointLights[%d].constant", i);
		uniformPointLights[i].constant = glGetUniformLocation(shaderID, buffer);

		snprintf(buffer, sizeof(buffer), "pointLights[%d].linear", i);
		uniformPointLights[i].linear = glGetUniformLocation(shaderID, buffer);

		snprintf(buffer, sizeof(buffer), "pointLights[%d].quadratic", i);
		uniformPointLights[i].quadratic = glGetUniformLocation(shaderID, buffer);
	}

	// Spot lights
	uniformSpotLightCount = glGetUniformLocation(shaderID, "spotLightCount");
	for (int i = 0; i < MAX_SPOT_LIGHTS; i++)
	{
		char buffer[100];
		snprintf(buffer, sizeof(buffer), "spotLights[%d].base.base.color", i);
		uniformSpotLights[i].color = glGetUniformLocation(shaderID, buffer);

		snprintf(buffer, sizeof(buffer), "spotLights[%d].base.base.ambientIntensity", i);
		uniformSpotLights[i].ambientIntensity = glGetUniformLocation(shaderID, buffer);

		snprintf(buffer, sizeof(buffer), "spotLights[%d].base.base.diffuseIntensity", i);
		uniformSpotLights[i].diffuseIntensity = glGetUniformLocation(shaderID, buffer);

		snprintf(buffer, sizeof(buffer), "spotLights[%d].base.position", i);
		uniformSpotLights[i].position = glGetUniformLocation(shaderID, buffer);

		snprintf(buffer, sizeof(buffer), "spotLights[%d].base.constant", i);
		uniformSpotLights[i].constant = glGetUniformLocation(shaderID, buffer);

		snprintf(buffer, sizeof(buffer), "spotLights[%d].base.linear", i);
		uniformSpotLights[i].linear = glGetUniformLocation(shaderID, buffer);

		snprintf(buffer, sizeof(buffer), "spotLights[%d].base.quadratic", i);
		uniformSpotLights[i].quadratic = glGetUniformLocation(shaderID, buffer);

		snprintf(buffer, sizeof(buffer), "spotLights[%d].direction", i);
		uniformSpotLights[i].direction = glGetUniformLocation(shaderID, buffer);

		snprintf(buffer, sizeof(buffer), "spotLights[%d].edge", i);
		uniformSpotLights[i].edge = glGetUniformLocation(shaderID, buffer);
	}
}

Shader::~Shader()
{
	clear();
}