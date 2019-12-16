#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;    
    float shininess;
}; 

struct DirLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
    // float constant;
    // float linear;
    // float quadratic;
};

struct PointLight {
    vec3 position;  
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    vec3 position;  
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
    float constant;
    float linear;
    float quadratic;
};

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;
  
# define NR_POINT_LIGHTS 4

uniform vec3 viewPos;
uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;

// function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{    
    // properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    // == =====================================================
    // Our lighting is set up in 3 phases: directional, point lights and an optional flashlight
    // For each phase, a calculate function is defined that calculates the corresponding color
    // per lamp. In the main() function we take all the calculated colors and sum them up for
    // this fragment's final color.
    // == =====================================================
    // phase 1: directional lighting
    vec3 result = vec3(0.0f,0.0f,0.0) ;
	result += CalcDirLight(dirLight, norm, viewDir);
    // phase 2: point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
       result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);    
    // phase 3: spot light
    result += CalcSpotLight(spotLight, norm, FragPos, viewDir);    
    
	
    FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir){
	vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

	vec3 lightDir = normalize(-light.direction);
	vec3 unitNormal = normalize(normal);
	float diff = max(dot(unitNormal, lightDir), 0.0f);
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
	
	vec3 reflectDir = reflect(-lightDir, unitNormal);  
	float spec = pow(max(dot(reflectDir, viewDir), 0.0f), material.shininess);
	vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;
	// return vec3(1.0f,0.0f,0.0f);
	return ambient + diffuse + specular;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

	vec3 lightDir = normalize(light.position - fragPos);
	vec3 unitNormal = normalize(normal);
	float diff = max(dot(unitNormal, lightDir), 0.0f);
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
	
	vec3 reflectDir = reflect(-lightDir, unitNormal);  
	float spec = pow(max(dot(reflectDir, viewDir), 0.0f), material.shininess);
	vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

	// attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0f / (light.constant + light.linear*distance + light.quadratic*distance*distance);

	diffuse *= attenuation;
	specular *= attenuation;

	return ambient + diffuse + specular;
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

	vec3 lightDir = normalize(light.position - fragPos);
	vec3 unitNormal = normalize(normal);
	float diff = max(dot(unitNormal, lightDir), 0.0f);
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
	
	vec3 reflectDir = reflect(-lightDir, unitNormal);  
	float spec = pow(max(dot(reflectDir, viewDir), 0.0f), material.shininess);
	vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;
	
	// spotLight
	float theta = dot(-lightDir, normalize(light.direction));
	float total = light.cutOff - light.outerCutOff;
	float intensity = clamp( (theta - light.outerCutOff)/total, 0.0f, 1.0f);

	diffuse *= intensity;
	specular *= intensity;

	// attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0f / (light.constant + light.linear*distance + light.quadratic*distance*distance);

	diffuse *= attenuation;
	specular *= attenuation;

	return ambient + diffuse + specular;
}