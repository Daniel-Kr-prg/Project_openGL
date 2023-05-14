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
	float getFogStart();
	float getFogEnd();
	glm::vec3 getFogColor();

	void loadScene(ObjectInstance& rootNode);
	ObjectInstance* createObjectByType(std::string typeName);
private:

	void getNextObject(nlohmann::json data, ObjectInstance& node);

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
	float fogStart;
	float fogEnd;
	glm::vec3 fogColor;

	// Graphics objects
	std::vector<ShaderObject> shaders;

};