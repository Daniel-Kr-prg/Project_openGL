#pragma once


#include "pgr.h"
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <unordered_map>
#include "json.hpp"
#include "jsonutils.h"

class Config;
class Camera;
class Shader;
class ObjectInstance;

/**
 * \brief Geometry of an object (vertices, triangles).
 */
typedef struct _ObjectGeometry {
	GLuint        vertexBufferObject;   ///< identifier for the vertex buffer object
	GLuint        elementBufferObject;  ///< identifier for the element buffer object
	GLuint        vertexArrayObject;    ///< identifier for the vertex array object
	unsigned int  numTriangles;         ///< number of triangles in the mesh

	// material
	glm::vec3     ambient;
	glm::vec3     diffuse;
	glm::vec3     specular;
	float         shininess;
	GLuint        texture;
} ObjectGeometry;

/**
 * \brief Linear representation of the scene objects.  The objects themselves may represent the subtrees.
 */
typedef std::list<ObjectInstance*> ObjectList;
typedef std::vector<Shader*> ShaderList;

class ObjectInstance {

protected:

	ObjectGeometry* geometry;
	glm::mat4		localModelMatrix;
	glm::mat4		globalModelMatrix;

	glm::vec3 globalPosition;
	glm::quat globalRotation;
	glm::vec3 globalScale;
	glm::vec3 globalForward;

	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;

	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 forward;

	// dynamic objects
	// glm::vec3 direction;
	// float     speed;
	// ...

	Shader* shaderProgram;

	float lastUpdateTime;
	float frameTime;
	ObjectList children;
	ObjectInstance* parent = nullptr;
	std::list<ObjectInstance*>::iterator iteratorAtParent;

public:
	ObjectInstance(Shader* shdrPrg = nullptr);
	~ObjectInstance();
  
	void addChild(ObjectInstance* child);
	void removeChild(ObjectInstance* childToRemove);
	void removeFromParent();

	glm::vec3 getPosition();
	void setPosition(glm::vec3 position);
	void addPosition(glm::vec3 move);

	glm::vec3 getRotationRad();
	glm::vec3 getRotationDeg();
	glm::quat getRotation();

	void setRotationRad(glm::vec3 rotation);
	void setRotationDeg(glm::vec3 rotation);
	void setRotation(glm::quat rotation);

	glm::vec3 getScale();
	void setScale(glm::vec3 scale);

	glm::vec3 getUp();
	glm::vec3 getRight();
	glm::vec3 getForward();

	glm::vec3 getGlobalPosition();
	glm::quat getGlobalRotation();
	glm::vec3 getGlobalScale();
	glm::vec3 getGlobalForward();

	void rotateRadX(float angleRad);
	void rotateRadY(float angleRad);
	void rotateRadZ(float angleRad);

	void rotateDegX(float angleRad);
	void rotateDegY(float angleRad);
	void rotateDegZ(float angleRad);

	float getLastUpdateTime();

	float getFrameTime(float elapsedTime);

	template<class T>
	T* firstNodeByType() 
	{
		T* result = dynamic_cast<T*>(this);
		if (result != nullptr)
			return result;

		for (ObjectInstance* child : children) {
			if (child != nullptr)
			{
				result = child->firstNodeByType<T>();
				if (result != nullptr)
					return result;
			}
		}

		return nullptr;
	}

	virtual void update(const float elapsedTime, const glm::mat4* parentModelMatrix);

	virtual void draw();

	virtual void initialize();

	virtual void onKeyPress(unsigned char key);

	virtual void onSpecialKeyPress(int key);

	virtual void onMouseMove(int deltaX, int deltaY, int mouseX, int mouseY);

	virtual void deserialize(nlohmann::json data);

protected:
	void printGraph(int level);

	void updateLocalVectors();
	void updateLocalMatrix();
	void updateWorldMatrix(glm::mat4 parentMatrix);
};





 