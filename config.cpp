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

		Render::getRender()->initialize(this);

		if (data.contains("ambient"))
		{
			nlohmann::json ambientData = data["ambient"];

			if (ambientData.contains("intensity"))
				ambientLightIntensity = ambientData["intensity"].get<float>();
			if (ambientData.contains("lightColor"))
				ambientLightColor = readVectorFromJSON(ambientData["lightColor"], glm::vec3(1));
		}

		if (data.contains("shaders"))
		{
			for (nlohmann::json shaderData : data["shaders"])
			{
				if (shaderData.contains("fragmentPath") && shaderData.contains("vertexPath"))
					shaders.push_back({ shaderData["fragmentPath"].get<std::string>(), shaderData["vertexPath"].get<std::string>() });
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

/**
 * \brief Loads objects from json to scene
 * \param scene List of objects in scene
 * \param shaderList List of shaders
 */
void Config::loadScene(ObjectList &scene, InteractableObjects &interactableObjects)
{
	std::ifstream file(configFilename);

	// Load shaders
	for (ShaderObject shaderData : shaders)
	{
		Render::getRender()->addShader(new Shader(shaderData.vertexPath.c_str(), shaderData.fragmentPath.c_str()));
	}

	if (file.is_open())
	{
		nlohmann::json data = nlohmann::json::parse(file);

		if (data.contains("scene"))
		{
			for (nlohmann::json sceneObjectData : data["scene"])
			{
				getNextObject(sceneObjectData, scene, interactableObjects);
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
void Config::getNextObject(nlohmann::json data, ObjectList& scene, InteractableObjects& interactableObjects)
{
	if (data.contains("objectType"))
	{
		ObjectInstance* object = createObjectByType(data["objectType"].get<std::string>(), interactableObjects);

		object->deserialize(data);
		scene.push_back(object);

		//TODO refactor
		if (dynamic_cast<Camera*>(object) != nullptr)
		{
			interactableObjects.cameraIterator = scene.end() - 1;
		}

		if (data.contains("children"))
		{
			for (nlohmann::json childrenData : data["children"])
			{
				getNextObject(childrenData, object->children, interactableObjects);
			}
		}
	}
}

ObjectInstance* Config::createObjectByType(std::string typeName, InteractableObjects& interactableObjects)
{
	if (typeName == "SingleMesh")
		return new SingleMesh();
	else
	if (typeName == "Camera")
	{
		Camera* camera = new Camera();
		interactableObjects.camera = camera;
		return camera;
	}
	else
	if (typeName == "SpotLight")
		return new SpotLight();
	else
	if (typeName == "DirectionalLight")
		return new DirectionalLight();
	else
	if (typeName == "PointLight")
		return new PointLight();
	else
	if (typeName == "MovingObject")
	{
		MovingObject* player = new MovingObject();
		//TODO remove kostil
		interactableObjects.player = player;
		return (ObjectInstance*) player;
	}
	else
		throw "Config::createObjectByType(): Type not found";
}

Config::~Config()
{

}