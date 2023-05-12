#include "spotlight.h"

SpotLight::SpotLight()
{

}

SpotLight::~SpotLight()
{

}

void SpotLight::update(float elapsedTime, const glm::mat4* parentModelMatrix)
{
	ObjectInstance::update(elapsedTime, parentModelMatrix);
	Render::getRender()->setSpotLight(this);
}

void SpotLight::deserialize(nlohmann::json data)
{
	ObjectInstance::deserialize(data);

	if (data.contains("attenuation"))
		attenuation = data["attenuation"].get<float>();
	if (data.contains("intensity"))
		intensity = data["intensity"].get<float>();
	if (data.contains("lightColor"))
		lightColor = readVectorFromJSON(data["lightColor"], glm::vec3(1));
	if (data.contains("innerCutoff"))
		innerCutoff = data["innerCutoff"].get<float>();
	if (data.contains("outerCutoff"))
		outerCutoff = data["outerCutoff"].get<float>();
}

void SpotLight::setUniforms(Shader* shader)
{
	glUniform1f(shader->getShaderData().locations.spotLightAttenuation, attenuation);
	glUniform3fv(shader->getShaderData().locations.spotLightPosition, 1, glm::value_ptr(getPosition()));
	glUniform3fv(shader->getShaderData().locations.spotLightDirection, 1, glm::value_ptr(getForward()));
	glUniform1f(shader->getShaderData().locations.spotLightIntensity, intensity);
	glUniform3fv(shader->getShaderData().locations.spotLightColor, 1, glm::value_ptr(lightColor));
	glUniform1f(shader->getShaderData().locations.spotLightInnerCutoff, innerCutoff);
	glUniform1f(shader->getShaderData().locations.spotLightOuterCutoff, outerCutoff);
}