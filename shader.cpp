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
		shaderData.locations.normal = glGetAttribLocation(shaderData.program, "normal");
		shaderData.locations.texCoord = glGetAttribLocation(shaderData.program, "texCoord");

		// other attributes and uniforms
		shaderData.locations.PMatrix = glGetUniformLocation(shaderData.program, "projection");
		shaderData.locations.VMatrix = glGetUniformLocation(shaderData.program, "view");
		shaderData.locations.MMatrix = glGetUniformLocation(shaderData.program, "model");
		shaderData.locations.normalMatrix = glGetUniformLocation(shaderData.program, "normalMatrix");
		// camera
		shaderData.locations.viewPosition = glGetUniformLocation(shaderData.program, "viewPosition");
		// material
		shaderData.locations.diffuse = glGetUniformLocation(shaderData.program, "diffuse");
		shaderData.locations.ambient = glGetUniformLocation(shaderData.program, "ambient");
		shaderData.locations.specular = glGetUniformLocation(shaderData.program, "specular");
		shaderData.locations.shininess = glGetUniformLocation(shaderData.program, "shininess");
		shaderData.locations.texSampler = glGetUniformLocation(shaderData.program, "texSampler");
		// light
		shaderData.locations.ambientLightIntensity = glGetUniformLocation(shaderData.program, "ambientLightIntensity");
		shaderData.locations.ambientLightColor = glGetUniformLocation(shaderData.program, "ambientLightColor");
		shaderData.locations.directionalLightDirection = glGetUniformLocation(shaderData.program, "directionalLightDirection");
		shaderData.locations.directionalLightIntensity = glGetUniformLocation(shaderData.program, "directionalLightIntensity");
		shaderData.locations.directionalLightColor = glGetUniformLocation(shaderData.program, "directionalLightColor");
		shaderData.locations.pointLightPosition = glGetUniformLocation(shaderData.program, "pointLightPosition");
		shaderData.locations.pointLightIntensity = glGetUniformLocation(shaderData.program, "pointLightIntensity");
		shaderData.locations.pointLightColor = glGetUniformLocation(shaderData.program, "pointLightColor");
		shaderData.locations.spotLightPosition = glGetUniformLocation(shaderData.program, "spotLightPosition");
		shaderData.locations.spotLightDirection = glGetUniformLocation(shaderData.program, "spotLightDirection");
		shaderData.locations.spotLightIntensity = glGetUniformLocation(shaderData.program, "spotLightIntensity");
		shaderData.locations.spotLightColor = glGetUniformLocation(shaderData.program, "spotLightColor");
		shaderData.locations.spotLightInnerCutoff = glGetUniformLocation(shaderData.program, "spotLightInnerCutoff");
		shaderData.locations.spotLightOuterCutoff = glGetUniformLocation(shaderData.program, "spotLightOuterCutoff");

		assert(shaderData.locations.PMatrix != -1);
		assert(shaderData.locations.VMatrix != -1);
		assert(shaderData.locations.MMatrix != -1);
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