#include <iostream>
#include "boat.h"
#include "shader.h"
#include "render.h"

const char* TEXTURE = "data/boat.png";

Boat::Boat() : ObjectInstance(), initialized(false)
{
}

void Boat::draw()
{
	if (initialized && (shaderProgram != nullptr)) {

		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		glStencilFunc(GL_ALWAYS, getIndex(), 0);

		glUseProgram(shaderProgram->getShaderData().program);

		Render::getRender()->setMaterialUniforms(
			shaderProgram->getShaderData(),
			geometry->ambient,
			geometry->diffuse,
			geometry->specular,
			geometry->shininess,
			geometry->texture
		);

		Render::getRender()->setCameraAndLightsUniforms(shaderProgram);

		glm::mat3 texCoordMatrix = glm::mat3(1.0);
		glUniformMatrix3fv(shaderProgram->getShaderData().locations.texCoordMatrix, 1, GL_FALSE, glm::value_ptr(texCoordMatrix));
		glm::mat4 normalMatrix = glm::transpose(glm::inverse(globalModelMatrix));
		glUniformMatrix4fv(shaderProgram->getShaderData().locations.normalMatrix, 1, GL_FALSE, glm::value_ptr(normalMatrix));
		glUniformMatrix4fv(shaderProgram->getShaderData().locations.MMatrix, 1, GL_FALSE, glm::value_ptr(globalModelMatrix));

		glBindVertexArray(geometry->vertexArrayObject);
		glDrawArrays(GL_TRIANGLES, 0, geometry->numTriangles * 3);
		glBindVertexArray(0);

		glDisable(GL_STENCIL_TEST);

		ObjectInstance::draw();
	}
	else {
		std::cerr << "Boat::draw(): Can't draw, mesh not initialized properly!" << std::endl;
	}
}

void Boat::deserialize(nlohmann::json data)
{
	ObjectInstance::deserialize(data);

	shaderProgram = Render::getRender()->getShader(data["shaderIndex"].get<int>());
	geometry = new ObjectGeometry;

	static constexpr float vertices[] = {
		-0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 1.0f, 1.0f,

		-0.5f, -0.5f,  0.5f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f, -0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f, -0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -0.5f, -0.5f, -0.5f, 1.0f, 1.0f,

		-0.5f,  0.5f,  0.5f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f, -0.5f, -0.5f, -0.5f, 1.0f, 1.0f,

		 0.5f,  0.5f,  0.5f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		 0.5f,  0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		 0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f, -0.5f, -0.5f, -0.5f, 1.0f, 1.0f,

		-0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f, -0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 1.0f, 1.0f,

		-0.5f,  0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		 0.5f,  0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f, -0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f,  0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
	};

	geometry->ambient = glm::vec3(1.0);
	geometry->diffuse = glm::vec3(1.0);
	geometry->specular = glm::vec3(0.0);
	geometry->shininess = 1.0;
	geometry->texture = 0;

	geometry->numTriangles = 12;
	geometry->elementBufferObject = 0;

	glGenVertexArrays(1, &geometry->vertexArrayObject);
	glBindVertexArray(geometry->vertexArrayObject);

	glGenBuffers(1, &geometry->vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, geometry->vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	if ((shaderProgram != nullptr) && shaderProgram->getShaderData().initialized && (shaderProgram->getShaderData().locations.position != -1) && (shaderProgram->getShaderData().locations.PMatrix != -1) && (shaderProgram->getShaderData().locations.VMatrix != -1) && (shaderProgram->getShaderData().locations.MMatrix != -1)) {
		glEnableVertexAttribArray(shaderProgram->getShaderData().locations.position);
		glVertexAttribPointer(shaderProgram->getShaderData().locations.position, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(shaderProgram->getShaderData().locations.normal);
		glVertexAttribPointer(shaderProgram->getShaderData().locations.normal, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(shaderProgram->getShaderData().locations.texCoord);
		glVertexAttribPointer(shaderProgram->getShaderData().locations.texCoord, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		initialized = true;
	}
	else {
		std::cerr << "Triangle::Triangle(): shaderProgram struct not initialized!" << std::endl;
	}
}

Boat::~Boat() {
}
