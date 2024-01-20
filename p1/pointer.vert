#version 330 core 

layout(location = 0) in vec2 inPos;
layout(location = 1) in vec4 inCol;
out vec4 chCol;
uniform float pRotation;
uniform float resolution;
void main()
{
    float x = inPos.x * cos(pRotation) - inPos.y * sin(pRotation);
    float y = inPos.x * sin(pRotation) * resolution + inPos.y * cos(pRotation) * resolution;

    // Postavljanje nove pozicije
    gl_Position = vec4(x, y, 0.0, 1.0);
	chCol = inCol;
}