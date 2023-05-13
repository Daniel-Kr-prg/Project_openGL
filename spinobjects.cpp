#include "spinobjects.h"

Bouy::Bouy() : SingleMesh()
{
};

Bouy::~Bouy()
{
};

void Bouy::draw()
{
	glStencilFunc(GL_ALWAYS, 2, 1);
	SingleMesh::draw();
}

void Bouy::update(float elapsedTime, const glm::mat4* parentModelMatrix) {
	getFrameTime(elapsedTime);
	setRotationRad(glm::vec3(0.2f * glm::cos(elapsedTime * rotationPerSecond / 2), elapsedTime * rotationPerSecond, 0.2f * glm::sin(elapsedTime * rotationPerSecond)));
	//rotateRadY(-frameTime * rotationPerSecond);

	ObjectInstance::update(elapsedTime, parentModelMatrix);
}




Airplane::Airplane() : SingleMesh()
{
};

Airplane::~Airplane()
{
};

void Airplane::draw()
{
	glStencilFunc(GL_ALWAYS, 1, 1);
	SingleMesh::draw();
}

void Airplane::update(float elapsedTime, const glm::mat4* parentModelMatrix) {

	float rotation = rotationPerSecond * elapsedTime;

	glm::vec3 pos = getPosition();
	setPosition(glm::vec3(radius * cos(rotation), pos.y, radius * sin(rotation)));
	
	getFrameTime(elapsedTime);
	rotateRadY(- frameTime * rotationPerSecond);

	ObjectInstance::update(elapsedTime, parentModelMatrix);
}