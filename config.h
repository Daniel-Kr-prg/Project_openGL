#pragma once

#include <string>
#include <vector>
#include <fstream>

#include "json.hpp"
#include "object.h"



typedef struct _ShaderObject {
	std::string fragmentPath;
	std::string vertexPath;
} ShaderObject;

typedef struct _SceneObject {
	std::string filePath;
	int shaderIndex;
	std::string texturePath;
} SceneObject;


class Config
{
public:
	Config(const char* filename);
	~Config();

	int getWindowWidth();
	int getWindowHeight();
	std::string getWindowTitle();

	float getFov();
	float getZNear();
	float getZFar();
	float getSpeed();
	float getKeySensitivity();
	float getMouseSensitivity();
	float getAmbientLightIntensity();
	glm::vec3 getAmbientLightColor();
	glm::vec3 getDirectionalLightDirection();
	float getDirectionalLightIntensity();
	glm::vec3 getDirectionalLightColor();
	float getPointLightAttenuation();
	glm::vec3 getPointLightPosition();
	float getPointLightIntensity();
	glm::vec3 getPointLightColor();
	float getSpotLightAttenuation();
	glm::vec3 getSpotLightPosition();
	glm::vec3 getSpotLightDirection();
	float getSpotLightIntensity();
	glm::vec3 getSpotLightColor();
	float getSpotLightInnerCutoff();
	float getSpotLightOuterCutoff();

	void loadScene(ObjectList &scene, ShaderList &shaderList, InteractableObjects& interactObjects);

private:

	void getNextObject(nlohmann::json data, ObjectList &scene, ShaderList &shaderList, InteractableObjects& interactObjects);

	const char* configFilename;

	// Window options
	int windowWidth;
	int windowHeight;
	std::string windowTitle;

	// Camera options
	float fov;
	float zNear;
	float zFar;
	float speed;
	float keySensitivity;
	float mouseSensitivity;

	// Light options
	float ambientLightIntensity;
	glm::vec3  ambientLightColor;
	glm::vec3 directionalLightDirection;
	float directionalLightIntensity;
	glm::vec3 directionalLightColor;
	float pointLightAttenuation;
	glm::vec3 pointLightPosition;
	float pointLightIntensity;
	glm::vec3 pointLightColor;
	float spotLightAttenuation;
	glm::vec3 spotLightPosition;
	glm::vec3 spotLightDirection;
	float spotLightIntensity;
	glm::vec3 spotLightColor;
	float spotLightInnerCutoff;
	float spotLightOuterCutoff;

	// Graphics objects
	std::vector<ShaderObject> shaders;
	std::vector<SceneObject> sceneObjects;

};

glm::vec3 readVectorFromJSON(nlohmann::json data, glm::vec3 defaultValue)
{
	if (data.contains("x") && data.contains("y") && data.contains("z"))
		return glm::vec3(data["x"].get<float>(), data["y"].get<float>(), data["z"].get<float>());
	else
		if (data.contains("r") && data.contains("g") && data.contains("b"))
			return glm::vec3(data["r"].get<float>(), data["g"].get<float>(), data["b"].get<float>());
		else
			return defaultValue;
}