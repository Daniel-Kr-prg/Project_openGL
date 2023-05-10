#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <pgr.h>

/**
 * \brief Shader program related stuff (id, locations, ...).
 */
typedef struct _ShaderProgram {
	/// identifier for the shader program
	GLuint program;

	bool initialized;

	/**
	  * \brief Indices of the vertex shader inputs (locations)
	  */
	struct {
		// vertex attributes locations
		GLint position;
		GLint normal;
		GLint texCoord;
		// uniforms locations
		GLint PVMmatrix;
		GLint time;
		GLint diffuse;
		GLint ambient;
		GLint specular;
		GLint shininess;
		GLint texSampler;
	} locations;

	// ...

	_ShaderProgram() : program(0), initialized(false) {
		locations.position = -1;
		locations.PVMmatrix = -1;
	}

} ShaderProgram;

class Shader
{
public:
	Shader(const char* vertexFilename, const char* fragmentFilename);
	~Shader();

	ShaderProgram getShaderData();
private: 
	ShaderProgram shaderData;
};

