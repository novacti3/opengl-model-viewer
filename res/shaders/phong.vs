#version 420 core

layout(location = 0) in vec3 a_Pos;
layout(location = 2) in vec3 a_Normal;

out vec3 o_FragPos;
out vec3 o_Normal;

uniform mat4 u_ModelMatrix = mat4(1.0);
uniform mat4 u_MVP = mat4(1.0);

void main()
{    
    gl_Position = u_MVP * vec4(a_Pos, 1.0);
    
    o_FragPos = vec3(u_ModelMatrix * vec4(a_Pos, 1.0));
    o_Normal = mat3(transpose(inverse(u_ModelMatrix))) * a_Normal;
}