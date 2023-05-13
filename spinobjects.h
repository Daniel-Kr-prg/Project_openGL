#pragma once

#include "singlemesh.h"


class Bouy : SingleMesh {
private:
	float rotationPerSecond = 0.5f;

public:
	Bouy();
	~Bouy();
	void draw() override;
	void update(float elapsedTime, const glm::mat4* parentModelMatrix) override;
};


class Airplane : SingleMesh {
private:
	float rotationPerSecond = 0.5f;
	float radius = 2.0f;

public:
	Airplane();
	~Airplane();
	void draw() override;
	void update(float elapsedTime, const glm::mat4* parentModelMatrix) override;
};