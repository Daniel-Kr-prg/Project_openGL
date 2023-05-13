#version 140 core

out vec4 fragmentColor;

in vec3 texCoord;

uniform SamplerCube skySampler;

void main()
{    
    FragColor = texture(skySampler, texCoord);
}