#version 460 core
#extension GL_ARB_bindless_texture: require
layout (location = 0) out vec4 FragColor;

vec3 light_dir = vec3(1,1,1);

struct Matierial {
    int AmbientMap_ID;
    int DiffuseMap_ID;
    int SpecularMap_ID;
    int SpecularExponentMap_ID;
    int AlphaMap_ID;
};

layout(std430, binding = 1) readonly buffer MatierialBuffer {
    Matierial Matierials[];
};
layout(std430, binding = 2) readonly buffer TextureBuffer {
    uvec2 Textures[];
};

uniform vec3 CameraPosition; 

in vec3 WorldPosition;
in vec3 WorldNormal;
in vec2 TexCoord;
in flat int MatID;

vec3 lightColor = vec3(1, 1, 1);
vec3 lightSource = vec3(1, 1, -1);

void main() {
    vec3 Ambient = texture(sampler2D(Textures[Matierials[MatID].AmbientMap_ID]), TexCoord).rgb;
    vec3 Diffuse = texture(sampler2D(Textures[Matierials[MatID].DiffuseMap_ID]), TexCoord).rgb;
    vec3 Specular = texture(sampler2D(Textures[Matierials[MatID].SpecularMap_ID]), TexCoord).rgb;
    float SpecularExponent = texture(sampler2D(Textures[Matierials[MatID].SpecularExponentMap_ID]), TexCoord).r;
    float Alpha = texture(sampler2D(Textures[Matierials[MatID].AlphaMap_ID]), TexCoord).r;

    vec3 ambientLighting = Ambient * vec3(0.5);

    vec3 normal = normalize(WorldNormal);

    float diffuseStrength = max(0.0, dot(lightSource, normal));
    vec3 diffuseLighting = Diffuse * diffuseStrength * lightColor;

    vec3 viewSource = normalize(CameraPosition);
    vec3 reflectSource = normalize(reflect(-lightSource, normal));
    float specularStrength = max(0.0, dot(viewSource, reflectSource));
    specularStrength = pow(specularStrength, 32);
    vec3 specularLighting = Specular.r * specularStrength * lightColor;

    vec3 color = Diffuse * (ambientLighting + diffuseLighting + (specularLighting * SpecularExponent));

    // TODO: Make this not suck later
    if(Alpha == 0) {
        discard;
    }

    FragColor = vec4(color, 1);
}