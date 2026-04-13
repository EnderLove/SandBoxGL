#version 330 core 

out vec4 FragColor;

in vec2 texCoord;
in vec3 fragPos;
in vec3 normal;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform sampler2D floorTexture;

void main(){
    float ambientStrength = 1.0;
    float specularStrength = 1.0;

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);

    vec3 ambient = ambientStrength * lightColor;

    float diff = max(dot(lightDir, norm), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;

    //FragColor = texture(floorTexture, texCoord) * vec4(0.5f, 0.2f, 0.5f, 1.0f);
    FragColor = texture(floorTexture, texCoord) * vec4(result, 1.0);
}


