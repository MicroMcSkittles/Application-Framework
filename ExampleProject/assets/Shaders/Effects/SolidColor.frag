#version 460 core
layout (location = 0) out vec4 FragColor;

in vec3 WorldPosition;
in vec3 WorldNormal;
in vec2 TexCoord;
in flat int MatID;

void main() {
    FragColor = vec4(1, 1, 1, 1);
}