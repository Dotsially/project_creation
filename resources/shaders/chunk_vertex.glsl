#version 460 core

layout(location = 0) in vec3 pos;
layout(location = 1) in float vertex;
layout(location = 2) in float face;
layout(location = 3) in vec2 voxel;

layout (location = 1) uniform mat4 projection;
layout (location = 2) uniform mat4 view;
layout (location = 0) uniform mat4 transform;

const vec2 uv_coords[4] = vec2[4](
    vec2(0, 0), vec2(0, 1),
    vec2(1, 1), vec2(1, 0)
);


const float multiplier[3] = float[3](
    1.0, 0.8, 0.5
);

out vec3 world_pos;
out vec2 uv; 
out vec2 voxel_id;
out float shading;

void main(){
    uv = uv_coords[int(vertex)];
    voxel_id = voxel;

    if(face < 4){
        shading = multiplier[1];
    }
    else if(face == 4){
        shading = multiplier[0];
    }
    else{
        shading = multiplier[2];
    }


    gl_Position = projection * view * transform * vec4(pos.x, pos.y, pos.z, 1.0);
    world_pos = (transform * vec4(pos-0.5, 1.0)).xyz;
}