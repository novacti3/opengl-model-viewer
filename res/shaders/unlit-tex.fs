#version 420 core

in vec2 UV;

out vec4 o_FragColor;

uniform sampler2D u_Tex;

void main()
{
    o_FragColor = texture(u_Tex, UV);
}