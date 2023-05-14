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

protected:
	bool initialized;  ///< object has the shader with defined locations
};

