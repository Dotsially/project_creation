#version 460 core

out vec4 fragColor;

layout(binding = 0)uniform sampler2D noiseTexture;
layout(binding = 1)uniform sampler2D caustic;

layout(location = 1)uniform float zoomLevel;

in vec2 uv;
in vec2 pos;

float threshold = 0.45;
float range = 0.03;

float threshold2 = 0.35;
float range2 = 0.03;

void main(){
    vec4 noiseCol = texture(noiseTexture,uv);
    vec4 causticCol = texture(caustic, fract(uv)*25.0);
    float t = smoothstep(threshold - range, threshold + range, noiseCol.r);
    float r = smoothstep(threshold2 - range2, threshold2 + range2, noiseCol.r);
    vec4 final_color = mix(vec4(0.09, 0.302, 0.439, 1.0),mix(vec4(0.137, 0.42, 0.612,1.0),vec4(0.306, 0.588, 0.78, 1.0), t), r);

    float distance = distance(pos, vec2(290/2.0 , 270/2.0));
    if(zoomLevel < 6){
        final_color = (final_color + causticCol.a*0.05) * smoothstep(135, 132, distance);
    }
    else{
        final_color = (final_color) * smoothstep(135, 132, distance);
    }

    fragColor = final_color;
} 