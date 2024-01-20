#version 330 core

struct Light{ //Svjetlosni izvor
	vec3 pos; //Pozicija
	vec3 kA; //Ambijentalna komponenta (Indirektno svjetlo)
	vec3 kD; //Difuzna komponenta (Direktno svjetlo)
	vec3 kS; //Spekularna komponenta (Odsjaj)
};
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  
in vec2 TexCoords;
  
//uniform vec3 uLightPos; 
uniform vec3 uViewPos; 
//uniform vec3 uLightColor;

uniform Light uLight;

uniform sampler2D uDiffMap1;

void main()
{    
    float ambientStrength = 0.5;
    vec3 ambient = uLight.kA * ambientStrength;
  	
    // diffuse 
    vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(uLight.pos - FragPos);
	float nD = max(dot(normal, lightDirection), 0.0);
	vec3 diffuse = uLight.kD * nD;

    // specular
    float specularStrength = 0.2;
    vec3 viewDirection = normalize(uViewPos - FragPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float s = pow(max(dot(viewDirection, reflectionDirection), 0.0), 32);
	vec3 specular = uLight.kS * s * specularStrength;

    FragColor = texture(uDiffMap1, TexCoords) * vec4(ambient + diffuse + specular, 1.0);
}

