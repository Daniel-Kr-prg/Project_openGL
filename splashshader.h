#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <pgr.h>

typedef struct _SplashShaderProgram {
	// identifier for the shader program
	GLuint program;              // = 0;
	// uniforms locations
	struct {
		// vertex attributes locations
		GLint position;
		GLint texCoord;
		// uniforms locations
		GLint Pmatrix;
		GLint Vmatrix;
		GLint Mmatrix;
		GLint time;
		GLint texSampler;
		GLint frameDuration;
	} locations;

} SplashShaderProgram;


class SplashShader
{
public:
	SplashShader(const char* vertexFilename, const char* fragmentFilename);
	~SplashShader();

	SplashShaderProgram getShaderData();
private:
	SplashShaderProgram shaderData;
};