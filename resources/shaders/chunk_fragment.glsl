#version 460 core


out vec4 fragColor;


uniform sampler2D tex;

in vec2 uv;
flat in vec2 voxel_id;
flat in float shading;


void main(){
    vec2 texCoords =  vec2(((1/10.0)*voxel_id.x) + uv.x/10.0, ((1/16.0)*voxel_id.y) + uv.y/16.0); 
    
    fragColor = vec4(vec3(texture(tex, texCoords).rgb * shading),1.0);

}