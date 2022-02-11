#version 460 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aColor;

uniform float HorizontalOffset;

out vec3 ourColor;
out vec4 ourPosition;

void main(){

    gl_Position = vec4(-aPos.x + HorizontalOffset, aPos.yz , 1.0);
    ourColor = aColor;
    ourPosition = gl_Position;
}
