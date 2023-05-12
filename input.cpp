#include "input.h"

std::unordered_map<char, bool> keyPressedState;
std::unordered_map<int, bool> keyPressedSpecialState;
int previousMouseX;
int previousMouseY;
int deltaX;
int deltaY;
Config* inputConfig;

void Input::initialize(Config* config)
{
	inputConfig = config;
}

void Input::processKeyPressed(unsigned char keyPressed)
{
	if (keyPressed == 27)
	{
		glutLeaveMainLoop();
		exit(EXIT_SUCCESS);
	}
	else if (keyPressed >= 'A' && keyPressed <= 'Z') {
		char lowerCaseKey = keyPressed + 32; // Преобразование в маленькую букву
		keyPressedState[lowerCaseKey] = true;
	}
	else
	{
		keyPressedState[keyPressed] = true;
	}

	// ON KEY DOWN HANDLE
	switch (keyPressed)
	{
	case 'r':
		//RESTART
		break;
	case 'c':
		//std::cout << "CAMERA POS: " << camera->getPosition().x << " " << camera->getPosition().y << " " << camera->getPosition().z << "\n";
		//std::cout << "YAW PITCH: " << camera->getYaw() << " " << camera->getPitch() << "\n";
		break;
	}

	Render::getRender()->getRootNode()->onKeyPress(keyPressed);
}

void Input::processKeyReleased(unsigned char keyReleased) {
	if (keyReleased >= 'A' && keyReleased <= 'Z') {
		char lowerCaseKey = keyReleased + 32; // Преобразование в маленькую букву
		keyPressedState[lowerCaseKey] = false;
	}
	else
	{
		keyPressedState[keyReleased] = false;
	}
}


void Input::processSpecialKeyPressed(int keyPressed)
{
	keyPressedSpecialState[keyPressed] = true;

	// ON KEY DOWN HANDLE
	switch (keyPressed)
	{
	case GLUT_KEY_F1:
		interactableObjects.player->freeCamera(&objects, &interactableObjects);
		interactableObjects.camera->setStaticView1();
		break;
	case GLUT_KEY_F2:
		interactableObjects.player->freeCamera(&objects, &interactableObjects);
		interactableObjects.camera->setStaticView2();
		break;
	case GLUT_KEY_F3:
		if (interactableObjects.player == nullptr)
			break;
		interactableObjects.camera->setCameraOnObject(interactableObjects.player, interactableObjects.camera, &objects, &interactableObjects);
		break;
	case GLUT_KEY_F4:
		interactableObjects.player->freeCamera(&objects, &interactableObjects);
		interactableObjects.camera->setDynamicCamera();
		break;

	default:
		break;
	}

	Render::getRender()->getRootNode()->onSpecialKeyPress(keyPressed);
}

void Input::processSpecialKeyReleased(int keyReleased)
{
	keyPressedSpecialState[keyReleased] = false;
}

void Input::processPassiveMouseMotion(int mouseX, int mouseY)
{
	int halfWidth = inputConfig->getWindowWidth() / 2;
	int halfHeight = inputConfig->getWindowHeight() / 2;

	int deltaX = previousMouseX - mouseX;
	int deltaY = previousMouseY - mouseY;

	Render::getRender()->getRootNode()->onMouseMove(deltaX, deltaY, mouseX, mouseY);
	interactableObjects.camera->handlePassiveMouseMotion(mouseX, mouseY, config);

	glutWarpPointer(halfWidth, halfHeight);

	previousMouseX = halfWidth;
	previousMouseY = halfHeight;
}

bool Input::getKeyPressed(unsigned char keyPressed)
{
	return keyPressedSpecialState[keyPressed];
}

bool Input::getSpecialKeyPressed(unsigned char keyPressed)
{
	return keyPressedState[keyPressed];
}