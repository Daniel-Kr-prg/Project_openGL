#pragma once

#include "object.h"

class Triangle : public ObjectInstance
{
public:

	Triangle(Shader* shdrPrg = nullptr);
	~Triangle();

	void update(float elapsedTime, const glm::mat4* parentModelMatrix) override;
	void draw() override;

private:

	bool initialized;  ///< object has the shader with defined locations
};

