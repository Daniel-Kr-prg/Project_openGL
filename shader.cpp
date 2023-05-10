#include "shader.h"

Shader::Shader(const char* vertexFilename, const char* fragmentFilename)
{
	std::ifstream vertexFile(vertexFilename);
	std::ifstream fragmentFile(fragmentFilename);

	if (vertexFile.is_open() && fragmentFile.is_open())
	{
		std::ostringstream vertexShaderSrc;
		std::ostringstream fragmentShaderSrc;

		vertexShaderSrc << vertexFile.rdbuf();
		fragmentShaderSrc << fragmentFile.rdbuf();

		GLuint shaders[] = {
			pgr::createShaderFromSource(GL_VERTEX_SHADER, vertexShaderSrc.str()),
			pgr::createShaderFromSource(GL_FRAGMENT_SHADER, fragmentShaderSrc.str()),
			0
		};

		shaderData.program = pgr::createProgram(shaders);
		shaderData.locations.position = glGetAttribLocation(shaderData.program, "position");

		// other attributes and uniforms
		shaderData.locations.PVMmatrix = glGetUniformLocation(shaderData.program, "PVM");

		assert(shaderData.locations.PVMmatrix != -1);
		assert(shaderData.locations.position != -1);
		// ...

		shaderData.initialized = true;
	}
}

ShaderProgram Shader::getShaderData()
{
	return shaderData;
}

Shader::~Shader()
{
	pgr::deleteProgramAndShaders(shaderData.program);
}