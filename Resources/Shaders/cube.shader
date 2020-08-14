#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoord;

void main(){
	FragPos = vec3(model * position);
	Normal = mat3(transpose(inverse(model))) * normal;
	TexCoord = texCoord;
	gl_Position = projection * view * model * position;
};

#shader fragment
#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

out vec4 FragColor;

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	//sampler2D emission;

	float shininess;
};

struct Light {
	vec3 position;
	
	//vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main(){
	vec3 norm = normalize(Normal);
	//vec3 lightDir = normalize(light.position - FragPos);
	vec3 lightDir = normalize(-light.direction);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse,TexCoord)) ;

	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));

	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0),material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular,TexCoord));

	//vec3 emission = vec3(texture(material.emission, TexCoord));

	//vec3 result = (ambient + diffuse + specular + emission);
	vec3 result = (ambient + diffuse + specular);
	FragColor = vec4(result, 1.0);
};