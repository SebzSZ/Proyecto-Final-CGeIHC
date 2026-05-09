#pragma once

#include "Light.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight();
	DirectionalLight(GLfloat red, GLfloat green, GLfloat blue,
					 GLfloat aIntensity, GLfloat dIntensity,
					 GLfloat xDir, GLfloat yDir, GLfloat zDir);

	void use(GLuint ambientIntensityLocation, GLuint colorLocation, GLuint diffuseIntensityLocation, GLuint directionLocation) const;

	void setDirection(GLfloat x, GLfloat y, GLfloat z);
	void setDirection(const glm::vec3& direction);

	void setAmbientIntensity(GLfloat intensity);
	void setDiffuseIntensity(GLfloat intensity);
	void setColor(GLfloat r, GLfloat g, GLfloat b);
	void setColor(const glm::vec3& newColor);

	glm::vec3 getColor() const { return color; }
	GLfloat getAmbientIntensity() const { return ambientIntensity; }
	GLfloat getDiffuseIntensity() const { return diffuseIntensity; }

	~DirectionalLight();

private:
	glm::vec3 direction;
};