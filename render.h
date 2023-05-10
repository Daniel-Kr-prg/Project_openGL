#ifndef __RENDER_H
#define __RENDER_H

//#include "data.h"

// defines geometry of object in the scene (space ship, ufo, asteroid, etc.)
// geometry is shared among all instances of the same object type
typedef struct _MeshGeometry {
	GLuint        vertexBufferObject;   // identifier for the vertex buffer object
	GLuint        elementBufferObject;  // identifier for the element buffer object
	GLuint        vertexArrayObject;    // identifier for the vertex array object
	unsigned int  numTriangles;         // number of triangles in the mesh
	// material
	glm::vec3     ambient;
	glm::vec3     diffuse;
	glm::vec3     specular;
	float         shininess;
	GLuint        texture;

} MeshGeometry;

// parameters of individual objects in the scene (e.g. position, size, speed, etc.)
typedef struct _Object {
	glm::vec3 position;
	glm::vec3 direction;
	float     speed;
	float     size;

	bool destroyed;

	float startTime;
	float currentTime;

} Object;

typedef struct _commonShaderProgram {
	// identifier for the shader program
	GLuint program;          // = 0;
	// vertex attributes locations
	GLint posLocation;       // = -1;
	GLint colorLocation;     // = -1;
	GLint normalLocation;    // = -1;
	GLint texCoordLocation;  // = -1;
	// uniforms locations
	GLint PVMmatrixLocation;    // = -1;
	GLint VmatrixLocation;      // = -1;  view/camera matrix
	GLint MmatrixLocation;      // = -1;  modeling matrix
	GLint normalMatrixLocation; // = -1;  inverse transposed Mmatrix

	GLint timeLocation;         // = -1; elapsed time in seconds

	// material 
	GLint diffuseLocation;    // = -1;
	GLint ambientLocation;    // = -1;
	GLint specularLocation;   // = -1;
	GLint shininessLocation;  // = -1;
	// texture
	GLint useTextureLocation; // = -1; 
	GLint texSamplerLocation; // = -1;
	// reflector related uniforms
	GLint reflectorPositionLocation;  // = -1; 
	GLint reflectorDirectionLocation; // = -1;
} SCommonShaderProgram;


void initializeShaderPrograms();
void cleanupShaderPrograms();

void initializeModels();
void cleanupModels();

#endif // __RENDER_H