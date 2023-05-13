#pragma once

#include "singlemesh.h"


class Bouy : SingleMesh {
private:
	float rotationPerSecond = 0.5f;
	bool lightsOn = true;

public:
	Bouy();
	~Bouy();
	void update(float elapsedTime, const glm::mat4* parentModelMatrix) override;
	void Interact() override;
};


class Airplane : SingleMesh {
private:
	float rotationPerSecond = 0.5f;
	float radius = 2.0f;

	bool falling = false;

public:
	Airplane();
	~Airplane();
	void update(float elapsedTime, const glm::mat4* parentModelMatrix) override;
	void Interact() override;
};