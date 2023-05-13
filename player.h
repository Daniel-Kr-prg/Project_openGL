#pragma once

#include "singlemesh.h"

class Player : public SingleMesh
{
private:
	float maxSpeed = 5.0f;
	float currentSpeed = 0.0f;
	float acceleration = 1.0f;
	float rotationSpeed = 60.0f;
	float drag = 0.75f;

	glm::vec3 cameraPosition = glm::vec3(0.0f, 6.0f, 5.0f);
	std::vector<ObjectInstance*>::iterator cameraIterator;
	bool cameraOnObject = false;
	Camera* camera;

public:
	Player();
	~Player();

	float getSpeed();

	float getRotationSpeed();

	float getDrag();

	void setCameraOnObject();

	void freeCamera();

	void initialize() override;
	void update(const float elapsedTime, const glm::mat4* parentModelMatrix) override;
	void onSpecialKeyPress(int key) override;
};

