#include "SpotLight.h"

SpotLight::SpotLight()
	: PointLight(), direction(0.0f, -1.0f, 0.0f), edge(0.0f)
{
	procEdge = cos(glm::radians(edge));
}

SpotLight::SpotLight(GLfloat red, GLfloat green, GLfloat blue,
					 GLfloat aIntensity, GLfloat dIntensity,
					 GLfloat xPos, GLfloat yPos, GLfloat zPos,
					 GLfloat xDir, GLfloat yDir, GLfloat zDir,
					 GLfloat constant, GLfloat linear, GLfloat quadratic,
					 GLfloat edge)
	: PointLight(red, green, blue, aIntensity, dIntensity, xPos, yPos, zPos, constant, linear, quadratic), direction(xDir, yDir, zDir), edge(edge)
{
	procEdge = cos(glm::radians(edge));
}

void SpotLight::use(GLuint ambientIntensityLocation, GLuint colorLocation, GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint directionLocation, GLuint constantLocation, GLuint linearLocation, GLuint quadraticLocation, GLuint edgeLocation) const
{
	glUniform3f(colorLocation, color.r, color.g, color.b);
	glUniform1f(ambientIntensityLocation, ambientIntensity);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);
	glUniform3f(positionLocation, position.x, position.y, position.z);
	glUniform1f(constantLocation, constant);
	glUniform1f(linearLocation, linear);
	glUniform1f(quadraticLocation, quadratic);
	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(edgeLocation, procEdge);
}

// Setters para la dirección y posición del foco
void SpotLight::setFlash(const glm::vec3& pos, const glm::vec3& dir)
{
	position = pos;
	direction = dir;
}

void SpotLight::setPosition(GLfloat x, GLfloat y, GLfloat z)
{
	position = glm::vec3(x, y, z);
}

void SpotLight::setPosition(const glm::vec3& pos)
{
	position = pos;
}

void SpotLight::setDirection(const glm::vec3& dir)
{
	direction = dir;
}

SpotLight::~SpotLight()
{}