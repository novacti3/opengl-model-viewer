#version 420 core

out vec4 o_FragColor;

const float AMBIENT_LIGHT_STRENGTH = 0.1;

uniform vec4 u_Color = vec4(1.0, 0.5, 0.31, 1.0);
uniform vec4 u_LightColor = vec4(1.0);

void main()
{
    vec4 ambientLight = u_LightColor * AMBIENT_LIGHT_STRENGTH;
    
    o_FragColor = u_Color * ambientLight;
}