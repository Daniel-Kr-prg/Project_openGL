#include "camera.h"
#include "object.h"
#include "config.h"
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
 * \brief Set camera parameters.
 * \param fov Field of view
 * \param aspect Screen aspect ratio
 * \param zNear Near clipping plane distance
 * \param zFar Far clipping plane distance
 */
void Camera::setCameraView(float fov, float aspect, float zNear, float zFar)
{
	projectionMatrix = glm::perspective(fov, aspect, zNear, zFar);
}

/**
 * \brief Allows camera to move
 */
void Camera::setDynamicCamera()
{
	setCameraState(DYNAMIC_CAMERA);
}

/**
 * \brief TODO: add parameters for cameraView
 */
void Camera::setStaticView1()
{
	setCameraState(STATIC_CAMERA);
	setCameraView(0.0f, 0.0f, 0.0f, 0.0f);
}
/**
 * \brief TODO: add parameters for cameraView
 */
void Camera::setStaticView2()
{
	setCameraState(STATIC_CAMERA);
	setCameraView(0.0f, 0.0f, 0.0f, 0.0f);
}
void Camera::setCameraOnObject(MovingObject* objectToFollow, Camera* camInstance)
{
	camInstance->setCameraState(CAMERA_ON_OBJECT);
	objectToFollow->setCameraOnObject(camInstance);
}

void Camera::setCameraState(camState newState)
{
	cameraState = newState;
}

camState Camera::getCameraState()
{
	return cameraState;
}

void handleCameraMovement(Camera& camera, float elapsedTime, std::unordered_map<char, bool>& keyPressedState, std::unordered_map<int, bool>& keyPressedSpecialState)
{
	if (camera.getCameraState() == STATIC_CAMERA)
		return;

	if (camera.getCameraState() == DYNAMIC_CAMERA)
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

	}


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

void Camera::handlePassiveMouseMotion(int mouseX, int mouseY, Config* config)
{
	if (cameraState == STATIC_CAMERA)
		return;

	int deltaX = previousMouseX - mouseX;
	int deltaY = previousMouseY - mouseY;

	addYawPitch(deltaX * getMouseSensitivity(), deltaY * getMouseSensitivity());

	int halfWidth = config->getWindowWidth() / 2;
	int halfHeight = config->getWindowHeight() / 2;

	glutWarpPointer(halfWidth, halfHeight);

	previousMouseX = halfWidth;
	previousMouseY = halfHeight;
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