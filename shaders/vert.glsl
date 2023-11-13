#version 430 core

uniform mat4 projectionViewMatrix;

layout(location = 0) in vec3 position;   
layout(location = 1) in vec2 texCoords; 
layout(location = 2) in float modelMatrixIndex;

layout (std430, binding=2) buffer shader_data
{ 
  mat4 modelMatrix[];
};

out vec2 TexCoord;

void main() 
{
    //TODO: at some point later, condense the mvp matrix into the scene graph update.

    gl_Position = projectionViewMatrix * modelMatrix[int(modelMatrixIndex)] * vec4(position, 1.0);

    TexCoord = texCoords;
}
