#version 410 core
out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

struct Material{

    sampler2D diffuse;
    sampler2D specular;

    float shininess;
};

struct DirLight{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

uniform Material material;
//uniform DirLight dirLight;
uniform vec3 objectColor;
uniform PointLight pointLight;
//uniform vec3 lightColor;
//uniform vec3 lightPos;
uniform vec3 viewPos;


void main()
{
    float distance = length(pointLight.position - FragPos);
    float attenuation = 1.0/(pointLight.constant + pointLight.linear * distance + pointLight.quadratic * (distance * distance));
// ambient
    vec3 ambient = pointLight.ambient * vec3(texture(material.diffuse, TexCoords));
// diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(pointLight.position - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = pointLight.diffuse * (diff) * vec3(texture(material.diffuse, TexCoords));
// specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = (vec3(texture(material.specular, TexCoords)) * spec) * pointLight.specular;


    vec3 result = ambient + diffuse + specular * attenuation;
    FragColor = vec4(result, 1.0);
}