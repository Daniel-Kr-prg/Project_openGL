#version 140

// Camera
uniform vec3 viewPosition;

// Material
uniform sampler2D texSampler;
uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;

// Ambient light
uniform float ambientLightIntensity;
uniform vec3 ambientLightColor;

// Directional light
uniform vec3 directionalLightDirection;
uniform float directionalLightIntensity;
uniform vec3 directionalLightColor;

in vec2 fragTexCoord;
in vec3 fragNormal;
in vec3 fragPosition;
out vec4 fragmentColor;

vec4 getDirectionalLightColor(vec3 normalizedNormal) {
  // Directional light
  // 
  vec3 lightDirection = normalize(directionalLightDirection);
  // Diffuse
  vec3 directionalDiffuse = directionalLightIntensity * max(dot(normalizedNormal, lightDirection), 0.0) * diffuse;
  // Specular
  vec3 viewDirection = normalize(viewPosition - fragPosition);
  vec3 reflectDirection = reflect(-lightDirection, normalizedNormal);
  vec3 directionalSpecular = directionalLightIntensity * pow(max(dot(viewDirection, reflectDirection), 0.0), shininess) * specular;
  return vec4(directionalDiffuse + directionalSpecular, 1.0);
}

void main() {
  vec3 normalizedNormal = normalize(fragNormal);
  
  // Ambient light
  vec3 ambientLight = ambientLightColor * ambientLightIntensity * ambient;
  
  fragmentColor = (getDirectionalLightColor(normalizedNormal) + vec4(ambientLight, 0.0)) * texture2D(texSampler, fragTexCoord);
}