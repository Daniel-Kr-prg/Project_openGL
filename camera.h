#pragma once

#include <pgr.h>
#include "object.h"
#include <unordered_map>
#include "input.h"

enum camState { STATIC_CAMERA, CAMERA_ON_OBJECT, DYNAMIC_CAMERA };

class Camera : public ObjectInstance
{
public:
	Camera();
	Camera(float fov, float aspect, float zNear, float zFar, float speed, float keySensitivity, float mouseSensitivity);
	~Camera();

	glm::mat4 getProjection();
	float getSpeed();
	float getKeySensitivity();
	float getMouseSensitivity();
	glm::mat4 getView();

	void addYawPitch(float yaw, float pitch);
	void setYawPitch(float yaw, float pitch);
	float getYaw();
	float getPitch();

	camState getCameraState();
	void setCameraState(camState newState);

	void setCameraView(glm::vec3 position, float yaw, float pitch);

	void setStaticView1();
	void setStaticView2();
	void setDynamicCamera();

	void initialize() override;
	void update(float elapsedTime, const glm::mat4* parentModelMatrix) override;
	void deserialize(nlohmann::json data) override;
	void onMouseMove(int deltaX, int deltaY, int mouseX, int mouseY) override;
private:
	glm::mat4 projectionMatrix;
	float speed;
	float keySensitivity;
	float mouseSensitivity;
	float yaw = 0;
	float pitch = 0;

	int previousMouseX = 0;
	int previousMouseY = 0;

	camState cameraState = STATIC_CAMERA;
};