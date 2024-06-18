
#version 410 core

layout (location = 0) in vec3 aPos;

layout(location = 1) in vec2 vertexUV;



uniform mat4 MVP;

out vec2 TexCoord;
out vec3  WorldControl;
out vec2 UVControl;

void main()
{
    WorldControl = aPos;
   UVControl = vertexUV;

    
}