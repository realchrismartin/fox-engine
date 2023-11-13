#version 430 core

layout(location = 0) in vec3 position;   
layout(location = 1) in vec2 texCoords; 
layout(location = 2) in float mvpMatrixIndex;

layout (std430, binding=2) buffer shader_data
{ 
  mat4 mvpMatrix[];
};

out vec2 TexCoord;

void main() 
{
    gl_Position = mvpMatrix[int(mvpMatrixIndex)] * vec4(position, 1.0);

    TexCoord = texCoords;
}
