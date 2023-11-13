//The version of OpenGL we are using.
#version 430 core

//These layouts rperesent the layout of our Vertex class
//The locations are determined by glAttribPointer settings in our OpenGL context.
layout(location = 0) in vec3 position;   
layout(location = 1) in vec2 texCoords; 
layout(location = 2) in float mvpMatrixIndex;

//This layout represents the SSBO binding. It contains an arbitrary length array of model-view-projection matrices.
layout (std430, binding=2) buffer mvp_data
{ 
  mat4 mvpMatrix[];
};

//This is the vertex shader output. We pass through our texture coordinates that we received on the VBO to the fragment shader.
out vec2 TexCoord;

void main() 
{
    //Update the position of the vertex by multiplying it by the correct model-view-projection matrix from the SSBO.
    //We transform position into a vec4 from a vec3
    //As per the layout, position is (x,y,z) coordinates (see Vertex struct) that comes from VerticesComponent
    //mvpMatrix is a glm::mat4 that comes from MVPTransformComponent

    gl_Position = mvpMatrix[int(mvpMatrixIndex)] * vec4(position, 1.0);

    //Pass the texture coordinates through to the frag shader

    TexCoord = texCoords;
}
