#pragma once

#include "PointLight.h"

class SpotLight : public PointLight
{
public:
	SpotLight();
	SpotLight(GLfloat red, GLfloat green, GLfloat blue,
			  GLfloat aIntensity, GLfloat dIntensity,
			  GLfloat xPos, GLfloat yPos, GLfloat zPos,
			  GLfloat xDir, GLfloat yDir, GLfloat zDir,
			  GLfloat constant, GLfloat linear, GLfloat quadratic,
			  GLfloat edge);

	void use(GLuint ambientIntensityLocation, GLuint colorLocation, GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint directionLocation, GLuint constantLocation, GLuint linearLocation, GLuint quadraticLocation, GLuint edgeLocation) const;

	void setFlash(const glm::vec3& pos, const glm::vec3& dir);
	void setPosition(GLfloat x, GLfloat y, GLfloat z);
	void setPosition(const glm::vec3& pos);
	void setDirection(const glm::vec3& dir);

	~SpotLight();

private:
	glm::vec3 direction;
	GLfloat edge;
	GLfloat procEdge;
};