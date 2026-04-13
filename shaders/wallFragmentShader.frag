#version 330 core

uniform sampler2D wallTexture;

in vec4 vertexColor;
in vec2 texCoord;
out vec4 FragColor;

void main(){
    FragColor = texture(wallTexture, texCoord) * vertexColor;
}


