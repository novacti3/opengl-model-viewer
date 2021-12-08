#version 420 core

in vec2 UV;

out vec4 o_FragColor;

uniform sampler2D u_TexA;
uniform sampler2D u_TexB;
uniform sampler2D u_Mask;

void main()
{
    vec4 texA = texture(u_TexA, UV);
    vec4 texB = texture(u_TexB, UV);
    vec4 mask = texture(u_Mask, UV);

    vec4 texInMask = texB * mask;
    vec4 texOutOfMask = texA * (1 - mask);

    o_FragColor = texOutOfMask + texInMask;
}