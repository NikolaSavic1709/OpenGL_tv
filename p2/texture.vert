#version 330 core 

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec2 inTex;
layout(location = 2) in vec3 inNormal;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 uM;
uniform mat4 uV;
uniform mat4 uP;

void main()
{
    FragPos = vec3(uM * vec4(inPos.x, inPos.y, inPos.z, 1.0));
    Normal = mat3(transpose(inverse(uM))) * inNormal;  
    TexCoords = inTex;
	gl_Position =  uP * uV * vec4(FragPos, 1.0);
    
}
