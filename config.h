#pragma once

#include <string>
#include <vector>
#include <fstream>

#include "json.hpp"
#include "jsonutils.h"
#include "object.h"
#include "directionallight.h"
#include "pointlight.h"
#include "spotlight.h"


typedef struct _ShaderObject {
	std::string fragmentPath;
	std::string vertexPath;
} ShaderObject;

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

	void loadScene(ObjectList &scene, InteractableObjects& interactableObjects);
	ObjectInstance* createObjectByType(std::string typeName, InteractableObjects& interactableObjects);
private:

	void getNextObject(nlohmann::json data, ObjectList &scene, InteractableObjects& interactableObjects);

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

	// Graphics objects
	std::vector<ShaderObject> shaders;

};