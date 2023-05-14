#pragma once

#include "singlemesh.h"

class Water : SingleMesh
{
public:
	Water();
	~Water();

	void update(float elapsedTime, const glm::mat4* parentModelMatrix) override;
	void draw() override;
private:
	glm::mat3 texCoordMatrix = glm::mat3(1);
};

