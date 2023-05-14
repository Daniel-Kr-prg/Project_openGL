#pragma once

#include "pgr.h"
#include <fstream>
#include <sstream>
#include <string>

const int splashNumQuadVertices = 4;
const float splashVertexData[splashNumQuadVertices * 5] = {

	// x      y     z     u     v
	-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
	 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
	 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
};

struct SplashShaderProgram {
	// identifier for the shader program
	GLuint program;              // = 0;
	// vertex attributes locations
	GLint posLocation;           // = -1;
	GLint texCoordLocation;      // = -1;
	// uniforms locations
	GLint PVMmatrixLocation;     // = -1;
	GLint VmatrixLocation;       // = -1;
	GLint timeLocation;          // = -1;
	GLint texSamplerLocation;    // = -1;
	GLint frameDurationLocation; // = -1;

} SplashShaderProgram;

/**
 * \brief Shader program related stuff (id, locations, ...).
 */
//typedef struct _SkyShaderProgram {
//	/// identifier for the shader program
//	GLuint program;
//
//	bool initialized;
//
//	/**
//	  * \brief Indices of the vertex shader inputs (locations)
//	  */
//	struct {
//		// vertex attributes locations
//		GLint position;
//		// uniforms locations
//		GLint PMatrix;
//		GLint VMatrix;
//		GLint texCoord;
//		GLint time;
//		GLint texSampler;
//		GLint frameDuration;
//
//	} locations;
//
//	// ...
//
//	_SkyShaderProgram() : program(0), initialized(false) {
//		locations.position = -1;
//		locations.PMatrix = -1;
//		locations.VMatrix = -1;
//		locations.texCoord = -1;
//		locations.time = -1;
//		locations.texSampler = -1;
//		location.frameDuration = -1;
//	}
//
//} SkyShaderProgram;
//
//class SkyBoxShader
//{
//public:
//	SkyBoxShader(const char* vertexFilename, const char* fragmentFilename);
//	~SkyBoxShader();
//
//	SkyShaderProgram getShaderData();
//private:
//	SkyShaderProgram shaderData;
//};

//void drawExplosion(ExplosionObject* explosion, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) {
//
//	glEnable(GL_BLEND);
//	glBlendFunc(GL_ONE, GL_ONE);
//
//	glUseProgram(explosionShaderProgram.program);
//
////  // just take 3x3 rotation part of the view transform
//glm::mat4 billboardRotationMatrix = glm::mat4(
//	viewMatrix[0],
//	viewMatrix[1],
//	viewMatrix[2],
//	glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
//);
//// inverse view rotation
//billboardRotationMatrix = glm::transpose(billboardRotationMatrix);
//
//glm::mat4 matrix = glm::mat4(1.0f);
//matrix = glm::translate(matrix, explosion->position);
//matrix = glm::scale(matrix, glm::vec3(explosion->size));
//
//matrix = matrix * billboardRotationMatrix; // make billboard to face the camera
//glm::mat4 PVMmatrix = projectionMatrix * viewMatrix * matrix;
//
//glUniformMatrix4fv(explosionShaderProgram.VmatrixLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));   // view

//	glUniformMatrix4fv(explosionShaderProgram.PVMmatrixLocation, 1, GL_FALSE, glm::value_ptr(PVMmatrix));  // model-view-projection
//	glUniform1f(explosionShaderProgram.timeLocation, explosion->currentTime - explosion->startTime);
//	glUniform1i(explosionShaderProgram.texSamplerLocation, 0);
//	glUniform1f(explosionShaderProgram.frameDurationLocation, explosion->frameDuration);
//
//	glBindVertexArray(explosionGeometry->vertexArrayObject);
//	glBindTexture(GL_TEXTURE_2D, explosionGeometry->texture);
//	glDrawArrays(GL_TRIANGLE_STRIP, 0, explosionGeometry->numTriangles);
//
//	glBindVertexArray(0);
//	glUseProgram(0);
//
//	glDisable(GL_BLEND);
//
//	return;
//}

//// push vertex shader and fragment shader
//shaderList.push_back(pgr::createShaderFromFile(GL_VERTEX_SHADER, "explosion.vert"));
//shaderList.push_back(pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "explosion.frag"));
//
//// create the program with two shaders
//explosionShaderProgram.program = pgr::createProgram(shaderList);
//
//// get position and texture coordinates attributes locations
//explosionShaderProgram.posLocation = glGetAttribLocation(explosionShaderProgram.program, "position");
//explosionShaderProgram.texCoordLocation = glGetAttribLocation(explosionShaderProgram.program, "texCoord");
//// get uniforms locations
//explosionShaderProgram.PVMmatrixLocation = glGetUniformLocation(explosionShaderProgram.program, "PVMmatrix");
//explosionShaderProgram.VmatrixLocation = glGetUniformLocation(explosionShaderProgram.program, "Vmatrix");
//explosionShaderProgram.timeLocation = glGetUniformLocation(explosionShaderProgram.program, "time");
//explosionShaderProgram.texSamplerLocation = glGetUniformLocation(explosionShaderProgram.program, "texSampler");
//explosionShaderProgram.frameDurationLocation = glGetUniformLocation(explosionShaderProgram.program, "frameDuration");
//
//// load and compile shader for banner (translation of texture coordinates)
//
//shaderList.clear();