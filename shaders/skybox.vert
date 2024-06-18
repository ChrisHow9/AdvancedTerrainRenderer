#version 330 core
layout (location = 0) in vec3 aPos; // the position variable has attribute position 0


out vec3 TexCoords;
; // specify a color output to the fragment shader
uniform mat4 MVP;
void main()
{
    gl_Position = MVP * vec4(aPos, 1.0);
    TexCoords = aPos;
}

