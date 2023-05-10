#version 140

uniform sampler2D texSampler;

in vec2 fragTexCoord;
out vec4 fragmentColor;

void main() {
  fragmentColor = texture2D(texSampler, fragTexCoord);
}