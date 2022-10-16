#version 460 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform vec3 cameraPos;
uniform samplerCube skybox;

in vec2 TexCoords;
uniform sampler2D texture_diffuse1;

void main()
{
    vec3 I = normalize(Position - cameraPos);
    vec3 R = reflect(I, normalize(Normal));

    FragColor =  vec4(texture(skybox,R).rgb, 1.0); // texture( texture_diffuse1, TexCoords) ;
}