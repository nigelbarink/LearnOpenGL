#version 460 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform vec3 cameraPos;
uniform samplerCube skybox;

in vec2 TexCoords;
uniform sampler2D texture_diffuse1;

const float air_ri = 1.00;
const float water_ri = 1.33;
const float Ice_ri = 1.309;
const float glass_ri = 1.52;
const float diamond_ri = 2.42;

void main()
{
    // Regular shading
    // texture( texture_diffuse1, TexCoords) ;


    // Reflective shading
    /*
    vec3 I = normalize(Position - cameraPos);
    vec3 R = reflect(I, normalize(Normal));

    FragColor =  vec4(texture(skybox,R).rgb, 1.0);
    */


    // Refractive shading
    float ratio = air_ri/water_ri;
    vec3 I = normalize(Position-cameraPos);
    vec3 R = refract(I, normalize(Normal), ratio);
    FragColor = vec4(texture(skybox, R).rgb, 1.0);

}