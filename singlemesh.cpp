#include "singlemesh.h"

void SingleMesh::update(float elapsedTime, const glm::mat4* parentModelMatrix) {
	// instance specific stuff

	// propagate the update to children
	ObjectInstance::update(elapsedTime, parentModelMatrix);
}

void SingleMesh::draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, Camera& camera, Config& config)
{
	if (initialized && (shaderProgram != nullptr)) {
		glUseProgram(shaderProgram->getShaderData().program);

		setMaterialUniforms(
			shaderProgram->getShaderData(),
			geometry->ambient,
			geometry->diffuse,
			geometry->specular,
			geometry->shininess,
			geometry->texture
		);

		glUniform3fv(shaderProgram->getShaderData().locations.viewPosition, 1, glm::value_ptr(camera.getPosition()));

		glUniform1f(shaderProgram->getShaderData().locations.ambientLightIntensity, config.getAmbientLightIntensity());
		glUniform3fv(shaderProgram->getShaderData().locations.ambientLightColor, 1, glm::value_ptr(config.getAmbientLightColor()));
		glUniformMatrix4fv(shaderProgram->getShaderData().locations.normalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(globalModelMatrix))));
		// Directional light
		glUniform3fv(shaderProgram->getShaderData().locations.directionalLightDirection, 1, glm::value_ptr(config.getDirectionalLightDirection()));
		glUniform1f(shaderProgram->getShaderData().locations.directionalLightIntensity, config.getDirectionalLightIntensity());
		glUniform3fv(shaderProgram->getShaderData().locations.directionalLightColor, 1, glm::value_ptr(config.getDirectionalLightColor()));
		// Point light
		glUniform1f(shaderProgram->getShaderData().locations.pointLightAttenuation, config.getPointLightAttenuation());
		glUniform3fv(shaderProgram->getShaderData().locations.pointLightPosition, 1, glm::value_ptr(config.getPointLightPosition()));
		glUniform1f(shaderProgram->getShaderData().locations.pointLightIntensity, config.getPointLightIntensity());
		glUniform3fv(shaderProgram->getShaderData().locations.pointLightColor, 1, glm::value_ptr(config.getPointLightColor()));
		// Spot light
		glUniform1f(shaderProgram->getShaderData().locations.spotLightAttenuation, config.getSpotLightAttenuation());
		glUniform3fv(shaderProgram->getShaderData().locations.spotLightPosition, 1, glm::value_ptr(config.getSpotLightPosition()));
		glUniform3fv(shaderProgram->getShaderData().locations.spotLightDirection, 1, glm::value_ptr(config.getSpotLightDirection()));
		glUniform1f(shaderProgram->getShaderData().locations.spotLightIntensity, config.getSpotLightIntensity());
		glUniform3fv(shaderProgram->getShaderData().locations.spotLightColor, 1, glm::value_ptr(config.getSpotLightColor()));
		glUniform1f(shaderProgram->getShaderData().locations.spotLightInnerCutoff, config.getSpotLightInnerCutoff());
		glUniform1f(shaderProgram->getShaderData().locations.spotLightOuterCutoff, config.getSpotLightOuterCutoff());

		glUniformMatrix4fv(shaderProgram->getShaderData().locations.PMatrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
		glUniformMatrix4fv(shaderProgram->getShaderData().locations.VMatrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));
		glUniformMatrix4fv(shaderProgram->getShaderData().locations.MMatrix, 1, GL_FALSE, glm::value_ptr(globalModelMatrix));

		glBindVertexArray(geometry->vertexArrayObject);
		glDrawElements(GL_TRIANGLES, geometry->numTriangles * 3, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
	else {
		std::cerr << "SingleMesh::draw(): Can't draw, mesh not initialized properly!" << std::endl;
	}
}


/** Load one mesh using assimp library (vertices only, for more attributes see method extended version in Asteroids)
 * \param fileName [in] file to open/load
 * \param shader [in] vao will connect loaded data to shader
 * \param geometry
 */
bool SingleMesh::loadSingleMesh(std::string fileName, Shader* shader, ObjectGeometry** geometry) {
	Assimp::Importer importer;

	// Unitize object in size (scale the model to fit into (-1..1)^3)
	importer.SetPropertyInteger(AI_CONFIG_PP_PTV_NORMALIZE, 1);

	// Load asset from the file - you can play with various processing steps
	const aiScene* scn = importer.ReadFile(fileName.c_str(), 0
		| aiProcess_Triangulate             // Triangulate polygons (if any).
		| aiProcess_PreTransformVertices    // Transforms scene hierarchy into one root with geometry-leafs only. For more see Doc.
		| aiProcess_GenSmoothNormals        // Calculate normals per vertex.
		| aiProcess_JoinIdenticalVertices);

	// abort if the loader fails
	if (scn == NULL) {
		std::cerr << "assimp error: " << importer.GetErrorString() << std::endl;
		geometry = NULL;
		return false;
	}

	// some formats store whole scene (multiple meshes and materials, lights, cameras, ...) in one file, we cannot handle that in our simplified example
	if (scn->mNumMeshes != 1) {
		std::cerr << "this simplified loader can only process files with only one mesh" << std::endl;
		geometry = NULL;
		return false;
	}

	// in this phase we know we have one mesh in our loaded scene, we can directly copy its data to OpenGL ...
	const aiMesh* mesh = scn->mMeshes[0];

	*geometry = new ObjectGeometry;

	// vertex buffer object, store all vertex positions and normals
	glGenBuffers(1, &((*geometry)->vertexBufferObject));
	glBindBuffer(GL_ARRAY_BUFFER, (*geometry)->vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float) * mesh->mNumVertices, 0, GL_STATIC_DRAW); // allocate memory for vertices, normals, and texture coordinates
	// first store all vertices
	glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * sizeof(float) * mesh->mNumVertices, mesh->mVertices);
	// then store all normals
	glBufferSubData(GL_ARRAY_BUFFER, 3 * sizeof(float) * mesh->mNumVertices, 3 * sizeof(float) * mesh->mNumVertices, mesh->mNormals);

	// just texture 0 for now
	float* textureCoords = new float[2 * mesh->mNumVertices];  // 2 floats per vertex
	float* currentTextureCoord = textureCoords;

	// copy texture coordinates
	aiVector3D vect;

	if (mesh->HasTextureCoords(0)) {
		// we use 2D textures with 2 coordinates and ignore the third coordinate
		for (unsigned int idx = 0; idx < mesh->mNumVertices; idx++) {
			vect = (mesh->mTextureCoords[0])[idx];
			*currentTextureCoord++ = vect.x;
			*currentTextureCoord++ = vect.y;
		}
	}

	// finally store all texture coordinates
	glBufferSubData(GL_ARRAY_BUFFER, 6 * sizeof(float) * mesh->mNumVertices, 2 * sizeof(float) * mesh->mNumVertices, textureCoords);

	// copy all mesh faces into one big array (assimp supports faces with ordinary number of vertices, we use only 3 -> triangles)
	unsigned int* indices = new unsigned int[mesh->mNumFaces * 3];
	for (unsigned int f = 0; f < mesh->mNumFaces; ++f) {
		indices[f * 3 + 0] = mesh->mFaces[f].mIndices[0];
		indices[f * 3 + 1] = mesh->mFaces[f].mIndices[1];
		indices[f * 3 + 2] = mesh->mFaces[f].mIndices[2];
	}

	// copy our temporary index array to OpenGL and free the array
	glGenBuffers(1, &((*geometry)->elementBufferObject));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*geometry)->elementBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(unsigned) * mesh->mNumFaces, indices, GL_STATIC_DRAW);

	delete[] indices;

	// copy the material info to MeshGeometry structure
	const aiMaterial* mat = scn->mMaterials[mesh->mMaterialIndex];
	aiColor4D color;
	aiString name;
	aiReturn retValue = AI_SUCCESS;

	// Get returns: aiReturn_SUCCESS 0 | aiReturn_FAILURE -1 | aiReturn_OUTOFMEMORY -3
	mat->Get(AI_MATKEY_NAME, name); // may be "" after the input mesh processing. Must be aiString type!

	if ((retValue = aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &color)) != AI_SUCCESS)
		color = aiColor4D(0.0f, 0.0f, 0.0f, 0.0f);

	(*geometry)->diffuse = glm::vec3(color.r, color.g, color.b);

	if ((retValue = aiGetMaterialColor(mat, AI_MATKEY_COLOR_AMBIENT, &color)) != AI_SUCCESS)
		color = aiColor4D(0.0f, 0.0f, 0.0f, 0.0f);
	(*geometry)->ambient = glm::vec3(color.r, color.g, color.b);

	if ((retValue = aiGetMaterialColor(mat, AI_MATKEY_COLOR_SPECULAR, &color)) != AI_SUCCESS)
		color = aiColor4D(0.0f, 0.0f, 0.0f, 0.0f);
	(*geometry)->specular = glm::vec3(color.r, color.g, color.b);

	ai_real shininess, strength;
	unsigned int max;	// changed: to unsigned

	max = 1;
	if ((retValue = aiGetMaterialFloatArray(mat, AI_MATKEY_SHININESS, &shininess, &max)) != AI_SUCCESS)
		shininess = 1.0f;
	max = 1;
	if ((retValue = aiGetMaterialFloatArray(mat, AI_MATKEY_SHININESS_STRENGTH, &strength, &max)) != AI_SUCCESS)
		strength = 1.0f;
	(*geometry)->shininess = shininess * strength;

	(*geometry)->texture = 0;

	// load texture image
	if (mat->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
		// get texture name 
		aiString path; // filename

		aiReturn texFound = mat->GetTexture(aiTextureType_DIFFUSE, 0, &path);
		std::string textureName = path.data;

		size_t found = fileName.find_last_of("/\\");
		// insert correct texture file path 
		if (found != std::string::npos) { // not found
			//subMesh_p->textureName.insert(0, "/");
			textureName.insert(0, fileName.substr(0, found + 1));
		}

		std::cout << "Loading texture file: " << textureName << std::endl;
		(*geometry)->texture = pgr::createTexture(textureName);
	}
	CHECK_GL_ERROR();

	glGenVertexArrays(1, &((*geometry)->vertexArrayObject));
	glBindVertexArray((*geometry)->vertexArrayObject);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*geometry)->elementBufferObject); // bind our element array buffer (indices) to vao
	glBindBuffer(GL_ARRAY_BUFFER, (*geometry)->vertexBufferObject);

	glEnableVertexAttribArray(shader->getShaderData().locations.position);
	glVertexAttribPointer(shader->getShaderData().locations.position, 3, GL_FLOAT, GL_FALSE, 0, 0);
	CHECK_GL_ERROR();
	glEnableVertexAttribArray(shader->getShaderData().locations.normal);
	glVertexAttribPointer(shader->getShaderData().locations.normal, 3, GL_FLOAT, GL_FALSE, 0, (void*)(3 * sizeof(float) * mesh->mNumVertices));

	glEnableVertexAttribArray(shader->getShaderData().locations.texCoord);
	glVertexAttribPointer(shader->getShaderData().locations.texCoord, 2, GL_FLOAT, GL_FALSE, 0, (void*)(6 * sizeof(float) * mesh->mNumVertices));
	CHECK_GL_ERROR();

	glBindVertexArray(0);

	(*geometry)->numTriangles = mesh->mNumFaces;

	return true;
}


SingleMesh::SingleMesh(const char* filename, Shader* shdrPrg) : ObjectInstance(shdrPrg), initialized(false)
{

	if (!loadSingleMesh(std::string(filename), shdrPrg, &geometry)) {
		if (geometry == nullptr) {
			std::cerr << "SingleMesh::SingleMesh(): geometry not initialized!" << std::endl;
		}
		else {
			std::cerr << "SingleMesh::SingleMesh(): shaderProgram struct not initialized!" << std::endl;
		}
	}
	else {
		if ((shaderProgram != nullptr) && shaderProgram->getShaderData().initialized && (shaderProgram->getShaderData().locations.PMatrix != -1 && shaderProgram->getShaderData().locations.VMatrix != -1 && shaderProgram->getShaderData().locations.MMatrix != -1)) {
			initialized = true;
		}
		else {
			std::cerr << "SingleMesh::SingleMesh(): shaderProgram struct not initialized!" << std::endl;
		}
	}
}

SingleMesh::~SingleMesh() {

	if (geometry != nullptr) {
		glDeleteVertexArrays(1, &(geometry->vertexArrayObject));
		glDeleteBuffers(1, &(geometry->elementBufferObject));
		glDeleteBuffers(1, &(geometry->vertexBufferObject));

		delete geometry;
		geometry = nullptr;
	}

	initialized = false;
}
