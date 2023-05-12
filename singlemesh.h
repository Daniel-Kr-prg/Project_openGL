#pragma once

#include "object.h"
#include <iostream>
#include "render.h"
#include "camera.h"
#include "config.h"

class SingleMesh : public ObjectInstance
{
public:

	SingleMesh();
	~SingleMesh();

	void update(float elapsedTime, const glm::mat4* parentModelMatrix) override;
	void draw() override;
	void deserialize(nlohmann::json data) override;
private:

	bool loadSingleMesh(std::string fileName, Shader* shader, ObjectGeometry** geometry);

	bool initialized;  ///< object has the shader with defined locations
};

class MovingObject : SingleMesh
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

public:
	MovingObject() : SingleMesh() {};

	float getSpeed() const {
		return currentSpeed;
	}

	float getRotationSpeed() const {
		return rotationSpeed;
	}

	float getDrag() const {
		return drag;
	}

	void timerHandle(std::unordered_map<char, bool> keyPressedState, bool movingAllowed)
	{
		float direction = 0;

		float frameTime = getFrameTime();

		if (movingAllowed)
		{
			direction = keyPressedState['w'] - keyPressedState['s'];

			float rotation = keyPressedState['a'] - keyPressedState['d'];
			if (rotation != 0)
				rotateDegY(rotationSpeed * frameTime * rotation);

		}

		if (direction == 0)
		{
			currentSpeed = currentSpeed - currentSpeed * drag * frameTime;
		}
		else if (direction > 0)
		{
			currentSpeed = glm::min(maxSpeed, currentSpeed + direction * acceleration * frameTime);
		}
		else
		{
			currentSpeed = glm::max(-maxSpeed, currentSpeed + direction * acceleration * frameTime);
		}

		std::cout << currentSpeed << "      FRAME TIME: <<" << frameTime << "\n";


		addPosition(currentSpeed * frameTime * forward);
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

	void freeCamera(ObjectList* objects, InteractableObjects* interactableObjects)
	{
		if (!cameraOnObject)
			return;
		Camera* camera = (Camera*)*cameraIterator;
		children.erase(cameraIterator);
		objects->push_back((ObjectInstance*)camera);
		interactableObjects->cameraIterator = objects->end() - 1;
		cameraOnObject = false;
	}
};
