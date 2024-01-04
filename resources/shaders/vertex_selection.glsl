#version 460 core

in vec3 pos;
//in float voxel;

layout (location = 1) uniform mat4 projection;
layout (location = 2) uniform mat4 view;
layout (location = 0) uniform mat4 transform;

const vec2 uv_coords[4] = vec2[4](
    vec2(0, 0), vec2(0, 1),
    vec2(1, 1), vec2(1, 0)
);

const int uv_indices[24] = int[24](
    0, 1, 2, 3, 0, 2,  // tex coords indices for vertices of an even face
    2, 1, 0, 2, 0, 3,  // odd face
    3, 1, 0, 3, 0, 2,  // even flipped face
    1, 2, 3, 1, 0, 2   // odd flipped face
);


//out float voxel_id;
out vec2 uv; 

void main(){
    
    int uv_index = (gl_VertexID % 6);

    uv = uv_coords[uv_indices[uv_index]];
    //face_id = face;
    //voxel_id = voxel;

    gl_Position = projection * view * transform * vec4(pos, 1.0);
}