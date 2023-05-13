#version 140

out vec4 fragmentColor;

in vec3 texCoord;

uniform samplerCube skySampler;

void main()
{    
    fragmentColor = texture(skySampler, texCoord);
}