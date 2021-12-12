#version 420 core

layout(location = 0) in vec3 a_Pos;
layout(location = 2) in vec3 a_Normals;

uniform mat4 u_MVP = mat4(1.0);

void main()
{
    gl_Position = u_MVP * vec4(a_Pos, 1.0);
}