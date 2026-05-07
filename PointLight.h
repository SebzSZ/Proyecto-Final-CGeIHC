#pragma once

#include <glm.hpp>
#include "Light.h"

class PointLight : public Light
{
public:
	PointLight();
	PointLight(GLfloat red, GLfloat green, GLfloat blue,
			   GLfloat aIntensity, GLfloat dIntensity,
			   GLfloat xPos, GLfloat yPos, GLfloat zPos,
			   GLfloat constant, GLfloat linear, GLfloat quadratic);

	void use(GLuint ambientIntensityLocation, GLuint colorLocation, GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint constantLocation, GLuint linearLocation, GLuint quadraticLocation) const;

	void setPosition(GLfloat x, GLfloat y, GLfloat z);
	void setPosition(const glm::vec3& position);

	virtual ~PointLight();

protected:
	glm::vec3 position;
	GLfloat constant;
	GLfloat linear;
	GLfloat quadratic;
};