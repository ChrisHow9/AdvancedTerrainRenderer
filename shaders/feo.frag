#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 4) out;
in vec3 postionCam;

out vec2 TexCoord;

void main() {  
    // Quad above Triangle 1
    vec4 newPos = (gl_in[0].gl_Position + gl_in[1].gl_Position + gl_in[2].gl_Position) / 3;

    vec4 toCamera = normalize(postionCam -vec3( newPos));
    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 right = cross(toCamera, up);

    newPos -= (right * 0.5);

    vec4 output;

    output = newPos;
    output.y = newPos.y + 0.4;
    output.x = newPos.x + 0.1;
    gl_Position = output;
    TexCoord = vec2(0.0, 1.0);
    EmitVertex();

    output = newPos;
    output.y = newPos.y + 0.4;
    output.z = newPos.z + 0.1;
    gl_Position = output;
    TexCoord = vec2(1.0, 1.0);

    EmitVertex();

    output = newPos;
    output.y = newPos.y + 0.01;
    output.x = newPos.x + 0.1;
    gl_Position = output;
    TexCoord = vec2(0.0, 0.0);
    EmitVertex();

    output = newPos;
    output.y = newPos.y + 0.01;
    output.z = newPos.z + 0.1;
    gl_Position = output;
    TexCoord = vec2(1.0, 0.0);
    EmitVertex();

    EndPrimitive();
}