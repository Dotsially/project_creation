#version 460 core

out vec4 fragColor;

in vec2 uv;   
in vec2 texture_info;

uniform sampler2D tex;

float textureSize = 1.0/8.0;

void main(){
    vec2 texCoords = textureSize*texture_info + textureSize * fract(vec2(uv.x, -uv.y));    

    vec4 final_color = texture(tex, texCoords);
    if(final_color.a < 0.1) discard;

    fragColor = final_color;
}