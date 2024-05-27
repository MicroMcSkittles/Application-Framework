#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 ViewProjection;
uniform mat4 Model;



out DATA {
    vec3 Normal;
    vec2 TexCoord;
    mat4 ViewProjection;
    mat4 Model;
} data_out;

void main() {
    gl_Position = Model * vec4(aPos, 1.0f);
    data_out.Normal = aNormal;
    //data_out.Normal = vec3(-aNormal.x, aNormal.y, -aNormal.z);
    data_out.TexCoord = aTexCoord;
    data_out.ViewProjection = ViewProjection;
    data_out.Model = Model;
}