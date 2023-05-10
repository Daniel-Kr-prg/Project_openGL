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

	if (this->pitch > 1.55)
	{
		this->pitch = 1.55;
	}
	else
	if (this->pitch < -1.55)
	{
		this->pitch = -1.55;
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