#pragma once

#include "pgr.h"
#include "object.h"
#include "shader.h"

class DirectionalLight;
class PointLight;
class SpotLight;
class Config;
class Camera;
class ObjectInstance;

//#include "data.h"

// parameters of individual objects in the scene (e.g. position, size, speed, etc.)
typedef struct _Object {
	glm::vec3 position;
	glm::vec3 direction;
	float     speed;
	float     size;

	bool destroyed;

	float startTime;
	float currentTime;

} Object;

typedef struct _commonShaderProgram {
	// identifier for the shader program
	GLuint program;          // = 0;
	// vertex attributes locations
	GLint posLocation;       // = -1;
	GLint colorLocation;     // = -1;
	GLint normalLocation;    // = -1;
	GLint texCoordLocation;  // = -1;
	// uniforms locations
	GLint PVMmatrixLocation;    // = -1;
	GLint VmatrixLocation;      // = -1;  view/camera matrix
	GLint MmatrixLocation;      // = -1;  modeling matrix
	GLint normalMatrixLocation; // = -1;  inverse transposed Mmatrix

	GLint timeLocation;         // = -1; elapsed time in seconds

	// material 
	GLint diffuseLocation;    // = -1;
	GLint ambientLocation;    // = -1;
	GLint specularLocation;   // = -1;
	GLint shininessLocation;  // = -1;
	// texture
	GLint useTextureLocation; // = -1; 
	GLint texSamplerLocation; // = -1;
	// reflector related uniforms
	GLint reflectorPositionLocation;  // = -1; 
	GLint reflectorDirectionLocation; // = -1;
} SCommonShaderProgram;

class Render {

public:

	void setMaterialUniforms(const ShaderProgram& shaderProgram, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float shininess, GLuint texture);
	void cleanupShaderPrograms();

	void initialize(Config* config);

	void addShader(Shader* shader);

	void setCamera(Camera* camera);
	void setDirectionalLight(DirectionalLight* light);
	void setPointLight(PointLight* light);
	void setSpotLight(SpotLight* light);
	void setCameraAndLightsUniforms(Shader* shader);

	float getCurrentAspect();
	Shader* getShader(int index);

	void initializeModels();
	void cleanupModels();

	ObjectInstance* getRootNode();

	static Render* getRender();
private:
	ObjectInstance rootNode;
	ShaderList shaders;

	Config* config;
	Camera* camera;
	DirectionalLight* directionalLight;
	PointLight* pointLight;
	SpotLight* spotLight;

	bool initialized = false;
};