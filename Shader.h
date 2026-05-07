#pragma once

#include <string>
#include <fstream>
#include <sstream>

#include <glew.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>

#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

class Shader
{
public:
	Shader();

	void createFromString(const char* vertexCode, const char* fragmentCode);
	void createFromFiles(const char* vertexPath, const char* fragmentPath);

	void use() const;

	void clear();

	GLuint getModelLocation() const { return uniformModel; }
	GLuint getProjectionLocation() const { return uniformProjection; }
	GLuint getViewLocation() const { return uniformView; }

	GLuint getEyePositionLocation() const { return uniformEyePosition; }
	GLuint getColorLocation() const { return uniformColor; }
	GLuint getTextureOffsetLocation() const { return uniformTextureOffset; }

	GLuint getSpecularIntensityLocation() const { return uniformSpecularIntensity; }
	GLuint getShininessLocation() const { return uniformShininess; }

	void setDirectionalLight(DirectionalLight* light);
	void setPointLights(PointLight* lights, unsigned int lightCount);
	void setSpotLights(SpotLight* lights, unsigned int lightCount);

	~Shader();

private:
	GLuint shaderID;
	GLuint uniformModel, uniformProjection, uniformView;
	GLuint uniformEyePosition, uniformColor, uniformTextureOffset;
	GLuint uniformSpecularIntensity, uniformShininess;

	struct DirectionalLightUniforms
	{
		GLuint color;
		GLuint ambientIntensity;
		GLuint diffuseIntensity;
		GLuint direction;
	} uniformDirectionalLight;

	struct PointLightUniforms
	{
		GLuint color;
		GLuint ambientIntensity;
		GLuint diffuseIntensity;
		GLuint position;
		GLuint constant, linear, quadratic;
	};
	PointLightUniforms uniformPointLights[MAX_POINT_LIGHTS];
	GLuint uniformPointLightCount;

	struct SpotLightUniforms
	{
		GLuint color;
		GLuint ambientIntensity;
		GLuint diffuseIntensity;
		GLuint position;
		GLuint direction;
		GLuint constant, linear, quadratic;
		GLuint edge;
	};
	SpotLightUniforms uniformSpotLights[MAX_SPOT_LIGHTS];
	GLuint uniformSpotLightCount;

	std::string readFile(const char* path);
	void compile(const char* vertexCode, const char* fragmentCode);
	void addShader(GLuint program, const char* code, GLenum type);
	void fetchUniformLocations();
};