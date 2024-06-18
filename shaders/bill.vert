#version 420 core
 
// Input vertex data, different for all executions of this shader.
 layout (binding=1) uniform sampler2D heightSampler;
layout(location = 0) in vec3 vertexPosition_ocs;
layout(location = 1) in vec2 vertexUV;

// Output data ; will be interpolated for each fragment.
out vec2 UV;
// Renamed to lightDir
out vec3 fragPos;
out vec4 lightDir;
out vec3 camPostion;// Renamed to lightDir
out vec3 Normal;
out vec3 base;
out vec2 VertexUV;
out float depth;
 
// Values that stay constant for the whole mesh.
uniform mat4 MVP;
uniform vec3 lightPos;

uniform mat4 ViewMatrix;
uniform float scale;
uniform float time;
uniform mat4 skyView;


float getHeight(vec2 localVertexUV)
{
    vec4 texel = texture(heightSampler, localVertexUV);
    float r = texel.r;
    float g = texel.g;
    float b = texel.b;
    float reconstructedValue = (r * 256.0 * 256.0) + (g * 256.0) + b;
    return (reconstructedValue /10000) * scale;
}

void main()
{
    float npoints = 1/200;
	float uR = getHeight(vertexUV + vec2(npoints,npoints)); //upper row
	float uM = getHeight(vertexUV + vec2(0.0,npoints));
	float uL = getHeight(vertexUV + vec2(-npoints,npoints));
 
	float mR = getHeight(vertexUV + vec2(npoints,0.0)); //middle row
	float mM = getHeight(vertexUV + vec2(0.0,0.0));
	float mL = getHeight(vertexUV + vec2(-npoints,0.0));
 
	float lR = getHeight(vertexUV + vec2(npoints,-npoints)); //lower row
	float lM = getHeight(vertexUV + vec2(0.0,-npoints));
	float lL = getHeight(vertexUV + vec2(-npoints,-npoints));
 
 
	
	float nx =((uL - uR)+(mL - mR)+(lL - lR))/3;
	float ny = npoints;
	float nz =((uL - lL)+(uM - lM)+(uR - lR))/3;
	vec3 normal = normalize( vec3(nx,ny,nz));
	Normal = normal;

    VertexUV = vertexUV*1;  
    vec3 newPos = vertexPosition_ocs;
    fragPos = newPos;
    vec2 newUV = vertexUV;
    fragPos.y  = getHeight(newUV);
    gl_Position = vec4(fragPos, 1.0);
}
