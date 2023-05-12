#pragma once

#include "object.h"
#include "json.hpp"
#include "config.h"
#include "render.h"

class DirectionalLight : public ObjectInstance
{
public:
	DirectionalLight();
	~DirectionalLight();

	void update(float elapsedTime, const glm::mat4* parentModelMatrix) override;
	void deserialize(nlohmann::json data) override;

	void setUniforms(Shader* shader);
private:
	float intensity;
	glm::vec3 lightColor;
};

