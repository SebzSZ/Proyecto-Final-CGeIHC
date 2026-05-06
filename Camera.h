#pragma once

#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "InputManager.h"

class Camera
{
public:
	Camera();
	Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat turnSpeed);

	void keyControl(const InputManager& input, GLfloat deltaTime);
	void mouseControl(GLfloat xDelta, GLfloat yDelta);

	glm::mat4 calculateViewMatrix() const;

	glm::vec3 getPosition() const { return position; }
	glm::vec3 getDirection() const { return glm::normalize(front); }

	~Camera();

private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	GLfloat yaw;
	GLfloat pitch;
	GLfloat moveSpeed;
	GLfloat turnSpeed;

	void update();
};