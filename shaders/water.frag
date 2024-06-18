
#version 420 core
out vec4 FragColor;
layout (binding=11) uniform sampler2D DiffuseTextureSampler;

in vec2 UV;
in vec3 fragPos;
in vec4 lightDir;
in vec3 camPostion;
in vec3 rVector;
in vec3 Normal;
in vec3 base;
in float depth;

uniform samplerCube skybox;

void main()
{   

    vec3 viewDir = normalize(camPostion - fragPos);
    
    vec3 lightDir3 = normalize(lightDir.xyz);
    // Ensure the light direction is in the same tangent space
    lightDir3 = normalize( lightDir3);
 
    vec3 halfwayDir = normalize(lightDir3 + viewDir);
    vec3 reflectDir = reflect(-lightDir3,Normal);
    float shininess = 10;
    float spec = pow(max(dot(Normal, halfwayDir), 0.0), shininess);
    vec3 specular = spec * vec3(0.01, 0.01, 0.01);
 
    float diffuse = max(dot(Normal, lightDir3), 0.0);
    vec2 newUV = UV;

    vec3 ambient = vec3(0,0.8,1) * 0.2;
    vec3 specVal = vec3(0,0.8,1)* specular;
    vec3 diffuseVal = vec3(0,0.8,1) * diffuse;
    vec3 diffuseColor = ambient+ specVal+ diffuse;
    
    vec3 I = normalize(fragPos - camPostion );
    vec3 R = reflect(I, Normal);
    vec4 reflectionValue = vec4(texture(skybox, R).rgb, 1.0);
    vec3 flatnormal = vec3(0,1,0);

    float fresnel = dot(viewDir, flatnormal);
    reflectionValue.w = 1 ;



    float transparency = clamp(1.0 - depth, 0.0, 0.0);
    vec4 output = mix(reflectionValue,vec4(diffuseColor, depth*1.8 ) ,fresnel);

    float threshold = 0.5;
    transparency = mix(1.0, transparency, step(threshold, depth*0.8));

    vec4 foam = vec4((texture(DiffuseTextureSampler,UV*5).rgb),1) * 1.3;
    vec4 out2 = vec4(Normal,1);
   
    //vec4 output = mix(reflectionValue,vec4(diffuseColor,1) ,fresnel);
    // mix(output, (transparency * foam), 0.2)
    FragColor = mix(output, (transparency * foam), 0.2);
}

