#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in float aMatID;

layout(std430, binding = 0) readonly buffer GeneralData {
    mat4 ViewProjection;
    mat4 LightSpaceMatrix;

    vec3 CameraPosition;
};
uniform mat4 Model;

out vec2 TexCoord;
out flat int MatID;

void main() {
    TexCoord = aTexCoord;
    MatID = int(aMatID);
    gl_Position = ViewProjection * Model * vec4(aPos, 1.0f);
}