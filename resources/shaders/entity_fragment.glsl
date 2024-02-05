#version 460 core

out vec4 fragColor;

in vec2 uv;     

uniform sampler2D tex;
layout (location = 6) uniform int textureIndex;
layout (location = 7) uniform int flipped;

float textureSizeY = 1.0;
float textureSizeX = 1.0/5.0;

void main(){
    vec2 texCoords = vec2(textureSizeX * textureIndex + textureSizeX * fract(flipped*uv.x), 
        textureSizeY * 3 + textureSizeY * fract(uv.y));
    vec4 final_color = texture(tex, texCoords);
    
    if(final_color.a < 0.1) discard;
    fragColor = final_color;
}