#version 460 core

layout(location = 0) in vec3 vertices;
layout(location = 1) in vec3 position;
layout(location = 2) in vec2 textureInfo;
layout(location = 3) in vec2 billboardSize;

layout (location = 1) uniform mat4 projection;
layout (location = 2) uniform mat4 view;
layout (location = 3) uniform vec3 cameraRight;
layout (location = 4) uniform vec3 cameraUp;

const vec2 uv_coords[4] = vec2[4](
    vec2(0, 0), vec2(0, 1),
    vec2(1, 1), vec2(1, 0)
);

const int uv_indices[6] = int[6](
    0, 1, 2, 3, 0, 2
);

out vec2 uv;
out vec2 texture_info;

void main(){
    vec3 vertexPosition = position
    + -cameraRight * vertices.x * billboardSize.x +
    + cameraUp * vertices.y * billboardSize.y +
    vec3(0.5, billboardSize.y/2.0, 0.5);

    uv = uv_coords[uv_indices[gl_VertexID%4]];
    texture_info = textureInfo;
    gl_Position = projection * view * vec4(vertexPosition,1.0);
}

