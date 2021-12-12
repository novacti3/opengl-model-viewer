#version 420 core

out vec4 o_FragColor;

in vec3 o_FragPos;
in vec3 o_Normal;

const float AMBIENT_LIGHT_STRENGTH = 0.1;

uniform vec4 u_Color = vec4(1.0, 0.5, 0.31, 1.0);
uniform vec3 u_LightPos = vec3(1.2, 1.0, 2.0);
uniform vec4 u_LightColor = vec4(1.0);

void main()
{
    // Ambient light
    vec4 ambientLight = u_LightColor * AMBIENT_LIGHT_STRENGTH;
    
    // Calculate diffuse light
    vec3 normal = normalize(o_Normal);
    vec3 lightDir = normalize(u_LightPos - o_FragPos);

    float diffuseImpact = max(dot(normal, lightDir), 0.0);
    vec4 diffuseLight = u_LightColor * diffuseImpact;

    o_FragColor = u_Color * (ambientLight + diffuseLight);
}