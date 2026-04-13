#version 330

struct Light{ // Light behaivour
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct Material{
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};
uniform Material material;
uniform Light    light;

uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float time;

//uniform sampler2D texture1;

in vec3 fragPos;
in vec3 normal;
in vec2 texCoord;

out vec4 FragColor;

void main(){
    //vec3 ambient = light.ambient * material.ambient;
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoord)) * vec3(0.3); 

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * vec3(texture(material.diffuse, texCoord)));

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    //vec3 specular = light.specular * (spec * material.specular);
    
    //vec3 specularTexColor = vec3(texture(material.specular, texCoord));
    //vec3 specular = light.specular * (spec * vec3(1.0 - specularTexColor.r, 1.0 - specularTexColor.g, 1.0 - specularTexColor.b));
    vec3 specular = light.specular * (spec * vec3(texture(material.specular, texCoord)));

    float emissionIntensity = dot(norm, lightDir);
    vec3 emission;
    if (emissionIntensity < 0.0 && texture(material.specular, texCoord).r == 0.0){
        emission = vec3(1.0) * (-emissionIntensity * vec3(texture(material.emission, texCoord + vec2(0.0, time))));
    }else{
        emission = vec3(0.0); 
    }

    vec3 result = (ambient + diffuse + specular + emission);

    //FragColor = texture(texture1, texCoord) * vec4(result, 1.0); 
    FragColor = vec4(result, 1.0); 
}


