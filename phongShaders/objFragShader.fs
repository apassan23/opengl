#version 330 core
in vec3 norm;
in vec3 fragPos;
in vec2 texCoord;

out vec4 FragColor;

uniform vec3 viewPos;

struct Material{
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

uniform Material material;

struct Light{
    vec3 position;
    vec3 direction;
    // shading parameters
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    // spotlight
    float cutOff;
    float outerCutOff;

    // attentuation parameters
    float constant;
    float linear;
    float quadratic;
};

uniform Light light;

void main()
{

    vec3 lightDir = normalize(light.position - fragPos);
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.cutOff) / epsilon, 0.0, 1.0);
    
    vec3 ambient = light.ambient * (texture(material.diffuse,texCoord).rgb);

    lightDir = normalize(-light.direction);
    float diff = max(dot(lightDir, norm), 0.0);
    vec3 diffuse = light.diffuse * diff * (texture(material.diffuse,texCoord).rgb);

    vec3 reflectDir = reflect(-lightDir,norm);
    vec3 viewDir = normalize(viewPos - fragPos);
    float spec = pow(max(dot(reflectDir,viewDir), 0.0), material.shininess);
    vec3 specMap = texture(material.specular, texCoord).rgb;

    if(specMap == vec3(0.0)){
        specMap = vec3(0.2);
    }
    vec3 specular = (specMap * spec) * light.specular;

    float dist = length(light.position - fragPos);
    float attentuation = 1.0 / (light.constant + (light.linear * dist) + (light.quadratic * dist * dist));

    ambient *= attentuation;
    diffuse *= attentuation * intensity;
    specular *= attentuation * intensity;

    FragColor = vec4((ambient + diffuse + specular), 1.0);
}