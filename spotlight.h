#pragma once

#include "object.h"
#include "json.hpp"
#include "config.h"
#include "render.h"

class SpotLight : public ObjectInstance
{
public:
	SpotLight();
	~SpotLight();

	void update(float elapsedTime, const glm::mat4* parentModelMatrix) override;
	void deserialize(nlohmann::json data) override;

	void setUniforms(Shader* shader);
private:
	float attenuation;
	float intensity;
	glm::vec3 lightColor;
	float innerCutoff;
	float outerCutoff;
};

