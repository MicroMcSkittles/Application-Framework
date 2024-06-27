#version 460 core
#extension GL_ARB_bindless_texture: require
layout (location = 0) out vec4 FragColor;

struct Matierial {
    int AmbientMap_ID;
    int DiffuseMap_ID;
    int SpecularMap_ID;
    int SpecularExponentMap_ID;
    int AlphaMap_ID;
};

struct PointLight {
    vec3 Position;
    float _PAD_1;
    vec3 Color;
    float _PAD_2;
    float Constant;
    float Linear;
    float Quadratic;
    float _PAD_3;
};
struct DirectionalLight {
    vec3 Direction;
    float _PAD_1;
    vec3 Color;
    float _PAD_2;
};

layout(std430, binding = 0) readonly buffer GeneralData {
    mat4 ViewProjection;

    vec3 CameraPosition;
};
layout(std430, binding = 1) readonly buffer PointLightData {
    PointLight PointLights[];
};
layout(std430, binding = 2) readonly buffer DirectionalLightData {
    DirectionalLight DirectionalLights[];
};
layout(std430, binding = 4) readonly buffer MatierialBuffer {
    Matierial Matierials[];
};
layout(std430, binding = 5) readonly buffer TextureBuffer {
    uvec2 Textures[];
};

in vec3 WorldPosition;
in vec3 WorldNormal;
in vec2 TexCoord;
in flat int MatID;

vec3 Ambient = texture(sampler2D(Textures[Matierials[MatID].AmbientMap_ID]), TexCoord).rgb;
vec3 Diffuse = texture(sampler2D(Textures[Matierials[MatID].DiffuseMap_ID]), TexCoord).rgb;
vec3 Specular = texture(sampler2D(Textures[Matierials[MatID].SpecularMap_ID]), TexCoord).rgb;
float SpecularExponent = texture(sampler2D(Textures[Matierials[MatID].SpecularExponentMap_ID]), TexCoord).r;
float Alpha = texture(sampler2D(Textures[Matierials[MatID].AlphaMap_ID]), TexCoord).r;

uniform sampler2D ShadowMap;

vec3 calcDirectionalLight(DirectionalLight light) {
    float ambientStrength = 0.3;
    vec3 ambientLighting = Ambient * light.Color * ambientStrength;

    vec3 normal = normalize(WorldNormal);
    vec3 lightDirection = normalize(-light.Direction);
    float diffuseStrength = max(dot(normal, lightDirection), 0.0);
    vec3 diffuseLighting = Diffuse * light.Color * diffuseStrength;

    vec3 viewDirection = normalize(CameraPosition - WorldPosition);
    vec3 reflectDirection = reflect(-lightDirection, normal); 
    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), SpecularExponent * 1000);
    vec3 specularLighting = light.Color * (spec * Specular);

    return ambientLighting + diffuseLighting + specularLighting;
}

vec3 calcPointLight(PointLight light) {
    float ambientStrength = 0.3;
    vec3 ambientLighting = Ambient * light.Color * ambientStrength;

    vec3 normal = normalize(WorldNormal);
    vec3 lightDirection = normalize(light.Position - WorldPosition);
    float diffuseStrength = max(dot(normal, lightDirection), 0.0);
    vec3 diffuseLighting = Diffuse * light.Color * diffuseStrength;

    vec3 viewDirection = normalize(CameraPosition - WorldPosition);
    vec3 reflectDirection = reflect(-lightDirection, normal); 
    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), SpecularExponent * 1000);
    vec3 specularLighting = light.Color * (spec * Specular);

    float lightDistance = length(light.Position - WorldPosition);
    float attenuation = 1.0 / (light.Constant + light.Linear * lightDistance + light.Quadratic * (lightDistance * lightDistance));

    return (ambientLighting + diffuseLighting + specularLighting) * attenuation;
}

void main() {
    vec3 color = vec3(0.0f);

    for(int i = 0; i < DirectionalLights.length(); i++){
        color += calcDirectionalLight(DirectionalLights[i]);
    }
    for(int i = 0; i < PointLights.length(); i++){
        color += calcPointLight(PointLights[i]);
    }

    // TODO: Make this not suck later
    if(Alpha == 0) {
        discard;
    }

    FragColor = vec4(color, 1);
}