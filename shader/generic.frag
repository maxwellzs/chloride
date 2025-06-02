#version 330

in vec2 texCoord;
in vec3 fragNormal;
in vec3 fragPos;
out vec4 fragColor;

uniform sampler2D fragTexture;

void main() {
    vec3 lightColor = vec3(1, 1, 1);
    vec3 lightPos = vec3(3, 3, 0);

    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(fragNormal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float specularStrength = 0.5;
    vec3 viewPos = vec3(0,0,0);
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 textureColor =  texture(fragTexture, texCoord).xyz;
    fragColor = vec4((ambient + diffuse + specular) * textureColor , 1.0);
}