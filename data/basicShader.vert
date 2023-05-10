#version 140

in vec2 texCoord;
in vec3 position;

out vec2 fragTexCoord;
uniform mat4 PVM;

void main() {
  gl_Position = PVM * vec4(position, 1.0f);
  fragTexCoord = texCoord;
}