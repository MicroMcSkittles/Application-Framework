#version 450 core
layout (location = 0) out vec4 FragColor;

in vec2 TexCoord;

uniform vec2 Resolution;
uniform sampler2D FrameTexture;

//float contrast = 9999999;

void main() {
	vec3 v = texture(FrameTexture, TexCoord).rgb;
	//float gamma = 2.2;
	FragColor = vec4(v, 1);//vec4(pow(((vec3(v) - 0.5) * (1.0 + contrast) + 0.5), vec3(gamma)), 1).arbg;//vec4(pow(v, vec3(gamma)), 1);
}