#version 460 core


out vec4 fragColor;


uniform sampler2D tex;

in vec2 uv;
//flat in float face_id;
//flat in float voxel_id;



void main(){
    vec4 final_color = texture(tex, uv);
    if(final_color.a < 0.1) discard;
    fragColor = final_color;
}