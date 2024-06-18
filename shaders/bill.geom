#version 420 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 4) out;
in vec2 vertexUV[];
in vec4 lightDir[];
layout (binding=1) uniform sampler2D heightSampler;

out vec3 NormalOut;
out vec4 Light;
out float randValue;

uniform vec3 postionCam;
uniform mat4 MVP;
uniform mat4 ViewMatrix;
uniform mat4 projectionMatrix;
uniform float time;
uniform float scale;


out vec2 TexCoord;
out float height;
out vec3 fragPos;
out vec2 UV;

float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}
vec3 GetNormal()
{
   vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
   vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
   return normalize(cross(a, b));
}  

vec3 getRandomPointInTriangle(vec3 v0, vec3 v1, vec3 v2)
{
// use random values of barcyteic coorduiates
//interpolate the postion by these random barycentric coordinates
    float r1 = sqrt(rand(vec2(v0.x,v0.z)));
    float r2 = rand(vec2(v0.x,v0.z));
    float a = 1.0 - r1;
    float b = r1 * (1.0 - r2);
    float c = r2 * r1;


    return a * v0 + b * v1 + c * v2;
}

float getHeight(vec2 localVertexUV)
{
    vec4 texel = texture(heightSampler, localVertexUV);
    float r = texel.r;
    float g = texel.g;
    float b = texel.b;
    float reconstructedValue = (r * 256.0 * 256.0) + (g * 256.0) + b;
    return (reconstructedValue /10000) * scale;
}


void main() {  


	NormalOut = GetNormal();
    // Quad above Triangle 1
    vec3 newPos = getRandomPointInTriangle(gl_in[0].gl_Position.xyz, gl_in[1].gl_Position.xyz, gl_in[2].gl_Position.xyz);
    newPos.y += 0.02 ;
   // NormalOut =  vec3(0,1,0);
    fragPos = newPos;

    randValue = sin(rand(newPos.xz));


    vec3 camRightWS = vec3(ViewMatrix[0][0],ViewMatrix[1][0],ViewMatrix[2][0]);
    vec3 camUPWS = vec3(ViewMatrix[0][1],ViewMatrix[1][1], ViewMatrix[2][1]);
    float size = 0.2;


    vec3 toCamera = normalize(postionCam -vec3( newPos));
    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 right = cross(up, toCamera);

    if ( newPos.y < 0.2*scale+sin(rand(vec2(fragPos.y,0)))  && newPos.y >1.8 -sin(rand(vec2(fragPos.y,0))))
    {
        vec3 output =newPos;
        float wiggle = sin(time* rand(vec2(newPos.x,newPos.z)))*0.03  ;
        output.z = output.z+ wiggle ;


        gl_Position = MVP * vec4((newPos + 
        camRightWS * 0.1 * size +
        camUPWS * -0.1 * size) 
        +(wiggle,0,0),1) ;
        TexCoord = vec2(1.0, 0.0) ;
        height = newPos.y;
    
        Light = lightDir[0];
        EmitVertex();


        gl_Position = MVP * vec4(output + 
        camRightWS * 0.1 * size +
        camUPWS * 0.1 * size,1) ;
        TexCoord = vec2(1.0, 1.0);
        height = newPos.y;
    
        Light = lightDir[0];

    

        EmitVertex();


        gl_Position = MVP * vec4(newPos + 
        camRightWS * -0.1 * size +
        camUPWS * -0.1 * size,1) ;
        TexCoord = vec2(0.0, 0.0);
        height = newPos.y;
   
        Light = lightDir[0];
    
        EmitVertex();


        gl_Position =MVP * vec4(newPos + 
        camRightWS * -0.1 * size +
        camUPWS * 0.1 * size,1) ;
        TexCoord = vec2(0.0, 1.0);
        height = newPos.y;
    
        Light = lightDir[0];
    
        EmitVertex();
    }




    EndPrimitive();
}