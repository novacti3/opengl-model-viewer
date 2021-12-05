#version 330 core

in vec2 UV;

out vec4 o_FragColor;

uniform float u_LerpStrength;
uniform sampler2D u_TexA;
uniform sampler2D u_TexB;
uniform sampler2D u_Mask;

void main()
{
    vec4 texA = texture(u_TexA, UV);
    vec4 texB = texture(u_TexB, UV);
    vec4 mask = texture(u_Mask, UV);

    vec4 texInMask = (texA - mask) + texB;

    float clampedLerpStrength = clamp(0, 1, u_LerpStrength);
    o_FragColor = mix(texA, texInMask, clampedLerpStrength);
}