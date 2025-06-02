#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

out vec2 texCoord;

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

    vec4 homo = perspective * vec4(original.y, original.z, -original.x, 1.0);

    gl_Position = vec4(homo.xyz/homo.w, 1.0);
    texCoord = uv;
}