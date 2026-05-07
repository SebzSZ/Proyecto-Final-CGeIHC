#include "PointLight.h"

PointLight::PointLight()
	: Light(), position(0.0f, 0.0f, 0.0f), constant(1.0f), linear(0.0f), quadratic(0.0f)
{}

PointLight::PointLight(GLfloat red, GLfloat green, GLfloat blue,
					   GLfloat aIntensity, GLfloat dIntensity,
					   GLfloat xPos, GLfloat yPos, GLfloat zPos,
					   GLfloat constant, GLfloat linear, GLfloat quadratic)
	: Light(red, green, blue, aIntensity, dIntensity), position(xPos, yPos, zPos), constant(constant), linear(linear), quadratic(quadratic)
{}

void PointLight::use(GLuint ambientIntensityLocation, GLuint colorLocation, GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint constantLocation, GLuint linearLocation, GLuint quadraticLocation) const
{
	glUniform3f(colorLocation, color.r, color.g, color.b);
	glUniform1f(ambientIntensityLocation, ambientIntensity);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);
	glUniform3f(positionLocation, position.x, position.y, position.z);
	glUniform1f(constantLocation, constant);
	glUniform1f(linearLocation, linear);
	glUniform1f(quadraticLocation, quadratic);
}

// Setters para la posición de la luz
void PointLight::setPosition(GLfloat x, GLfloat y, GLfloat z)
{
	position = glm::vec3(x, y, z);
}

void PointLight::setPosition(const glm::vec3& pos)
{
	position = pos;
}

PointLight::~PointLight()
{}