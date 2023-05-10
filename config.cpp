#include "config.h"

Config::Config(const char* filename)
{
	configFilename = filename;

	std::ifstream file(filename);

	if (file.is_open())
	{
		nlohmann::json data = nlohmann::json::parse(file);
		
		if (data.contains("window"))
		{
			nlohmann::json windowData = data["window"];
			if (windowData.contains("windowWidth"))
				windowWidth = windowData["windowWidth"].get<int>();
			if (windowData.contains("windowHeight"))
				windowHeight = windowData["windowHeight"].get<int>();
			if (windowData.contains("windowTitle"))
				windowTitle = windowData["windowTitle"].get<std::string>();
		}

		if (data.contains("camera"))
		{
			nlohmann::json cameraData = data["camera"];
			if (cameraData.contains("fov"))
				fov = cameraData["fov"].get<float>();
			if (cameraData.contains("zNear"))
				zNear = cameraData["zNear"].get<float>();
			if (cameraData.contains("zFar"))
				zFar = cameraData["zFar"].get<float>();
			if (cameraData.contains("speed"))
				speed = cameraData["speed"].get<float>();
			if (cameraData.contains("keySensitivity"))
				keySensitivity = cameraData["keySensitivity"].get<float>();
			if (cameraData.contains("mouseSensitivity"))
				mouseSensitivity = cameraData["mouseSensitivity"].get<float>();
		}

		if (data.contains("shaders"))
		{
			for (nlohmann::json shaderData : data["shaders"])
			{
				if (shaderData.contains("fragmentPath") && shaderData.contains("vertexPath"))
					shaders.push_back({ shaderData["fragmentPath"].get<std::string>(), shaderData["vertexPath"].get<std::string>() });
			}
		}

		if (data.contains("sceneObjects"))
		{
			for (nlohmann::json objectData : data["sceneObjects"])
			{
				if (objectData.contains("filePath") && objectData.contains("shaderIndex") && objectData.contains("texturePath"))
					sceneObjects.push_back({ objectData["filePath"].get<std::string>(), objectData["shaderIndex"].get<int>(), objectData["texturePath"].get<std::string>() });
			}
		}
	}
}

int Config::getWindowWidth()
{
	return windowWidth;
}

int Config::getWindowHeight()
{
	return windowHeight;
}

std::string Config::getWindowTitle()
{
	return windowTitle;
}

float Config::getFov()
{
	return fov;
}

float Config::getZNear()
{
	return zNear;
}

float Config::getZFar()
{
	return zFar;
}

float Config::getSpeed()
{
	return speed;
}

float Config::getKeySensitivity()
{
	return keySensitivity;
}

float Config::getMouseSensitivity()
{
	return mouseSensitivity;
}

/**
 * \brief Loads objects from json to scene
 * \param scene List of objects in scene
 * \param shaderList List of shaders
 */
void Config::loadScene(ObjectList &scene, ShaderList &shaderList)
{
	std::ifstream file(configFilename);

	// Load shaders
	for (ShaderObject shaderData : shaders)
	{
		shaderList.push_back(new Shader(shaderData.vertexPath.c_str(), shaderData.fragmentPath.c_str()));
	}

	if (file.is_open())
	{
		nlohmann::json data = nlohmann::json::parse(file);

		if (data.contains("scene"))
		{
			for (nlohmann::json sceneObjectData : data["scene"])
			{
				getNextObject(sceneObjectData, scene, shaderList);
			}
		}
	}
}

/**
 * \brief Processes next json scene object
 * \param data Json data
 * \param scene Scene objects list
 * \param shaderList List of shaders
 */
void Config::getNextObject(nlohmann::json data, ObjectList &scene, ShaderList &shaderList)
{
	if (data.contains("objectId"))
	{
		SceneObject object = sceneObjects[data["objectId"].get<int>()];
		SingleMesh* mesh = new SingleMesh(object.filePath.c_str(), shaderList[object.shaderIndex]);

		if (data.contains("position"))
		{
			nlohmann::json positionData = data["position"];

			if (positionData.contains("x") && positionData.contains("y") && positionData.contains("z"))
				mesh->setPosition(glm::vec3(positionData["x"].get<float>(), positionData["y"].get<float>(), positionData["z"].get<float>()));
		}

		if (data.contains("rotation"))
		{
			nlohmann::json positionData = data["rotation"];

			if (positionData.contains("x") && positionData.contains("y") && positionData.contains("z"))
				mesh->setRotationRad(glm::vec3(positionData["x"].get<float>(), positionData["y"].get<float>(), positionData["z"].get<float>()));
		}

		if (data.contains("scale"))
		{
			nlohmann::json positionData = data["scale"];

			if (positionData.contains("x") && positionData.contains("y") && positionData.contains("z"))
				mesh->setScale(glm::vec3(positionData["x"].get<float>(), positionData["y"].get<float>(), positionData["z"].get<float>()));
		}

		scene.push_back(mesh);
		
		if (data.contains("next"))
		{
			for (nlohmann::json childrenData : data["next"])
			{
				getNextObject(childrenData, mesh->children, shaderList);
			}
		}
	}
}

Config::~Config()
{

}