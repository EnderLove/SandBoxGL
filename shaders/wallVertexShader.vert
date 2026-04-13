#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexture;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec2 texCoord;
out vec4 vertexColor;

void main(){
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
    vertexColor = vec4(0.8, 0.2, 0.2, 1.0);
    texCoord = aTexture;
}


