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

