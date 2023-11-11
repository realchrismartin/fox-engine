#version 330 core

uniform mat4 modelViewProjectionMatrix;

layout(location = 0) in vec3 position;   
layout(location = 1) in vec2 texCoords; 
layout(location = 3) in float mvpIndex;

layout (std330, binding=2) buffer shader_data
{ 
  mat4 mvpMatrix[];
};

out vec2 TexCoord;

void main() 
{
    //gl_Position = modelViewProjectionMatrix * vec4(position, 1.0);

    gl_Position = shader_data.mvpMatrix[int(mvpIndex)] * vec4(position, 1.0);

    TexCoord = texCoords;
}
