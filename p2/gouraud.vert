#version 330 core
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;

out vec3 LightingColor; // resulting color from lighting calculations

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

uniform mat4 uM;
uniform mat4 uV;
uniform mat4 uP;

uniform mat4 uButtonM;
uniform float screen;

void main()
{
    vec3 FragPos = vec3(uM * vec4(inPos, 1.0));
	gl_Position = uP * uV * vec4(FragPos, 1.0);

	if (uButtonM != mat4(0.0)) {
        FragPos = vec3(uButtonM * vec4(inPos, 1.0));
        gl_Position = uP * uV * vec4(FragPos, 1.0);
    }
    vec3 Position = vec3(uM * vec4(inPos, 1.0));
    vec3 Normal = mat3(transpose(inverse(uM))) * inNormal;
    
    // ambient
    float ambientStrength = 1.0;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - Position);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    if (screen!=1.0)
    {
        diffuse=vec3(0.0,0.0,0.0);
    }
    
    // specular
    float specularStrength = 1.0; // this is set higher to better show the effect of Gouraud shading 
    vec3 viewDir = normalize(viewPos - Position);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;      

    

    LightingColor = ambient + diffuse + specular;
}