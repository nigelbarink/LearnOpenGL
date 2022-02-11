#version 460 core
out vec4 FragColor;

in vec4 ourPosition;

in vec3 ourColor;
    
void main()
{
    FragColor = ourPosition;// vec4(ourColor, 1.0f);
}