#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  
in vec3 viewLightPos;  
in vec3 GouraudLightColor;
  
// uniform vec3 lightPos; 
// uniform vec3 viewPos; 
// uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{
    // // ambient
    // float ambientStrength = 0.3;
    // vec3 ambient = ambientStrength * lightColor;
  	
    // // diffuse 
    // vec3 norm = normalize(Normal);
    // vec3 lightDir = normalize(viewLightPos - FragPos);
    // float diff = max(dot(norm, lightDir), 0.0);
    // vec3 diffuse = diff * lightColor*0.7;
    
	
    // // specular
    // float specularStrength = 0.2;
    // vec3 viewDir = normalize( - FragPos);
    // vec3 reflectDir = reflect(-lightDir, norm);  
    // float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);
    // vec3 specular = specularStrength * spec * lightColor;  
        
    // vec3 result = (ambient + diffuse + specular) * objectColor;
    // // FragColor = vec4(result, 1.0);
	// FragColor = vec4(result, 1.0);
    vec3 result = GouraudLightColor * objectColor;
    FragColor = vec4(result, 1.0f);
} 