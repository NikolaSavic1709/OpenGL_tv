#version 330 core

in vec4 chCol;
out vec4 outCol;
uniform vec3 pCol;

void main()
{
	
	outCol = vec4(pCol, chCol.a);
}