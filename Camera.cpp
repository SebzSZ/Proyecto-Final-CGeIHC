#include "Camera.h"

Camera::Camera()
	: position(0.0f), front(0.0f, 0.0f, -1.0f), up(0.0f),
	  right(0.0f), worldUp(0.0f, 1.0f, 0.0f),
	  yaw(-90.0f), pitch(0.0f), moveSpeed(0.0f), turnSpeed(0.0f)
{}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp,
			   GLfloat startYaw, GLfloat startPitch,
			   GLfloat startMoveSpeed, GLfloat turnSpeed)
	: position(startPosition), worldUp(startUp),
	  yaw(startYaw), pitch(startPitch), 
	  front(0.0f, 0.0f, -1.0f),
	  moveSpeed(startMoveSpeed), turnSpeed(turnSpeed)
{
	update();
}

void Camera::keyControl(const InputManager& input, GLfloat deltaTime)
{
	GLfloat velocity = moveSpeed * deltaTime;

	if (input.isKeyDown(GLFW_KEY_W)) position += front * velocity;
	if (input.isKeyDown(GLFW_KEY_S)) position -= front * velocity;
	if (input.isKeyDown(GLFW_KEY_A)) position -= right * velocity;
	if (input.isKeyDown(GLFW_KEY_D))	position += right * velocity;
}

void Camera::mouseControl(GLfloat xDelta, GLfloat yDelta)
{
	yaw += xDelta;
	pitch -= yDelta;

	if (pitch > 89.0f) pitch = 89.0f;
	if (pitch < -89.0f) pitch = -89.0f;

	update();
}

glm::mat4 Camera::calculateViewMatrix() const
{
	return glm::lookAt(position, position + front, up);
}

void Camera::update()
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

Camera::~Camera()
{}