#version 460 core

out vec4 fragColor;

in vec2 uv;

uniform sampler2D tex;


void main(){
    vec4 final_color = texture(tex, uv);
    if(final_color.a < 0.1) discard;
    fragColor = final_color;
}