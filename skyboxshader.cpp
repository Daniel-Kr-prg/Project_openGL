#include "skyboxshader.h"

SkyBoxShader::SkyBoxShader(const char* vertexFilename, const char* fragmentFilename)
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
		shaderData.locations.PMatrix = glGetUniformLocation(shaderData.program, "projection");
		shaderData.locations.VMatrix = glGetUniformLocation(shaderData.program, "view");
		shaderData.locations.skySampler = glGetUniformLocation(shaderData.program, "skySampler");
		
		assert(shaderData.locations.position != -1);
		assert(shaderData.locations.PMatrix != -1);
		assert(shaderData.locations.VMatrix != -1);
		assert(shaderData.locations.skySampler != -1);
		// ...

		shaderData.initialized = true;
	}
}

SkyShaderProgram SkyBoxShader::getShaderData()
{
	return shaderData;
}

SkyBoxShader::~SkyBoxShader()
{
	pgr::deleteProgramAndShaders(shaderData.program);
}