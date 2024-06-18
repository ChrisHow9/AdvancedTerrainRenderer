#version 420 core
out vec4 FragColor;
layout (binding=12) uniform sampler2D grassTexture;
layout (binding=13) uniform sampler2D flowerTexture;

layout (binding=5) uniform sampler2D GrassSampler;

in vec2 TexCoord;
in float height;
in vec2 UV;
in vec3 NormalOut;
in float randValue;

in vec3 fragPos;
uniform vec3 lightPos;
uniform mat4 MVP;
in vec3 camPostion;
uniform float scale;
vec3 gramSchmidt(vec3 a, vec3 b, vec3 c) {
    b = b - dot(b, a) * a;
    c = c - dot(c, a) * a - dot(c, b) * b;
    return normalize(cross(b, c));
}
 
mat3 calculateTBN(vec3 normal, vec3 tangent, vec3 bitangent)
{
    return mat3(tangent, bitangent, normal);
}
float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}
void main()
{  
   vec3 Normal = NormalOut;
  
    vec4 Color;

    if (randValue < 0.80) {
        Color = texture(grassTexture, TexCoord);
    } else if (randValue < 1.4) {
        Color = texture(flowerTexture, TexCoord) ;
    } 
    if (Color.r >= 0.0 && Color.g >= 0.4 && Color.b >= 0.0) {
        discard;
    }
    vec4 Grass = texture(GrassSampler, UV);
    
    vec4 finalColor = mix(Grass, Color,TexCoord.y*2.5);
    finalColor = mix(vec4(0,0.2,0,0   ), Color,TexCoord.y*2.5);

    
  
     vec3 tangent = vec3(1, 0, 0);
    vec3 bitangent = vec3(0, 0, 1);
 
    
    tangent = tangent - dot(tangent, Normal) * Normal;
    bitangent = bitangent - dot(bitangent, Normal) * Normal;
    bitangent = bitangent - dot(bitangent, tangent) * tangent;
 

    mat3 TBN = calculateTBN(Normal, tangent, bitangent);
    
    
    vec3 viewDir = normalize(camPostion - fragPos);
    vec4 lightDir =  vec4(lightPos,0);
    
    vec3 lightDir3 =  normalize(lightPos.xyz);
    lightDir3 = normalize(TBN *lightDir3);
   
    
 
    
 

    vec3 specular =  vec3(0.01, 0.01, 0.01);
 
    float diffuse = max(dot(Normal, lightDir3), 0.0)+0.1;

    finalColor =  finalColor * diffuse  ;
    finalColor.w = 1;
    
    FragColor = finalColor ;

}

