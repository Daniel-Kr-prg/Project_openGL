#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <pgr.h>
/**
 * \brief Shader program related stuff (id, locations, ...).
 */
typedef struct _SkyShaderProgram {
	/// identifier for the shader program
	GLuint program;

	bool initialized;

	/**
	  * \brief Indices of the vertex shader inputs (locations)
	  */
	struct {
		// vertex attributes locations
		GLint position;
		// uniforms locations
		GLint PMatrix;
		GLint VMatrix;
		GLint skySampler;
		// Fog
		GLint fogHeightStart;
		GLint fogHeightEnd;
		GLint fogColor;
	} locations;

	// ...

	_SkyShaderProgram() : program(0), initialized(false) {
		locations.position = -1;
		locations.PMatrix = -1;
		locations.VMatrix = -1;
		locations.skySampler = -1;
	}

} SkyShaderProgram;

class SkyBoxShader
{
public:
	SkyBoxShader(const char* vertexFilename, const char* fragmentFilename);
	~SkyBoxShader();

	SkyShaderProgram getShaderData();
private:
	SkyShaderProgram shaderData;
};