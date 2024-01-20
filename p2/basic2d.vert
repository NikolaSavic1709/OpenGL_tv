#version 330 core 

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec2 inTex; //Koordinate texture, propustamo ih u FS kao boje
out vec2 chTex;
uniform vec2 uPosTop;

void main()
{
    gl_Position = vec4(vec3(inPos.x,inPos.y,inPos.z) + vec3(uPosTop, 0.0), 1.0);
    chTex = inTex;
}