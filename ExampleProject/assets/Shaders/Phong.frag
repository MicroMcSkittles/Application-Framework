#version 460 core
layout (location = 0) out vec4 FragColor;

vec3 light_dir = vec3(1,1,1);

struct Matierial {
    vec3 Albedo;
    float PAD;
};

layout(std140, binding = 1) readonly buffer MatierialBuffer {
    Matierial Matierials[];
};
uniform int MatierialCount;

in vec3 WorldPosition;
in vec3 WorldNormal;
in vec2 TexCoord;
in flat int MatID;

void main() {
    float l = (dot(light_dir, WorldNormal));

    vec3 albedo = Matierials[MatID].Albedo;
    vec3 color = albedo * l;

    FragColor = vec4(color, 1);
}