#version 460 core

layout(location = 0) in uint packedPosUV;
layout(location = 1) in float face; 

layout (location = 1) uniform mat4 projection;
layout (location = 2) uniform mat4 view;
layout (location = 0) uniform mat4 transform;

const vec2 uv_coords[4] = vec2[4](
    vec2(0.0, 0.0), vec2(0.0, 1.0),
    vec2(1.0, 1.0), vec2(1.0, 0.0)
);

const int uv_indices[24] = int[24](
    0, 1, 2, 3, 0, 2,  // tex coords indices for vertices of an even face
    2, 1, 0, 2, 0, 3,  // odd face
    3, 1, 0, 3, 0, 2,  // even flipped face
    1, 2, 3, 1, 0, 2   // odd flipped face
);

const float multiplier[3] = float[3](
    1.0, 0.8, 0.5
);

out vec3 world_pos;
out vec2 uv; 
out vec2 voxel_id;
out float shading;

void main(){
    int uv_index = (gl_VertexID % 4);
    
    vec3 vertexPosition = 
        vec3((packedPosUV & 63), 
            ((packedPosUV >> 6) & 63), 
            ((packedPosUV >> 12) & 63));

    voxel_id = vec2((packedPosUV >> 18) & 127, (packedPosUV >> 25));

    uv = uv_coords[uv_indices[uv_index]];

    if(face < 4){
        shading = multiplier[1];
    }
    else if(face == 4){
        shading = multiplier[0];
    }
    else{
        shading = multiplier[2];
    }

    gl_Position = projection * view * transform * vec4(vertexPosition, 1.0);
    world_pos = (transform * vec4(vertexPosition, 1.0)).xyz;
}