#version 330 core

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormal; //Normale

out vec3 FragPos; //Interpolirana pozicija fragmenta
out vec3 Normal; //Interpolirane normale

uniform mat4 uM;
uniform mat4 uV;
uniform mat4 uP;

uniform mat4 uButtonM;

void main()
{
	FragPos = vec3(uM * vec4(inPos, 1.0));
	gl_Position = uP * uV * vec4(FragPos, 1.0);

	if (uButtonM != mat4(0.0)) {
        FragPos = vec3(uButtonM * vec4(inPos, 1.0));
        gl_Position = uP * uV * vec4(FragPos, 1.0);
    }

	Normal = mat3(transpose(inverse(uM))) * inNormal; //Inverziju matrica bolje racunati na CPU
}