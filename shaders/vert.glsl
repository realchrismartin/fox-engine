#version 330 core

uniform mat4 modelViewProjectionMatrix;

layout(location = 0) in vec3 position;   
layout(location = 1) in vec2 texCoords; 

out vec2 TexCoord;

void main() 
{
    gl_Position = modelViewProjectionMatrix * vec4(position, 1.0);

    TexCoord = texCoords;
}
