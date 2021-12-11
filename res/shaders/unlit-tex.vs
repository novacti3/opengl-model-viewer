#version 420 core

layout(location = 0) in vec3 a_VertPos;
layout(location = 1) in vec2 a_TexCoord;

uniform mat4 u_MVP = mat4(1.0);

out vec2 UV;

void main()
{
    gl_Position = u_MVP * vec4(a_VertPos, 1.0);
    UV = a_TexCoord;
}