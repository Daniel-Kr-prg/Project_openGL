#include "player.h"

Player::Player() : SingleMesh() 
{
};

Player::~Player()
{
};

void Player::initialize()
{
	camera = Render::getRender()->getRootNode()->firstNodeByType<Camera>();
}

void Player::onSpecialKeyPress(int key)
{
	switch (key)
	{
	case GLUT_KEY_F1:
		freeCamera();
		camera->setStaticView1();
		break;
	case GLUT_KEY_F2:
		freeCamera();
		camera->setStaticView2();
		break;
	case GLUT_KEY_F3:
		setCameraOnObject();
		break;
	case GLUT_KEY_F4:
		freeCamera();
		camera->setDynamicCamera();
		break;

	default:
		break;
	}
}

float Player::getSpeed()
{
	return currentSpeed;
}

float Player::getRotationSpeed() 
{
	return rotationSpeed;
}

float Player::getDrag()
{
	return drag;
}

void Player::update(const float elapsedTime, const glm::mat4* parentModelMatrix)
{
	bool movingAllowed = camera->getCameraState() == CAMERA_ON_OBJECT;
	float direction = 0;

	getFrameTime(elapsedTime);

	if (movingAllowed)
	{
		direction = Input::getKeyPressed('w') - Input::getKeyPressed('s');

		float rotation = Input::getKeyPressed('a') - Input::getKeyPressed('d');
		if (rotation != 0)
			rotateDegY(rotationSpeed * frameTime * rotation);

	}
	
	if (direction == 0)
	{
		currentSpeed = currentSpeed - currentSpeed * drag * frameTime;
	}
	else 
	{
		float currentAcceleration = acceleration;
		if (Input::getSpecialKeyPressed(GLUT_KEY_SHIFT_L))
			acceleration *= 2;

		if (direction > 0)
			currentSpeed = glm::min(maxSpeed, currentSpeed + direction * currentAcceleration * frameTime);
		else
			currentSpeed = glm::max(-maxSpeed, currentSpeed + direction * currentAcceleration * frameTime);
	}

	std::cout << currentSpeed << "      FRAME TIME: <<" << frameTime << "\n";


	addPosition(currentSpeed * frameTime * forward);
	ObjectInstance::update(elapsedTime, parentModelMatrix);
}

void Player::setCameraOnObject()
{
	if (cameraOnObject)
		return;

	camera->setCameraState(CAMERA_ON_OBJECT);
	//camera->removeFromParent();
	addChild(camera);
	camera->setPosition(cameraPosition);
	cameraOnObject = true;
}

void Player::freeCamera()
{
	if (!cameraOnObject)
		return;
	removeChild(camera);
	cameraOnObject = false;
}