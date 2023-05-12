#pragma once

#include "object.h"
#include <iostream>
#include "render.h"
#include "camera.h"
#include "config.h"

class SingleMesh : public ObjectInstance
{
public:

	SingleMesh(const char* filename, Shader* shdrPrg = nullptr);
	~SingleMesh();

	void update(float elapsedTime, const glm::mat4* parentModelMatrix) override;
	void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, Camera& camera, Config& config) override;

private:

	bool loadSingleMesh(std::string fileName, Shader* shader, ObjectGeometry** geometry);

	bool initialized;  ///< object has the shader with defined locations
};

class MovingObject : SingleMesh
{
private:
	float maxSpeed = 5.0f;
	float currentSpeed = 0.0f;
	float acceleration = 0.5f;
	float rotationSpeed = 0.75f;
	float drag = 0.1f;

	glm::vec3 cameraPosition = glm::vec3(0.0f, 1.0f, 0.0f);
	std::vector<ObjectInstance*>::iterator cameraIterator;
	bool cameraOnObject = false;

public:
	MovingObject(const char* filename, Shader* shdrPrg = nullptr) : SingleMesh(filename, shdrPrg) {};

	float getSpeed() const {
		return currentSpeed;
	}

	float getRotationSpeed() const {
		return rotationSpeed;
	}

	float getDrag() const {
		return drag;
	}

	void timerHandle(std::unordered_map<char, bool> keyPressedState, float elapsedTime, bool movingAllowed)
	{
		float elapsedSeconds = elapsedTime * 1000;

		float direction = 0;
		if (movingAllowed)
		{
			direction = keyPressedState['w'] - keyPressedState['s'];

			float rotation = keyPressedState['a'] - keyPressedState['d'];
			if (rotation != 0)
				rotateDegY(rotationSpeed * elapsedSeconds * rotation);
		}

		if (direction == 0)
		{
			currentSpeed = glm::max(0.0f, currentSpeed - drag * elapsedSeconds);
		}
		else
		{
			currentSpeed = direction * glm::max(maxSpeed, glm::abs(currentSpeed) + acceleration * elapsedSeconds);
		}

		addPosition(currentSpeed * elapsedTime * forward);
	}

	void setCameraOnObject(ObjectInstance* camera)
	{
		if (cameraOnObject)
			return;
		children.push_back(camera);
		cameraIterator = children.end() - 1;
		camera->setPosition(cameraPosition);
		cameraOnObject = true;
	}
	void freeCamera()
	{
		if (!cameraOnObject)
			return;
		children.erase(cameraIterator);
		cameraOnObject = false;
	}
};
