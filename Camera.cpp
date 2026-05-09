#include "Camera.h"
#include <algorithm>

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
	  moveSpeed(startMoveSpeed), turnSpeed(turnSpeed),
	  aerialPosition(startPosition)
{
	update();
}

void Camera::keyControl(const InputManager& input, GLfloat deltaTime)
{
	if (currentMode == CameraMode::THIRD_PERSON)
	{
		// En modo 3era persona no se controla con teclado
		return;
	}

	if (currentMode == CameraMode::AERIAL)
	{
		updateAerialCamera(input, deltaTime);
		return;
	}

	// Fallback a control de primera persona
	GLfloat velocity = moveSpeed * deltaTime;

	if (input.isKeyDown(GLFW_KEY_W)) position += front * velocity;
	if (input.isKeyDown(GLFW_KEY_S)) position -= front * velocity;
	if (input.isKeyDown(GLFW_KEY_A)) position -= right * velocity;
	if (input.isKeyDown(GLFW_KEY_D)) position += right * velocity;
}

void Camera::mouseControl(GLfloat xDelta, GLfloat yDelta)
{
	if (currentMode == CameraMode::THIRD_PERSON || currentMode == CameraMode::INTEREST_POINT)
	{
		// No se controla con ratón en estos modos
		return;
	}

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

void Camera::lookAt(const glm::vec3& target)
{
	front = glm::normalize(target - position);
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));

	// Calcular yaw y pitch basado en la dirección
	yaw = glm::degrees(atan2(front.z, front.x));
	pitch = glm::degrees(asin(front.y));
}

void Camera::setCameraMode(CameraMode mode)
{
	currentMode = mode;

	if (mode == CameraMode::AERIAL)
	{
		// Inicializar posición aérea
		aerialPosition = position;
		aerialPosition.y = aerialHeight;
	}
	else if (mode == CameraMode::INTEREST_POINT && !interestPoints.empty())
	{
		// Iniciar transición al primer punto de interés
		interestPointTransitionTime = 0.0f;
		currentInterestPoint = 0;
		interestPointStartPos = position;
		interestPointStartLookAt = position + front;
	}
}

void Camera::updateThirdPersonCamera(const glm::vec3& targetPosition, GLfloat deltaTime)
{
	if (currentMode != CameraMode::THIRD_PERSON) return;

	// Calcular posición en órbita alrededor del objetivo
	GLfloat radiusX = thirdPersonDistance * cos(glm::radians(orbitAngle));
	GLfloat radiusZ = thirdPersonDistance * sin(glm::radians(orbitAngle));

	glm::vec3 desiredPosition = targetPosition;
	desiredPosition.x += radiusX;
	desiredPosition.y += thirdPersonHeight;
	desiredPosition.z += radiusZ;

	// Interpolar suavemente hacia la posición deseada
	position = glm::mix(position, desiredPosition, smoothSpeed * deltaTime);

	// Mirar hacia el objetivo
	lookAt(targetPosition + glm::vec3(0.0f, 1.0f, 0.0f));
}

void Camera::updateAerialCamera(const InputManager& input, GLfloat deltaTime)
{
	if (currentMode != CameraMode::AERIAL) return;

	GLfloat velocity = aerialMoveSpeed * deltaTime;

	// Movimiento en el plano XZ (aéreo)
	if (input.isKeyDown(GLFW_KEY_W)) aerialPosition.z += velocity;
	if (input.isKeyDown(GLFW_KEY_S)) aerialPosition.z -= velocity;
	if (input.isKeyDown(GLFW_KEY_A)) aerialPosition.x -= velocity;
	if (input.isKeyDown(GLFW_KEY_D)) aerialPosition.x += velocity;

	// Ajustar altura con Q y E
	if (input.isKeyDown(GLFW_KEY_Q)) aerialPosition.y += velocity;
	if (input.isKeyDown(GLFW_KEY_E)) aerialPosition.y -= velocity;

	// Mantener altura mínima
	if (aerialPosition.y < 5.0f) aerialPosition.y = 5.0f;

	position = aerialPosition;

	// Mirar hacia abajo en ángulo
	lookAt(glm::vec3(aerialPosition.x, 0.0f, aerialPosition.z));
	pitch = -45.0f; // Ángulo hacia abajo
	update();
}

void Camera::addInterestPoint(const glm::vec3& point, const glm::vec3& lookAt)
{
	InterestPoint ip;
	ip.cameraPosition = point;
	ip.lookAtPoint = lookAt;
	interestPoints.push_back(ip);
}

void Camera::nextInterestPoint()
{
	if (interestPoints.empty()) return;

	interestPointTransitionTime = 0.0f;
	interestPointStartPos = position;
	interestPointStartLookAt = position + front;

	currentInterestPoint = (currentInterestPoint + 1) % interestPoints.size();
}

void Camera::updateInterestPointCamera(GLfloat deltaTime)
{
	if (currentMode != CameraMode::INTEREST_POINT || interestPoints.empty()) return;

	interestPointTransitionTime += deltaTime;
	GLfloat t = std::min(interestPointTransitionTime / interestPointTransitionDuration, 1.0f);

	const InterestPoint& current = interestPoints[currentInterestPoint];

	// Interpolación suave de posición
	position = glm::mix(interestPointStartPos, current.cameraPosition, t);

	// Interpolación suave de punto de vista
	glm::vec3 targetLookAt = glm::mix(interestPointStartLookAt, current.lookAtPoint, t);
	lookAt(targetLookAt);
}

Camera::~Camera()
{}