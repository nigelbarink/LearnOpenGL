#version 460 core
uniform vec3 outlineColor; 

out vec4 FragColor;


in vec2 texCoords;
uniform sampler2D texture_diffuse1;


void main(){
    FragColor = vec4(outlineColor, 1.0);
}