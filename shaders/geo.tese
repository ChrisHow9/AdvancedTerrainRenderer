#version 420 core

layout(triangles,  fractional_odd_spacing, ccw) in;
layout (binding=1) uniform sampler2D heightSampler;

  // the view matrix
uniform mat4 MVP;    
uniform float pointsnum; 
uniform float scale;
// the projection matrix

// received from Tessellation Control Shader - all texture coordinates for the patch vertices
in vec3 WorldPos_ES_in[];
in vec2 UV_ES_in[];
in float tessLevel[];

out vec2 UV_FS_in;

out vec3 WorldPos_FS_in;
out vec3 Normal;
out vec3 fragPos;

// send to Fragment Shader for coloring
out float Height;


float getHeight(vec2 localVertexUV)
{
    vec4 texel = texture(heightSampler, localVertexUV);
    float r = texel.r;
    float g = texel.g;
    float b = texel.b;
    float reconstructedValue = (r * 256.0 * 256.0) + (g * 256.0) + b;
    return (reconstructedValue /10000) * scale;
}

vec3 interpolate3D(vec3 v0, vec3 v1, vec3 v2)
{
    return vec3(gl_TessCoord.x) * v0 + vec3(gl_TessCoord.y) * v1 + vec3(gl_TessCoord.z) * v2;
}

vec2 interpolate2D(vec2 v0, vec2 v1, vec2 v2)
{
    return vec2(gl_TessCoord.x) * v0 + vec2(gl_TessCoord.y) * v1 + vec2(gl_TessCoord.z) * v2;
}

void main()
{

   WorldPos_FS_in = interpolate3D(WorldPos_ES_in[0], WorldPos_ES_in[1], WorldPos_ES_in[2]);

   vec2 uv = interpolate2D(UV_ES_in[0],UV_ES_in[1],UV_ES_in[2]);

   	float npoints =pointsnum * tessLevel[0]; //points in mesh
	npoints = 1/npoints;


 
	float uR = getHeight(uv + vec2(npoints,npoints)); //upper row
	float uM = getHeight(uv + vec2(0.0,npoints));
	float uL = getHeight(uv + vec2(-npoints,npoints));
 
	float mR = getHeight(uv + vec2(npoints,0.0)); //middle row
	float mM = getHeight(uv + vec2(0.0,0.0));
	float mL = getHeight(uv + vec2(-npoints,0.0));
 
	float lR = getHeight(uv + vec2(npoints,-npoints)); //lower row
	float lM = getHeight(uv + vec2(0.0,-npoints));
	float lL = getHeight(uv + vec2(-npoints,-npoints));
 
 
	
	float nx =((uL - uR)+(mL - mR)+(lL - lR))/3;
	float ny = 10 * npoints;
	float nz =((uL - lL)+(uM - lM)+(uR - lR))/3;
	Normal = normalize( vec3(nx,ny,nz));
	


   UV_FS_in = uv;
   WorldPos_FS_in.y = getHeight(uv);

   fragPos = WorldPos_FS_in;

    gl_Position =  vec4(WorldPos_FS_in,1);
}