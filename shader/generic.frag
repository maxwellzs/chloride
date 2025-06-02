#version 330

in vec2 texCoord;
out vec4 fragColor;

uniform sampler2D fragTexture;

void main() {
    fragColor = texture(fragTexture, texCoord);
}