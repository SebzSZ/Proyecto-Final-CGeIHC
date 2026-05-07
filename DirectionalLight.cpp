#include "DirectionalLight.h"

DirectionalLight::DirectionalLight()
	: Light(), direction(0.0f, -1.0f, 0.0f)
{}

DirectionalLight::DirectionalLight(GLfloat red, GLfloat green, GLfloat blue,
								   GLfloat aIntensity, GLfloat dIntensity,
								   GLfloat xDir, GLfloat yDir, GLfloat zDir)
	: Light(red, green, blue, aIntensity, dIntensity), direction(xDir, yDir, zDir)
{}

void DirectionalLight::use(GLuint ambientIntensityLocation, GLuint colorLocation, GLuint diffuseIntensityLocation, GLuint directionLocation) const
{
	glUniform3f(colorLocation, color.r, color.g, color.b);
	glUniform1f(ambientIntensityLocation, ambientIntensity);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);
	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
}

// Setters para la dirección de la luz
void DirectionalLight::setDirection(GLfloat x, GLfloat y, GLfloat z)
{
	direction = glm::vec3(x, y, z);
}

void DirectionalLight::setDirection(const glm::vec3& dir)
{
	direction = dir;
}

// Setters para las características de la luz
void DirectionalLight::setAmbientIntensity(GLfloat intensity)
{
	ambientIntensity = intensity;
}

void DirectionalLight::setDiffuseIntensity(GLfloat intensity)
{
	diffuseIntensity = intensity;
}

void DirectionalLight::setColor(GLfloat r, GLfloat g, GLfloat b)
{
	color = glm::vec3(r, g, b);
}

DirectionalLight::~DirectionalLight()
{}