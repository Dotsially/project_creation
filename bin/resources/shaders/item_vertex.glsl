#version 460 core

layout(location = 0) in vec3 vertices;
layout(location = 1) in vec3 position;
layout(location = 2) in vec2 itemIndex;
layout(location = 3) in vec2 billboardSize;

layout (location = 1) uniform mat4 projection;
layout (location = 2) uniform mat4 view;
layout (location = 3) uniform vec3 cameraRight;
layout (location = 4) uniform vec3 cameraUp;

const vec2 uv_coords[4] = vec2[4](
    vec2(0, 0), vec2(0, 1),
    vec2(1, 1), vec2(1, 0)
);

out vec2 uv;
out vec2 item_id;

void main(){
    vec3 vertexPosition = position
    + -cameraRight * vertices.x * billboardSize.x +
    + cameraUp * vertices.y * billboardSize.y +
    vec3(-0.5, 0.5, 0.5);

    item_id = itemIndex;
    uv = uv_coords[gl_VertexID%4];
    gl_Position = projection * view * vec4(vertexPosition, 1.0);
}