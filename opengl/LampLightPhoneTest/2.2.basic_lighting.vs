#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;
out vec3 viewLightPos;
out vec3 GouraudLightColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{
	
	// view*model用了2次，不好
    FragPos = vec3(view*model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(view*model))) * aNormal;  
    
    gl_Position = projection * vec4(FragPos, 1.0);
	viewLightPos = vec3(view * vec4(lightPos, 1.0f));


	// ambient
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(viewLightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor*0.7;
    
	
    // specular
    float specularStrength = 1.0;
    vec3 viewDir = normalize( - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
    vec3 specular = specularStrength * spec * lightColor;  
        
    
    // FragColor = vec4(result, 1.0);
	GouraudLightColor = (ambient + diffuse + specular);

}