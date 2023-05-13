#include "directionallight.h"

DirectionalLight::DirectionalLight()
{

}

DirectionalLight::~DirectionalLight()
{

}

void DirectionalLight::update(float elapsedTime, const glm::mat4* parentModelMatrix)
{
	ObjectInstance::update(elapsedTime, parentModelMatrix);
	Render::getRender()->setDirectionalLight(this);
}

void DirectionalLight::deserialize(nlohmann::json data)
{
	ObjectInstance::deserialize(data);

	if (data.contains("intensity"))
		intensity = data["intensity"].get<float>();
	if (data.contains("lightColor"))
		lightColor = readVectorFromJSON(data["lightColor"], glm::vec3(1));
}

void DirectionalLight::setUniforms(Shader* shader)
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
	glUniform3fv(shader->getShaderData().locations.directionalLightDirection, 1, glm::value_ptr(globalForward));
	glUniform1f(shader->getShaderData().locations.directionalLightIntensity, intensity);
	glUniform3fv(shader->getShaderData().locations.directionalLightColor, 1, glm::value_ptr(lightColor));
}