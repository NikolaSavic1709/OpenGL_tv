#version 330 core 

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec4 inCol;
out vec4 chCol;
uniform float pRotation;

uniform mat4 uM;
uniform mat4 uV;
uniform mat4 uP;

void main()
{
    float x = inPos.x * cos(pRotation) - (inPos.y-0.5) * sin(pRotation);
    float y = inPos.x * sin(pRotation) + (inPos.y-0.5) * cos(pRotation)+0.5;

    gl_Position = uP * uV * uM * vec4(x, y, inPos.z, 1.0);
	chCol = inCol;
}