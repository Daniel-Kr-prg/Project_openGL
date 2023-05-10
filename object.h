#pragma once

#include "pgr.h"
#include <glm/gtx/quaternion.hpp>
#include "shader.h"

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

class ObjectInstance;
/**
 * \brief Linear representation of the scene objects.  The objects themselves may represent the subtrees.
 */
typedef std::vector<ObjectInstance*> ObjectList;
typedef std::vector<Shader*> ShaderList;

class ObjectInstance {

protected:

	ObjectGeometry* geometry;
	glm::mat4		localModelMatrix;
	glm::mat4		globalModelMatrix;

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

public:

	ObjectList children;

	/**
	 * \brief ObjectInstance constructor. Takes a pointer to the shader and must create object resources (VBO and VAO)
	 * \param shdrPrg pointer to the shader program for rendering objects data
	 */
	ObjectInstance(Shader* shdrPrg = nullptr) : geometry(nullptr), shaderProgram(shdrPrg) {
		this->position = { 0, 0, 0 };
		this->rotation = glm::quat(1, 0, 0, 0);
		this->scale = { 1, 1, 1 };
		updateLocalVectors();
	}

	~ObjectInstance() {}
  
	/**
	 * \brief Returns the current local position
	 */
	glm::vec3 getPosition() const {
		return position;
	}

	/**
	 * \brief Sets the current local position
	 * \param position New local position
	 */
	void setPosition(glm::vec3 position) {
		this->position = position;
		updateLocalMatrix();
	}

	/**
	 * \brief Returns the current local rotation (Euler angeles, in radians)
	 */
	glm::vec3 getRotationRad() const {
		return glm::vec3(glm::eulerAngles(rotation));
	}

	/**
	 * \brief Returns the current local rotation (Euler angeles, in degrees)
	 */
	glm::vec3 getRotationDeg() const {
		return glm::vec3(glm::eulerAngles(rotation)) * 180.0f / glm::pi<float>();
	}

	/**
	 * \brief Returns the current local rotation (quaternion)
	 */
	glm::quat getRotation() const {
		return rotation;
	}

	/**
	 * \brief Sets the current local rotation
	 * \param rotation New local rotation (Euler angeles, in radians)
	 */
	void setRotationRad(glm::vec3 rotation) {
		this->rotation = glm::quat(rotation);
		updateLocalVectors();
	}

	/**
	 * \brief Sets the current local rotation
	 * \param rotation New local rotation (Euler angeles, in degrees)
	 */
	void setRotationDeg(glm::vec3 rotation) {
		this->rotation = glm::quat(rotation / 180.0f * glm::pi<float>());
		updateLocalVectors();
	}

	/**
	 * \brief Sets the current local rotation
	 * \param rotation New local rotation (quaternion)
	 */
	void setRotation(glm::quat rotation) {
		this->rotation = rotation;
		updateLocalVectors();
	}

	/**
	 * \brief Returns the current local scale
	 */
	glm::vec3 getScale() const {
		return scale;
	}

	/**
	 * \brief Sets the current local scale
	 * \param scale New local scale
	 */
	void setScale(glm::vec3 scale) {
		this->scale = scale;
		updateLocalMatrix();
	}

	/**
	 * \brief Returns the current local up vector
	 */
	glm::vec3 getUp() const {
		return up;
	}

	/**
	 * \brief Returns the current local right vector
	 */
	glm::vec3 getRight() const {
		return right;
	}

	/**
	 * \brief Returns the current local forward vector
	 */
	glm::vec3 getForward() const {
		return forward;
	}

	/**
	 * \brief Rotates the object around global X axis
	 * \param angleRad Angle in radians
	 */
	void rotateRadX(float angleRad) {
		rotation = glm::rotate(rotation, angleRad, { 1, 0, 0 });
		updateLocalVectors();
	}

	/**
	 * \brief Rotates the object around global Y axis
	 * \param angleRad Angle in radians
	 */
	void rotateRadY(float angleRad) {
		rotation = glm::rotate(rotation, angleRad, { 0, 1, 0 });
		updateLocalVectors();
	}

	/**
	 * \brief Rotates the object around global Z axis
	 * \param angleRad Angle in radians
	 */
	void rotateRadZ(float angleRad) {
		rotation = glm::rotate(rotation, angleRad, { 0, 0, 1 });
		updateLocalVectors();
	}

	/**
	 * \brief Rotates the object around global X axis
	 * \param angleRad Angle in degrees
	 */
	void rotateDegX(float angleRad) {
		rotation = glm::rotate(rotation, float(angleRad / 180.0f * glm::pi<float>()), { 0, 0, 1 });
		updateLocalVectors();
	}

	/**
	 * \brief Rotates the object around global Y axis
	 * \param angleRad Angle in degrees
	 */
	void rotateDegY(float angleRad) {
		rotation = glm::rotate(rotation, float(angleRad / 180.0f * glm::pi<float>()), { 0, 1, 0 });
		updateLocalVectors();
	}

	/**
	 * \brief Rotates the object around global Z axis
	 * \param angleRad Angle in degrees
	 */
	void rotateDegZ(float angleRad) {
		rotation = glm::rotate(rotation, float(angleRad / 180.0f * glm::pi<float>()), { 0, 0, 1 });
		updateLocalVectors();
	}

	/**
	* \brief Recalculates the global matrix and updates all children.
	*   Derived classes should also call this method (using ObjectInstance::update()).
	* \param elapsedTime time value in seconds, such as 0.001*glutGet(GLUT_ELAPSED_TIME) (conversion milliseconds => seconds)
	* \param parentModelMatrix parent transformation in the scene-graph subtree
	*/
	virtual void update(const float elapsedTime, const glm::mat4* parentModelMatrix) {
		// update model matrix - localModelMatrix - of the instance 
		// ...

		// if we have parent, multiply parent's matrix with ours
		if (parentModelMatrix != nullptr)
			globalModelMatrix = *parentModelMatrix * localModelMatrix;
		else
			globalModelMatrix = localModelMatrix;

		// update all children
		for (ObjectInstance* child : children) {
			if (child != nullptr)
				child->update(elapsedTime, &globalModelMatrix);
		}
	}

	/**
	 * \brief Draw instance geometry and calls the draw() on child nodes.
	 */
	virtual void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) {
		// draw instance geometry using globalModelMatrix
		// ...

		// process all children
		for (ObjectInstance* child : children) {   //for (auto child : children) {
			if (child != nullptr)
				child->draw(viewMatrix, projectionMatrix);
		}
	}

protected:

	/**
	 * \brief Updates object's local vectors (forward, up, right). Call after any rotation changes.
	 */
	void updateLocalVectors() {
		forward = { -2 * (rotation.x * rotation.z + rotation.w * rotation.y), -2 * (rotation.y * rotation.z - rotation.w * rotation.x), -(1 - 2 * (rotation.x * rotation.x + rotation.y * rotation.y)) };
		forward = glm::normalize(forward);
		right = glm::normalize(glm::cross(forward, { 0, 1, 0 }));
		up = glm::normalize(glm::cross(right, forward));
		updateLocalMatrix();
	}

	/**
	 * \brief Updates object's model matrix. Call after any transform changes.
	 */
	void updateLocalMatrix() {
		localModelMatrix = glm::mat4(1.0f);
		localModelMatrix = glm::translate(localModelMatrix, (glm::vec3)position);
		localModelMatrix = localModelMatrix * glm::toMat4(rotation);
		localModelMatrix = glm::scale(localModelMatrix, (glm::vec3)scale);
	}
};
