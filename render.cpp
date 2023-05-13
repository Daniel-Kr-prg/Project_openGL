#include <time.h>
#include <list>
#include "pgr.h"
#include <iostream>
#include "singlemesh.h"
#include "render.h"
#include "directionallight.h"
#include "pointlight.h"
#include "spotlight.h"


static Render render;

Render* Render::getRender()
{
    return &render;
}

ObjectInstance* Render::getRootNode()
{
    return &rootNode;
}

// ïðè draw ìåòîäå
void setTransformUniforms(const glm::mat4& modelMatrix, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) {

    //glm::mat4 PVM = projectionMatrix * viewMatrix * modelMatrix;
    //glUniformMatrix4fv(shaderProgram.PVMmatrixLocation, 1, GL_FALSE, glm::value_ptr(PVM));

    //glUniformMatrix4fv(shaderProgram.VmatrixLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    //glUniformMatrix4fv(shaderProgram.MmatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    //// just take 3x3 rotation part of the modelMatrix
    //// we presume the last row contains 0,0,0,1
    //const glm::mat4 modelRotationMatrix = glm::mat4(
    //    modelMatrix[0],
    //    modelMatrix[1],
    //    modelMatrix[2],
    //    glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
    //);
    //glm::mat4 normalMatrix = glm::transpose(glm::inverse(modelRotationMatrix));

    ////or an alternative single-line method: 
    ////glm::mat4 normalMatrix = glm::transpose(glm::inverse(glm::mat4(glm::mat3(modelRotationMatrix))));

    //glUniformMatrix4fv(shaderProgram.normalMatrixLocation, 1, GL_FALSE, glm::value_ptr(normalMatrix));  // correct matrix for non-rigid transform
}

// ìàòåðèàë + òåêñòóðà ïðè draw ìåòîäå
void Render::setMaterialUniforms(const ShaderProgram& shaderProgram, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float shininess, GLuint texture) {

    glUniform3fv(shaderProgram.locations.diffuse, 1, glm::value_ptr(diffuse));  // 2nd parameter must be 1 - it declares number of vectors in the vector array
    glUniform3fv(shaderProgram.locations.ambient, 1, glm::value_ptr(ambient));
    glUniform3fv(shaderProgram.locations.specular, 1, glm::value_ptr(specular));
    glUniform1f(shaderProgram.locations.shininess, shininess);

    if (texture != 0) {
        glUniform1i(shaderProgram.locations.useTexture, 1);  // do texture sampling
        glUniform1i(shaderProgram.locations.texSampler, 0);  // texturing unit 0 -> samplerID   [for the GPU linker]
        glActiveTexture(GL_TEXTURE0 + 0);                  // texturing unit 0 -> to be bound [for OpenGL BindTexture]
        glBindTexture(GL_TEXTURE_2D, texture);
    }
    else {
        glUniform1i(shaderProgram.locations.useTexture, 0);  // do not sample the texture
    }
}

// ÏÎÄ ËÎÄÊÓ
void initBoatGeometry(ShaderProgram& shader, ObjectGeometry** geometry) {

    //*geometry = new ObjectGeometry;

    //glGenVertexArrays(1, &((*geometry)->vertexArrayObject));
    //glBindVertexArray((*geometry)->vertexArrayObject);

    //glGenBuffers(1, &((*geometry)->vertexBufferObject));
    //glBindBuffer(GL_ARRAY_BUFFER, (*geometry)->vertexBufferObject);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(ufoVertices), ufoVertices, GL_STATIC_DRAW);

    //glGenBuffers(1, &((*geometry)->elementBufferObject));
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*geometry)->elementBufferObject);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(unsigned int) * ufoTrianglesCount, ufoIndices, GL_STATIC_DRAW);

    //glEnableVertexAttribArray(shader.posLocation);
    //// vertices of triangles - start at the beginning of the array
    //glVertexAttribPointer(shader.posLocation, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), 0);

    //if (useLighting == false) {
    //    glEnableVertexAttribArray(shader.colorLocation);
    //    // color of vertex starts after the position (interlaced arrays)
    //    glVertexAttribPointer(shader.colorLocation, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    //}
    //else {
    //    glEnableVertexAttribArray(shader.normalLocation);
    //    // normal of vertex starts after the color (interlaced array)
    //    glVertexAttribPointer(shader.normalLocation, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
    //}

    //(*geometry)->ambient = glm::vec3(1.0f, 0.0f, 1.0f);
    //(*geometry)->diffuse = glm::vec3(1.0f, 0.0f, 1.0f);
    //(*geometry)->specular = glm::vec3(1.0f, 0.0f, 1.0f);
    //(*geometry)->shininess = 10.0f;
    //(*geometry)->texture = 0;

    //glBindVertexArray(0);

    //(*geometry)->numTriangles = ufoTrianglesCount;
}

/** Initialize vertex buffers and vertex arrays for all objects.
 */
void Render::initializeModels() {

    // ÇÀÃÐÓÆÀÅÌ ÌÎÄÅËÈ


    // ÈÍÈÖÈÀËÈÇÈÐÓÅÌ ËÎÄÊÓ ÂÐÓ×ÍÓÞ
    // fill MeshGeometry structure for skybox object
    //initSkyboxGeometry(skyboxFarPlaneShaderProgram.program, &skyboxGeometry);
}

void cleanupGeometry(ObjectGeometry* geometry) {

    glDeleteVertexArrays(1, &(geometry->vertexArrayObject));
    glDeleteBuffers(1, &(geometry->elementBufferObject));
    glDeleteBuffers(1, &(geometry->vertexBufferObject));

    if (geometry->texture != 0)
        glDeleteTextures(1, &(geometry->texture));
}

void Render::cleanupModels() {

    // for ãåîìåòðèè 
        // cleanupGeometry(...);

}

/**
 * \brief Delete all shader program objects.
 */
void Render::cleanupShaderPrograms(void) {

    //TODO Delete all shaders
}

void Render::initialize(Config* config)
{
    if (config != nullptr)
    {
        this->config = config;
        initialized = true;
    }
}

void Render::addShader(Shader* shader) {
    shaders.push_back(shader);
}

void Render::setCamera(Camera* camera)
{
    this->camera = camera;
}

void Render::setDirectionalLight(DirectionalLight* light)
{
    directionalLight = light;
}

void Render::setPointLight(PointLight* light)
{
    pointLight = light;
}

void Render::setSpotLight(SpotLight* light)
{
    spotLight = light;
}

void Render::setCameraAndLightsUniforms(Shader* shader)
{
    // Camera
    glUniform3fv(shader->getShaderData().locations.viewPosition, 1, glm::value_ptr(camera->getPosition()));
    glUniformMatrix4fv(shader->getShaderData().locations.PMatrix, 1, GL_FALSE, glm::value_ptr(camera->getProjection()));
    glUniformMatrix4fv(shader->getShaderData().locations.VMatrix, 1, GL_FALSE, glm::value_ptr(camera->getView()));

    // Ambient light
    glUniform1f(shader->getShaderData().locations.ambientLightIntensity, config->getAmbientLightIntensity());
    glUniform3fv(shader->getShaderData().locations.ambientLightColor, 1, glm::value_ptr(config->getAmbientLightColor()));

    // Directional light
    if (directionalLight != nullptr)
    {
        directionalLight->setUniforms(shader);
    }
    // Point light
    if (pointLight != nullptr)
    {
        pointLight->setUniforms(shader);
    }
    // Spot light
    if (spotLight != nullptr)
    {
        spotLight->setUniforms(shader);
    }
}

float Render::getCurrentAspect()
{
    //TODO handle resize event
    if (!initialized)
        throw "Render::getCurrentAspect(): Render is not initialized";
    return (float)config->getWindowWidth() / config->getWindowHeight();
}

Shader* Render::getShader(int index)
{
    if (shaders.size() > index && index != -1)
    {
        return shaders[index];
    }
    else
    {
        return nullptr;
    }
}