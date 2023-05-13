#version 140

in vec2 texCoord;
in vec3 normal;
in vec3 position;

out vec3 fragNormal;
out vec3 fragPosition;
out vec2 fragTexCoord;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 normalMatrix;

void main() {
  vec4 globalPosition = model * vec4(position, 1.0f);
  fragPosition = vec3(globalPosition);
  fragTexCoord = texCoord;
  fragNormal = mat3(normalMatrix) * normal;  
  gl_Position = projection * view * globalPosition;
}