#version 460 core

out vec4 fragColor;

in vec2 uv;   
in vec2 texture_info;
in float tile_type;

uniform sampler2D tex;
layout(location = 2) uniform vec3 colors[6];

float textureSize = 1.0/16.0;

void main(){
    vec2 texCoords = textureSize*texture_info + textureSize * fract(vec2(uv.x, -uv.y));    

    vec4 final_color = texture(tex, texCoords);
    float avg = (final_color.r + final_color.b + final_color.g) / 3.0;
    vec3 pallete = mix(colors[int(tile_type*2+1)], colors[int(tile_type*2)], avg*1.9);

    final_color = vec4(pallete, final_color.a);
    if(final_color.a < 0.1) discard;

    fragColor = final_color;
} 