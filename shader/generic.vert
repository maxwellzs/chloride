#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

out vec2 texCoord;
out vec3 fragNormal;
out vec3 fragPos;

uniform mat4 actorTranslation[32];
uniform mat4 actorRotation[32];
uniform mat4 perspective;
uniform mat4 cameraPosition;
uniform mat4 cameraRotation;

void main()
{
    vec4 original = cameraRotation 
    * cameraPosition 
    * actorTranslation[gl_InstanceID] 
    * actorRotation[gl_InstanceID] 
    * vec4(position, 1.0);

    vec4 originalNormal = cameraPosition 
    * actorRotation[gl_InstanceID] 
    * vec4(normal, 1.0);

    fragNormal = vec3(originalNormal.xz, -originalNormal.y);
    fragPos = vec3(original.xz, -original.y);

    vec4 homo = perspective * vec4(original.xz, -original.y, 1.0);

    gl_Position = vec4(homo.xy/homo.w, -homo.z/homo.w, 1.0);
    texCoord = uv;
}