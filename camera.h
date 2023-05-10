#pragma once

#include <pgr.h>
#include "object.h"
#include <unordered_map>


class Camera : public ObjectInstance
{
public:
	//TODO Move to config
	Camera(float fov, float aspect, float zNear, float zFar, float speed, float keySensitivity, float mouseSensitivity);
	~Camera();

	glm::mat4 getProjection();
	float getSpeed();
	float getKeySensitivity();
	float getMouseSensitivity();
	glm::mat4 getView();

	void addYawPitch(float yaw, float pitch);
private:
	glm::mat4 projectionMatrix;
	float speed;
	float keySensitivity;
	float mouseSensitivity;
	float yaw = 0;
	float pitch = 0;
};

void handleCameraMovement(Camera& camera, float elapsedTime, std::unordered_map<char, bool>& keyPressedState, std::unordered_map<int, bool>& keyPressedSpecialState);


