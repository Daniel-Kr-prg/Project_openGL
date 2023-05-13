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
	glm::vec3 globalPosition;
	glm::quat globalRotation;
	glm::vec3 globalScale;
	glm::vec3 globalSkew;
	glm::vec4 globalPerspective;
	glm::decompose(globalModelMatrix, globalScale, globalRotation, globalPosition, globalSkew, globalPerspective);
	glm::vec3 globalForward = {
		-2 * (globalRotation.x * globalRotation.z + globalRotation.w * globalRotation.y),
		-2 * (globalRotation.y * globalRotation.z - globalRotation.w * globalRotation.x),
		-(1 - 2 * (globalRotation.x * globalRotation.x + globalRotation.y * globalRotation.y))
	};
	glUniform1f(shader->getShaderData().locations.spotLightAttenuation, attenuation);
	glUniform3fv(shader->getShaderData().locations.spotLightPosition, 1, glm::value_ptr(globalPosition));
	glUniform3fv(shader->getShaderData().locations.spotLightDirection, 1, glm::value_ptr(globalForward));
	glUniform1f(shader->getShaderData().locations.spotLightIntensity, intensity);
	glUniform3fv(shader->getShaderData().locations.spotLightColor, 1, glm::value_ptr(lightColor));
	glUniform1f(shader->getShaderData().locations.spotLightInnerCutoff, innerCutoff);
	glUniform1f(shader->getShaderData().locations.spotLightOuterCutoff, outerCutoff);
}