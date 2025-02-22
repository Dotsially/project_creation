#version 460 core

layout(location = 0) in vec3 vertices;

layout (location = 0) uniform mat4 projection;

const vec2 uv_coords[4] = vec2[4](
    vec2(0, 0), vec2(0, 1),
    vec2(1, 1), vec2(1, 0)
);

const int uv_indices[6] = int[6](
    0, 1, 2, 3, 0, 2
);

out vec2 uv;
out vec2 pos;

void main(){
    uv = uv_coords[uv_indices[gl_VertexID%4]];
    pos = vec2(vertices.xy);
    gl_Position = projection * vec4(vertices,1.0);
}

