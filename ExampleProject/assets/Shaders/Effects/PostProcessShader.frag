#version 450 core
layout (location = 0) out vec4 FragColor;

in vec2 TexCoord;

uniform vec2 Resolution;
uniform sampler2D FrameTexture;

void main() {
	vec3 v = texture(FrameTexture, TexCoord).rgb;
	FragColor = vec4(v,1);
}