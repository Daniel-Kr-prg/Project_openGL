#pragma once

#include <string>
#include <vector>
#include <fstream>

#include "json.hpp"
#include "object.h"
#include "singlemesh.h"

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

	void loadScene(ObjectList &scene, ShaderList &shaderList);

private:

	void getNextObject(nlohmann::json data, ObjectList &scene, ShaderList &shaderList);

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

	// Graphics objects
	std::vector<ShaderObject> shaders;
	std::vector<SceneObject> sceneObjects;
};

