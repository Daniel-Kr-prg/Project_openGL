#pragma once

#include "pgr.h"
#include <fstream>
#include <sstream>
#include <string>
#include "object.h"
#include "splashshader.h"

const int splashNumQuadVertices = 4;
const float splashVertexData[splashNumQuadVertices * 5] = {

	// x      y     z     u     v
	-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
	 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
	 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
};

class Splash
{
public:
	Splash(SplashShader* shader, std::string textureName, float frameDuration);
	~Splash();

	void splashAtPosition(glm::vec3 position);

	void update(float elapsedTime);
	void draw();

private:
	bool isDrawing = false;
	float drawStartTime = 0;
	float frameDuration;
	float elapsedTime;
	glm::vec3 drawPosition = glm::vec3(0, 1, 0);
	SplashShader* shader;

	ObjectGeometry* geometry;
};