#version 460 core

out vec4 fragColor;


uniform sampler2D tex;
layout (location = 3) uniform vec3 cameraWorldPos;
layout (location = 4) uniform vec3 fogColor;

in vec2 uv;
in vec3 world_pos;
flat in vec2 voxel_id;
flat in float shading;

float fogEnd = 96;
float fogStart = 90;

float textureSize = 1.0/8.0;

float calculate_fog(){
    float cameraPixelDistance = length(world_pos - cameraWorldPos);
    float FogRange = fogEnd - fogStart;
    float FogDistance = fogEnd - cameraPixelDistance;
    float FogFactor = FogDistance / FogRange;
    FogFactor = clamp(FogFactor, 0, 1);
    return FogFactor;
}
void main(){
    float fogFactor = calculate_fog();
    vec2 texCoords = textureSize*voxel_id + textureSize * fract(uv);
    
    fragColor = mix(vec4(fogColor, 1.0), vec4(vec3(texture(tex, texCoords).rgb * shading),1.0), fogFactor);
    
}