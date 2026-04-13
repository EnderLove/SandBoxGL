#version 330 core 
out vec4 FragColor;

//in vec3 ourColor;
in vec2 texCoord;
in vec3 fragPos;
in vec3 normal;
in vec3 lightPos;

uniform vec3 objectColor;
uniform vec3 lightColor;
//uniform vec3 lightPos;
uniform vec3 viewPos; // I don't need view pos here since it will be treated like 0.0

uniform sampler2D texture1; // Here we pass the texture
uniform sampler2D texture2;
uniform float alphaBlend;

void main(){
    float ambientStrength  = 0.1;
    float specularStrength = 2.0;

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);

    vec3 ambient = ambientStrength * lightColor;
    
    float diff = max(dot(lightDir, norm), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 viewDir = normalize(-fragPos); // viewPos - fragPos
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64.0);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;

    //FragColor = mix(texture(texture1, texCoord), texture(texture2, texCoord), alphaBlend) * vec4(result, 1.0);
    //FragColor = vec4(0.2, 0.2, 0.2, 1.0) * vec4(result, 1.0);
    FragColor = texture(texture1, texCoord) * vec4(result, 1.0);
}


