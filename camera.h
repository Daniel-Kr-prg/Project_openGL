#pragma once

#include <pgr.h>
#include "object.h"

class Camera : public ObjectInstance
{
public:
	//TODO Move to config
	Camera(float fov = 45.0f, float aspect = 16.0f / 9.0f, float zNear = 0.1f, float zFar = 1000.0f, float speed = 0.01f, float keySensitivity = 0.005f, float mouseSensitivity = 0.005f);
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

