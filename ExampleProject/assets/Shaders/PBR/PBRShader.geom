#version 450 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

out vec3 Position;
out vec3 Normal;
out vec2 TexCoord;

in DATA {
    vec3 Normal;
    vec2 TexCoord;
    mat4 ViewProjection;
    mat4 Model;
} data_in[];

void main() {
    // vec3 edge0 = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
    // vec3 edge1 = gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz;
    // vec2 deltaUV0 = data_in[1].TexCoord - data_in[0].TexCoord;
    // vec2 deltaUV1 = data_in[2].TexCoord - data_in[0].TexCoord;

    // float invDet = 1.0 / (deltaUV0.x * deltaUV1.y - deltaUV1.x * deltaUV0.y);

    // vec3 tanget = vec3(invDet * (deltaUV1.y * edge0 - deltaUV0.y * edge1));
    // vec3 bitangent = vec3(invDet * (-deltaUV1.x * edge0 + deltaUV0.x * edge1));

    // vec3 T = normalize(vec3(data_in[0].Model * vec4(tanget, 0.0)));
    // vec3 B = normalize(vec3(data_in[0].Model * vec4(bitangent, 0.0)));
    // vec3 N = normalize(vec3(data_in[0].Model * vec4(cross(edge1, edge0), 0.0)));

    // mat3 TBN = mat3(T, B, N);
    // TBN = inverse(TBN);

    gl_Position = data_in[0].ViewProjection * gl_in[0].gl_Position;
    Position = gl_in[0].gl_Position.xyz;
    Normal = data_in[0].Normal;
    TexCoord = data_in[0].TexCoord;
    EmitVertex();

    gl_Position = data_in[1].ViewProjection * gl_in[1].gl_Position;
    Position = gl_in[1].gl_Position.xyz;
    Normal = data_in[1].Normal;
    TexCoord = data_in[1].TexCoord;
    EmitVertex();

    gl_Position = data_in[2].ViewProjection * gl_in[2].gl_Position;
    Position = gl_in[2].gl_Position.xyz;
    Normal = data_in[2].Normal;
    TexCoord = data_in[2].TexCoord;
    EmitVertex();

    EndPrimitive();
}

// void main() {
//     vec3 vector0 = vec3(gl_in[2].gl_Position - gl_in[1].gl_Position);
//     vec3 vector1 = vec3(gl_in[0].gl_Position - gl_in[1].gl_Position);
//     vec4 surfaceNormal = vec4(normalize(cross(vector0, vector1)), 0.0);

//     gl_Position = data_in[0].ViewProjection * (gl_in[0].gl_Position + surfaceNormal);
//     Position = vec3(gl_in[0].gl_Position);
//     Normal = data_in[0].Normal;
//     TexCoord = data_in[0].TexCoord;
//     EmitVertex();

//     gl_Position = data_in[1].ViewProjection * (gl_in[1].gl_Position + surfaceNormal);
//     Position = vec3(gl_in[1].gl_Position);
//     Normal = data_in[1].Normal;
//     TexCoord = data_in[1].TexCoord;
//     EmitVertex();

//     gl_Position = data_in[2].ViewProjection * (gl_in[2].gl_Position + surfaceNormal);
//     Position = vec3(gl_in[2].gl_Position);
//     Normal = data_in[2].Normal;
//     TexCoord = data_in[2].TexCoord;
//     EmitVertex();

//     EndPrimitive();
// }