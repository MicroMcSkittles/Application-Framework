#version 460 core
#extension GL_ARB_bindless_texture: require
layout (location = 0) out vec4 FragColor;

// struct Matierial {
//     int AmbientMap_ID;
//     int DiffuseMap_ID;
//     int SpecularMap_ID;
//     int SpecularExponentMap_ID;
//     int AlphaMap_ID;
// };

// layout(std430, binding = 4) readonly buffer MatierialBuffer {
//     Matierial Matierials[];
// };
// layout(std430, binding = 5) readonly buffer TextureBuffer {
//     uvec2 Textures[];
// };

in vec2 TexCoord;
in flat int MatID;

//float Alpha = texture(sampler2D(Textures[Matierials[MatID].AlphaMap_ID]), TexCoord).r;

void main() {
    
    // TODO: Make this not suck later
    //if(Alpha == 0) {
    //    discard;
    //}

    //FragColor = vec4(color, 1);
}