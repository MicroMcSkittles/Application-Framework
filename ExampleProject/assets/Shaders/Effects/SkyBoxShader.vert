#version 450 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoord;

uniform mat4 View;
uniform mat4 Projection;

void main() {
    TexCoord = aPos;
    vec4 pos = Projection * View * vec4(aPos, 1.0f);
    gl_Position = pos.xyww;
}