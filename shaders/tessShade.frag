#version 420 core



uniform vec3 postionCam;
uniform vec3 lightPos;
uniform float pointsnum; 
uniform float scale;

layout (binding=0) uniform sampler2D DiffuseTextureSampler;
layout (binding=1) uniform sampler2D heightSampler;
layout (binding=2) uniform sampler2D RoughnessSampler;
layout (binding=3) uniform sampler2D NormalMapSampler;
layout (binding=4) uniform sampler2D SnowSampler;
layout (binding=5) uniform sampler2D GrassSampler;
layout (binding=6) uniform sampler2D GrassRoughSampler;
layout (binding=7) uniform sampler2D SnowRoughSampler;
layout (binding=8) uniform sampler2D GrassNormalSampler;
layout (binding=9) uniform sampler2D SnowNormalSampler;


in vec2 UVFragIN;
in vec3 Normal;
in vec3 fragPos;
in float tess;

out vec4 FragColor;

float getHeight(vec2 localVertexUV)
{
    vec4 texel = texture(heightSampler, localVertexUV);
    float r = texel.r;
    float g = texel.g;
    float b = texel.b;
    float reconstructedValue = (r * 256.0 * 256.0) + (g * 256.0) + b;
    return (reconstructedValue /10000) * scale;
}
mat3 calculateTBN(vec3 normal, vec3 tangent, vec3 bitangent)
{
    return mat3(tangent, bitangent, normal);
}
void main()
{
	
	//FragColor = vec4(Normal, 1.0);

   
    vec2 grassUV = tess*UVFragIN*pointsnum/4;
    vec3 diffuseColor = texture(DiffuseTextureSampler, grassUV).rgb;
    vec3 snowColor = texture(SnowSampler, grassUV).rgb;
    vec3 grassColor = texture(GrassSampler,  grassUV).rgb;
    float rockRoughness = texture(RoughnessSampler, grassUV).r;
    float grassRoughness = texture(GrassRoughSampler,  grassUV).r;
    float snowRoughness = texture(SnowRoughSampler, grassUV).r;
    vec3 normalMap = texture(NormalMapSampler, grassUV).rgb;
    vec3 normalMapSnow = texture(SnowNormalSampler, grassUV).rgb;
    vec3 normalMapGrass = texture(GrassNormalSampler,  grassUV).rgb;
 
    // Convert to [-1, 1] range
    normalMap = normalize(normalMap * 2.0 - 1.0);
    normalMapSnow = normalize(normalMapSnow * 2.0 - 1.0);
    normalMapGrass = normalize(normalMapGrass * 2.0 - 1.0);
 
 
    // Linearly blend between diffuse and snow textures
    vec3 resultColor = diffuseColor ;
    
    normalMap = normalMap;

    float a = smoothstep(-0.1, 0.5, getHeight(UVFragIN)-1.5);
    resultColor =mix(grassColor,diffuseColor,a) ;
    float roughness = mix(grassRoughness,rockRoughness,a);
    normalMap =mix(normalMapGrass ,normalMap ,a);

    a = smoothstep(-0.01, 0.3, getHeight(UVFragIN)-2.3);
    resultColor =mix(resultColor,snowColor,a) ;
    roughness = mix(roughness,snowRoughness,a);
    normalMap =mix(normalMap ,normalMapSnow ,a);

 
 
    
    float shininess = clamp((2 / (pow(roughness, 4) + 1e-2)) - 2, 0, 500.0f) ;
    vec3 viewDir = normalize(postionCam - fragPos);
 
    vec3 waterColor = vec3(0.0, 0.0, 1.0);

 
    // Tangent space basis vectors
    //conert to tangent space
    vec3 tangent = vec3(1, 0, 0);
    vec3 bitangent = vec3(0, 0, 1);
 
    vec3 normal = Normal;
    tangent = tangent - dot(tangent, normal) * normal;
    bitangent = bitangent - dot(bitangent, normal) * normal;
    bitangent = bitangent - dot(bitangent, tangent) * tangent;
 
    //normal = (gramSchmidt(tangent, bitangent, normalV));
    //tangent = (gramSchmidt(normalMap, bitangent, tangent));
    //bitangent = cross(normal,tangent);

 
    // Transform normal, tangent, and bitangent to world space
    mat3 TBN = calculateTBN(normal, tangent, bitangent);
    normalMap = normalize(TBN * normalMap);
    //normalMap = normalV;

 
    vec3 lightDir3 = normalize(lightPos);
    // Ensure the light direction is in the same tangent space
    lightDir3 = normalize(TBN * lightDir3);
 
    vec3 halfwayDir = normalize(lightDir3 + viewDir);
    vec3 reflectDir = reflect(-lightDir3, normalMap);
 
    float spec = pow(max(dot(normalMap, halfwayDir), 0.0), shininess);
    vec3 specular = spec * vec3(0.01, 0.01, 0.01);
 
    float diffuse = max(dot(normalMap, lightDir3), 0.0);
    //vec3 diffuse = diff * vec3(1.0, 1.0, 1.0);
    //vec3 color = texture(Die(SnowSffuseTextureSampler, UV).rgb * 0.5 + texture(SnowSampler, UV).rgb * 0.5 ;
    //vec3 result = (0.2 + diffuse + specular) * texturampler, UV).rgb;
 
 
 
    resultColor = max(0.1 + diffuse + specular , 0.0) * resultColor;

    

    
 
    // Output the final color
    FragColor =vec4( resultColor ,1);

}