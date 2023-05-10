#include "camera.h"
/**
 * \brief Creates the camera object.
 * \param fov Field of view
 * \param aspect Screen aspect ratio
 * \param zNear Near clipping plane distance
 * \param zFar Far clipping plane distance
 */
Camera::Camera(float fov, float aspect, float zNear, float zFar, float speed, float keySensitivity, float mouseSensitivity) {
	projectionMatrix = glm::perspective(fov, aspect, zNear, zFar);
	this->speed = speed;
	this->keySensitivity = keySensitivity;
	this->mouseSensitivity = mouseSensitivity;
}


void handleCameraMovement(Camera& camera, float elapsedTime, std::unordered_map<char, bool>& keyPressedState, std::unordered_map<int, bool>& keyPressedSpecialState)
{
	glm::vec3 movementVector(0.0f);

	if (keyPressedState['w'])
	{
		movementVector += camera.getForward();
	}
	else
		if (keyPressedState['s'])
		{
			movementVector -= camera.getForward();
		}

	if (keyPressedState['a'])
	{
		movementVector -= camera.getRight();
	}
	else
		if (keyPressedState['d'])
		{
			movementVector += camera.getRight();
		}

	movementVector *= elapsedTime * camera.getSpeed();
	camera.setPosition(camera.getPosition() + movementVector);

	float rotationX = 0;
	float rotationY = 0;

	if (keyPressedSpecialState[GLUT_KEY_UP])
	{
		rotationX += 1 * elapsedTime * camera.getKeySensitivity();
	}
	else
		if (keyPressedSpecialState[GLUT_KEY_DOWN])
		{
			rotationX -= 1 * elapsedTime * camera.getKeySensitivity();
		}

	if (keyPressedSpecialState[GLUT_KEY_LEFT])
	{
		rotationY += 1 * elapsedTime * camera.getKeySensitivity();
	}
	else
		if (keyPressedSpecialState[GLUT_KEY_RIGHT])
		{
			rotationY -= 1 * elapsedTime * camera.getKeySensitivity();
		}

	camera.addYawPitch(rotationY, rotationX);
}


/**
 * \brief Returns the current projection matrix
 */
glm::mat4 Camera::getProjection() {
	return projectionMatrix;
}

/**
 * \brief Returns the camera speed
 */
float Camera::getSpeed() {
	return speed;
}

/**
 * \brief Returns the camera rotation sensitivity (rotation by keys)
 */
float Camera::getKeySensitivity()
{
	return keySensitivity;
}

/**
 * \brief Returns the camera rotation sensitivity (rotation by mouse)
 */
float Camera::getMouseSensitivity()
{
	return mouseSensitivity;
}

/**
 * \brief Returns the current view matrix
 */
glm::mat4 Camera::getView() {
	return glm::inverse(localModelMatrix);
}

/**
 * \brief Add camera rotation from input
 */
void Camera::addYawPitch(float yaw, float pitch) {
	this->yaw += yaw;
	this->pitch += pitch;

	if (this->pitch > 1.55f)
	{
		this->pitch = 1.55f;
	}
	else
	if (this->pitch < -1.55f)
	{
		this->pitch = -1.55f;
	}

	setRotation(glm::quat());
	rotateRadY(this->yaw);
	rotateRadX(this->pitch);
}

/**
 * \brief Destroys the camera object
 */
Camera::~Camera() {

}