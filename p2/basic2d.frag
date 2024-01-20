#version 330 core

in vec2 chTex; //koordinate teksture
out vec4 outCol;

uniform sampler2D uTex;
//uniform sampler2D uTex2;//teksturna jedinica

void main()
{
    if (texture(uTex, chTex).a < 0.1) {
        discard;
    }
    outCol = texture(uTex, chTex); //boja na koordinatama chTex teksture vezane na teksturnoj jedinici uTex
}