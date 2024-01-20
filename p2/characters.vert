#version 330 core 

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec2 inTex; //Koordinate texture, propustamo ih u FS kao boje
out vec2 chTex;

uniform mat4 uM;
uniform mat4 uV;
uniform mat4 uP;
uniform float tMovement;

void main()
{
	gl_Position =  uP * uV * uM * vec4(inPos.x + tMovement, inPos.y, inPos.z, 1.0);
	chTex = inTex;
}