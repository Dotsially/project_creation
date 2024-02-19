#version 460 core

out vec4 fragColor;

in vec2 uv;   
in vec2 texture_info;

uniform sampler2D tex;

float textureSizeY = 1.0;
float textureSizeX = 1.0/5.0;

void main(){
    vec2 texCoords = vec2(textureSizeX * texture_info.y + textureSizeX * fract(texture_info.x*uv.x), 
        textureSizeY * 3 + textureSizeY * fract(uv.y));
    vec4 final_color = texture(tex, texCoords);
    
    if(final_color.a < 0.1) discard;
    fragColor = final_color;
}