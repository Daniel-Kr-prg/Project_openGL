#include "config.h"
#include "singlemesh.h"

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

		if (data.contains("ambient"))
		{
			nlohmann::json ambientData = data["ambient"];

			if (ambientData.contains("intensity"))
				ambientLightIntensity = ambientData["intensity"].get<float>();
			if (ambientData.contains("lightColor"))
				ambientLightColor = readVector(ambientData["lightColor"], glm::vec3(1));
		}

		if (data.contains("light"))
		{
			nlohmann::json lightData = data["light"];

			if (lightData.contains("directional"))
			{
				nlohmann::json directionalData = lightData["directional"];

				if (directionalData.contains("direction"))
					directionalLightDirection = readVector(directionalData["direction"], glm::vec3(0));
				if (directionalData.contains("intensity"))
					directionalLightIntensity = directionalData["intensity"].get<float>();
				if (directionalData.contains("lightColor"))
					directionalLightColor = readVector(directionalData["lightColor"], glm::vec3(1));
			}

			if (lightData.contains("point"))
			{
				nlohmann::json pointData = lightData["point"];

				if (pointData.contains("attenuation"))
					pointLightAttenuation = pointData["attenuation"].get<float>();
				if (pointData.contains("position"))
					pointLightPosition = readVector(pointData["position"], glm::vec3(0));
				if (pointData.contains("intensity"))
					pointLightIntensity = pointData["intensity"].get<float>();
				if (pointData.contains("lightColor"))
					pointLightColor = readVector(pointData["lightColor"], glm::vec3(1));
			}

			if (lightData.contains("spot"))
			{
				nlohmann::json spotData = lightData["spot"];

				if (spotData.contains("attenuation"))
					spotLightAttenuation = spotData["attenuation"].get<float>();
				if (spotData.contains("position"))
					spotLightPosition = readVector(spotData["position"], glm::vec3(0));
				if (spotData.contains("direction"))
					spotLightDirection = readVector(spotData["direction"], glm::vec3(0));
				if (spotData.contains("intensity"))
					spotLightIntensity = spotData["intensity"].get<float>();
				if (spotData.contains("lightColor"))
					spotLightColor = readVector(spotData["lightColor"], glm::vec3(1));
				if (spotData.contains("innerCutoff"))
					spotLightInnerCutoff = spotData["innerCutoff"].get<float>();
				if (spotData.contains("outerCutoff"))
					spotLightOuterCutoff = spotData["outerCutoff"].get<float>();
			}
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

/**

\brief Returns the width of the window
\return The width of the window
*/
int Config::getWindowWidth()
{
	return windowWidth;
}
/**

\brief Returns the height of the window
\return The height of the window
*/
int Config::getWindowHeight()
{
	return windowHeight;
}
/**

\brief Returns the title of the window
\return The title of the window
*/
std::string Config::getWindowTitle()
{
	return windowTitle;
}
/**

\brief Returns the field of view (FOV) of the camera
\return The field of view (FOV) of the camera
*/
float Config::getFov()
{
	return fov;
}
/**

\brief Returns the near clipping plane value (ZNear) of the camera
\return The near clipping plane value (ZNear) of the camera
*/
float Config::getZNear()
{
	return zNear;
}
/**

\brief Returns the far clipping plane value (ZFar) of the camera
\return The far clipping plane value (ZFar) of the camera
*/
float Config::getZFar()
{
	return zFar;
}
/**

\brief Returns the speed value for movement
\return The speed value for movement
*/
float Config::getSpeed()
{
	return speed;
}
/**

\brief Returns the sensitivity value for keyboard input
\return The sensitivity value for keyboard input
*/
float Config::getKeySensitivity()
{
	return keySensitivity;
}
/**

\brief Returns the sensitivity value for mouse input
\return The sensitivity value for mouse input
*/
float Config::getMouseSensitivity()
{
	return mouseSensitivity;
}

float Config::getAmbientLightIntensity() {
	return ambientLightIntensity;
}

glm::vec3 Config::getAmbientLightColor() {
	return ambientLightColor;
}

glm::vec3 Config::getDirectionalLightDirection() {
	return directionalLightDirection;
}

float Config::getDirectionalLightIntensity() {
	return directionalLightIntensity;
}

glm::vec3 Config::getDirectionalLightColor() {
	return directionalLightColor;
}

float Config::getPointLightAttenuation() {
	return pointLightAttenuation;
}

glm::vec3 Config::getPointLightPosition() {
	return pointLightPosition;
}

float Config::getPointLightIntensity() {
	return pointLightIntensity;
}

glm::vec3 Config::getPointLightColor() {
	return pointLightColor;
}

float Config::getSpotLightAttenuation()
{
	return spotLightAttenuation;
}

glm::vec3 Config::getSpotLightPosition() {
	return spotLightPosition;
}

glm::vec3 Config::getSpotLightDirection() {
	return spotLightDirection;
}

float Config::getSpotLightIntensity() {
	return spotLightIntensity;
}

glm::vec3 Config::getSpotLightColor() {
	return spotLightColor;
}

float Config::getSpotLightInnerCutoff() {
	return spotLightInnerCutoff;
}

float Config::getSpotLightOuterCutoff() {
	return spotLightOuterCutoff;
}

/**
 * \brief Loads objects from json to scene
 * \param scene List of objects in scene
 * \param shaderList List of shaders
 */
void Config::loadScene(ObjectList &scene, ShaderList &shaderList, InteractableObjects& interactObjects)
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
				getNextObject(sceneObjectData, scene, shaderList, interactObjects);
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
void Config::getNextObject(nlohmann::json data, ObjectList &scene, ShaderList &shaderList, InteractableObjects& interactObjects)
{
	if (data.contains("objectId"))
	{
		SceneObject object = sceneObjects[data["objectId"].get<int>()];
		SingleMesh* mesh;

		if (data.contains("type"))
		{
			nlohmann::json typeData = data["type"];
			ObjectType type = typeData.get<ObjectType>();

			switch (type)
			{
				case PLAYER:
					interactObjects.player = new MovingObject(object.filePath.c_str(), shaderList[object.shaderIndex]);;
					mesh = (SingleMesh*)interactObjects.player;
					break;
				default:
					mesh = new SingleMesh(object.filePath.c_str(), shaderList[object.shaderIndex]);
					break;
			}

			mesh->setType(type);
		}
		else
		{
			mesh = new SingleMesh(object.filePath.c_str(), shaderList[object.shaderIndex]);
		}

		if (data.contains("type"))
		{
			nlohmann::json typeData = data["type"];
			ObjectType type = typeData.get<ObjectType>();
			
			mesh->setType(type);
			if (type > STATIC_OBJECT)
			{
				switch (type)
				{
					case PLAYER:
						//interactObjects.player = mesh;
						break;
				}


			}

			
		}

		scene.push_back(mesh);
		
		if (data.contains("next"))
		{
			for (nlohmann::json childrenData : data["next"])
			{
				getNextObject(childrenData, mesh->children, shaderList, interactObjects);
			}
		}
	}
}

Config::~Config()
{

}