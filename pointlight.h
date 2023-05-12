#pragma once

#include "object.h"
#include "json.hpp"
#include "config.h"
#include "render.h"

class PointLight : public ObjectInstance
{
public:
	PointLight();
	~PointLight();

	void update(float elapsedTime, const glm::mat4* parentModelMatrix) override;
	void deserialize(nlohmann::json data) override;

	void setUniforms(Shader* shader);
private:
	float attenuation;
	float intensity;
	glm::vec3 lightColor;
};

