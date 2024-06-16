#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in float aMatID;

uniform mat4 ViewProjection;
uniform mat4 Model;

out vec3 WorldPosition;
out vec3 WorldNormal;
out vec2 TexCoord;
out flat int MatID;

void main() {
    gl_Position = ViewProjection * Model * vec4(aPos, 1.0f);
    WorldPosition = vec3(Model * vec4(aPos, 1.0f));
    WorldNormal = aNormal;
    TexCoord = aTexCoord;
    MatID = int(aMatID);
}