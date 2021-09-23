#version 330 core

layout(location = 0) in vec3 a_VertPos;

uniform mat4 u_MVP = mat4(1.0);

void main()
{
    gl_Position = u_MVP * vec4(a_VertPos, 1.0);
}