#include <iostream>
#include "triangle.h"
#include "shader.h"

const char* TEXTURE = "data/boat.png";

void Triangle::update(float elapsedTime, const glm::mat4* parentModelMatrix) {
	// instance specific stuff
// ======== BEGIN OF SOLUTION - TASK 1_0-3 ======== //
// ========  END OF SOLUTION - TASK 1_0-3  ======== //

	// propagate the update to children
	ObjectInstance::update(elapsedTime, parentModelMatrix);
}

void Triangle::draw()
{
	//if(initialized && (shaderProgram != nullptr)) {
	//	glUseProgram(shaderProgram->getShaderData().program);

	//	glm::mat4 PVMmatrix = projectionMatrix * viewMatrix * globalModelMatrix;
	//	glUniformMatrix4fv(shaderProgram->getShaderData().locations.PVMmatrix, 1, GL_FALSE, glm::value_ptr(PVMmatrix));

	//	glBindVertexArray(geometry->vertexArrayObject);
	//	glDrawArrays(GL_TRIANGLES, 0, 36);
	//	glBindVertexArray(0);
	//}
	//else {
	//	std::cerr << "Triangle::draw(): Can't draw, triangle not initialized properly!" << std::endl;
	//}
}

Triangle::Triangle(Shader* shdrPrg) : ObjectInstance(shdrPrg), initialized(false)
{
	geometry = new ObjectGeometry;

	static constexpr float vertices[] = {
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
	};

	geometry->numTriangles = 1;
	geometry->elementBufferObject = 0;

	glGenVertexArrays(1, &geometry->vertexArrayObject);
	glBindVertexArray(geometry->vertexArrayObject);

	glGenBuffers(1, &geometry->vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, geometry->vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	if ((shaderProgram != nullptr) && shaderProgram->getShaderData().initialized && (shaderProgram->getShaderData().locations.position != -1) && (shaderProgram->getShaderData().locations.PMatrix != -1) && (shaderProgram->getShaderData().locations.VMatrix != -1) && (shaderProgram->getShaderData().locations.MMatrix != -1)) {
		glEnableVertexAttribArray(shaderProgram->getShaderData().locations.position);
		glVertexAttribPointer(shaderProgram->getShaderData().locations.position, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		initialized = true;
	}
	else {
		std::cerr << "Triangle::Triangle(): shaderProgram struct not initialized!" << std::endl;
	}
}

Triangle::~Triangle() {
	glDeleteVertexArrays(1, &(geometry->vertexArrayObject));
	glDeleteBuffers(1, &(geometry->elementBufferObject));
	glDeleteBuffers(1, &(geometry->vertexBufferObject));

	delete geometry;
	geometry = nullptr;

	initialized = false;
}
