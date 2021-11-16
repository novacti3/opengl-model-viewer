#version 330 core

out vec4 o_FragColor;

uniform vec4 u_Color = vec4(1.0);

void main()
{
    o_FragColor = u_Color;
}