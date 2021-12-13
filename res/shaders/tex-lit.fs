#version 420 core

out vec4 o_FragColor;

in vec3 o_FragPos;
in vec2 o_UV;
in vec3 o_Normal;

const float AMBIENT_LIGHT_STRENGTH = 0.1;
const float SPECULAR_STRENGTH = 0.5;

uniform sampler2D u_Tex;
uniform vec3 u_ViewPos = vec3(0.0);
uniform vec4 u_Color = vec4(1.0);
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

    // Calculate specular light
    vec3 viewDir = normalize(u_ViewPos - o_FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec4 specularLight = spec * u_LightColor * SPECULAR_STRENGTH;

    // Final output
    o_FragColor = texture(u_Tex, o_UV) * u_Color * (ambientLight + diffuseLight + specularLight);
}