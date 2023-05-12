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

Camera::Camera() {

}

/**
 * \brief Set camera parameters.
 * \param fov Field of view
 * \param aspect Screen aspect ratio
 * \param zNear Near clipping plane distance
 * \param zFar Far clipping plane distance
 */
void Camera::setCameraView(glm::vec3 position, float yaw, float pitch)
{
	
	setYawPitch(yaw, pitch);
	setPosition(position);
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
	setCameraView(glm::vec3(3.98f, 0.637f, 4.60f), -5.54f, -0.14f);
}
/**
 * \brief TODO: add parameters for cameraView
 */
void Camera::setStaticView2()
{
	setCameraState(STATIC_CAMERA);
	setCameraView(glm::vec3(5.516f, 4.74f, -2.87f), -4.31f, -0.71f);
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

void Camera::handleCameraMovement(float elapsedTime, std::unordered_map<char, bool>& keyPressedState, std::unordered_map<int, bool>& keyPressedSpecialState)
{
	if (getCameraState() == STATIC_CAMERA)
		return;

	if (getCameraState() == DYNAMIC_CAMERA)
	{
		glm::vec3 movementVector(0.0f);

		if (keyPressedState['w'])
		{
			movementVector += getForward();
		}
		else
			if (keyPressedState['s'])
			{
				movementVector -= getForward();
			}

		if (keyPressedState['a'])
		{
			movementVector -= getRight();
		}
		else
			if (keyPressedState['d'])
			{
				movementVector += getRight();
			}

		movementVector *= elapsedTime * getSpeed();
		setPosition(getPosition() + movementVector);

	}


	float rotationX = 0;
	float rotationY = 0;

	if (keyPressedSpecialState[GLUT_KEY_UP])
	{
		rotationX += 1 * elapsedTime * getKeySensitivity();
	}
	else
		if (keyPressedSpecialState[GLUT_KEY_DOWN])
		{
			rotationX -= 1 * elapsedTime * getKeySensitivity();
		}

	if (keyPressedSpecialState[GLUT_KEY_LEFT])
	{
		rotationY += 1 * elapsedTime * getKeySensitivity();
	}
	else
		if (keyPressedSpecialState[GLUT_KEY_RIGHT])
		{
			rotationY -= 1 * elapsedTime * getKeySensitivity();
		}

	addYawPitch(rotationY, rotationX);
}

void Camera::handlePassiveMouseMotion(int mouseX, int mouseY, Config* config)
{
	int halfWidth = config->getWindowWidth() / 2;
	int halfHeight = config->getWindowHeight() / 2;

	if (cameraState == STATIC_CAMERA)
	{
		glutWarpPointer(halfWidth, halfHeight);
		previousMouseX = halfWidth;
		previousMouseY = halfHeight;
		return;
	}

	int deltaX = previousMouseX - mouseX;
	int deltaY = previousMouseY - mouseY;

	addYawPitch(deltaX * getMouseSensitivity(), deltaY * getMouseSensitivity());

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
void Camera::setYawPitch(float yaw, float pitch) {
	this->yaw = yaw;
	this->pitch = pitch;

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
float Camera::getYaw()
{
	return this->yaw;
}
float Camera::getPitch()
{
	return this->pitch;
}

void Camera::deserialize(nlohmann::json data)
{
	ObjectInstance::deserialize(data);

	if (data.contains("camera"))
	{
		nlohmann::json cameraData = data["camera"];
		if (cameraData.contains("fov") && cameraData.contains("zNear") && cameraData.contains("zFar"))
			projectionMatrix = glm::perspective(
				cameraData["fov"].get<float>(),
				render.getCurrentAspect(),
				cameraData["zNear"].get<float>(),
				cameraData["zFar"].get<float>());
		if (cameraData.contains("speed"))
			speed = cameraData["speed"].get<float>();
		if (cameraData.contains("keySensitivity"))
			keySensitivity = cameraData["keySensitivity"].get<float>();
		if (cameraData.contains("mouseSensitivity"))
			mouseSensitivity = cameraData["mouseSensitivity"].get<float>();
	}
}

/**
 * \brief Destroys the camera object
 */
Camera::~Camera() {

}