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
out float depth;
 
// Values that stay constant for the whole mesh.
uniform mat4 MVP;
uniform vec3 lightPos;
uniform vec3 postionCam;
uniform mat4 ViewMatrix;
uniform float scale;
uniform float time;
uniform mat4 skyView;
float newtime = time *2;


float amplitudeB = 1;

 
 /*
 //old functions used for gerstner wave that are kept for legacy 
 //these were combinbed into the singular gersnter function
vec3 setupGertsnerWave(float wavelength,vec2 dir,float steepness)
{
    float k = 2 * 3.142 / wavelength;
    float c = sqrt(9.8 / k); //speed of wave, gravity in relation of wave length
    float f = k * (dot(dir,vec2(fragPos.x,fragPos.z) )- c * newtime); //for deriv
    float amp = steepness/k;
    vec3 value =  (vec3(f,amp,steepness));
    return value;

}


vec3 CalcGerstenerTangent(vec2 dir,float steepness,float f)
{
    return vec3(
    1 - dir.x *dir.x * (steepness * sin(f)),
    dir.x * (steepness * cos(f)), 
    -dir.x *dir.y * (steepness * sin(f)));
}

vec3 CalcGerstenerBin(vec2 dir,float steepness,float f)
{
 return vec3(
    -dir.x *dir.y * (steepness * sin(f)),
    dir.y * (steepness * cos(f)), 
    1 - dir.y *dir.y * (steepness * sin(f))) ;
}


vec3 calcGerstenerWave(vec3 fragPos,float f,float amp,vec2 dir) //addiative
{
   
    fragPos.x = dir.x* (amp* cos(f *frequency)); 
    fragPos.y=  amp * sin(f*frequency);
    fragPos.z = dir.y * (amp *cos(f*frequency));
    return fragPos;
}*/
float getHeight(vec2 localVertexUV)
{
    vec4 texel = texture(heightSampler, localVertexUV);
    float r = texel.r;
    float g = texel.g;
    float b = texel.b;
    float reconstructedValue = (r * 256.0 * 256.0) + (g * 256.0) + b;
    return (reconstructedValue /10000) * scale;
}
vec3 result;
vec3 tangent = vec3 (0,0,0);
vec3 bitangent = vec3 (0,0,0);  
vec3 normal;

void GerstnerWave(float aTime, float waveLength, float speed, float amplitude, float steepness, vec2 direction, in vec3 position, inout vec3 result, inout vec3 normal, inout vec3 tangent, inout vec3 bitangent)
{

    float wavelength = 2.0 * 3.1416 / waveLength; //prevent wave from overlapping
    float waveAMP = wavelength * amplitude;
    vec2 Direction = normalize(direction); 
    vec2 waveDir = Direction  * wavelength; 

    float S = speed * 0.5; 
    float waveFreq = S * wavelength; 
    float waveTime = waveFreq * aTime;

    //wDOTp - how much does the wave direction match the current postion then add time to make it move

    float wDotp = dot(waveDir, position.xz) + waveTime; 
    float SinOffset = sin(wDotp);
    float CosOffset = cos(wDotp);

    vec2 xz = position.xz - Direction  * steepness * amplitude * SinOffset ;

    float y = amplitude * CosOffset ;
  
    // bitangent
    vec3 B = vec3(
        1 - (steepness * Direction .x * Direction .x * waveAMP  * CosOffset),
        Direction .x * waveAMP  * SinOffset,
        -(steepness * Direction .x * Direction .y * waveAMP  * CosOffset));
    // tangent
    vec3 T = vec3(
        -(steepness * Direction .x * Direction .y * waveAMP  * CosOffset),
        Direction .y * waveAMP  * SinOffset,
        1 - (steepness * Direction .y * Direction .y * waveAMP  * CosOffset)
        );

    B = normalize(B);
    T = normalize(T);
    vec3 N = cross(T, B);

  
    result.xz +=  (xz - result.xz) * amplitudeB ;
    result.y += y * amplitudeB ;
    N.y = 0;
    N.xz =(normal.xz -  N.xz );
    normal +=  N ;
    
 
	tangent += T ;
    bitangent += B ;

}

void main()
{
    
    UV = vertexUV;
    camPostion = postionCam;
    vec3 newPos = vertexPosition_ocs;
    fragPos = newPos;
 
   
    vec2 dir= vec2(0,1);
    dir = normalize(dir);


    int waveNumber =1;


    
    vec3 result = vec3(0,0,0);
     vec3 tangent = vec3 (0,0,0);
    vec3 bitangent = vec3 (0,0,0);  
    vec3 normal= vec3(0,1,0);
    // (TIME,WAVELENGTH, SPEED,AMP,STEEPNES,DIRECTION)
    GerstnerWave(newtime,10,1,0.1,1,dir,fragPos,result,normal,tangent,bitangent);
     dir= vec2(-1,0);
   GerstnerWave(newtime,20,1,0.1,1,dir,fragPos,result,normal,tangent,bitangent);
   dir= vec2(-1,-1);
    GerstnerWave(newtime,2,0.02,0.2,0.5,dir,fragPos,result,normal,tangent,bitangent);
    dir= vec2(-0.5,-0.5);
   GerstnerWave(newtime,1,1,0.2,0.3,dir,fragPos,result,normal,tangent,bitangent);


    
   
   vec2 newUV = vertexUV;
    

    fragPos = result;
    /*

    // Calculate the normal by taking the cross product of tangent vectors
    for (int i = 0; i < waveNumber; ++i)
    {
        vec4 input = waveInput[i];
        dir = normalize(vec2(input.z,input.w));
        vec3 values = setupGertsnerWave(input.y, dir,input.x);
        tangent += (CalcGerstenerTangent(dir,input.x,values.x));
        binomial += (CalcGerstenerBin(dir,input.x,values.x));
        fragPos += amplitude *calcGerstenerWave(fragPos ,values.x,values.y,dir)*0.5 ;
        newUV += amplitude *calcGerstenerWave(fragPos ,values.x,values.y,dir).xz *0.005;

        frequency *= lacunarity;
	    amplitude *= gain;

        

    }*/
   
    

    //second wave

    float pixelHeight = 0;
    float weight = 1;
    //float newpixelHeight = getHeight(newUV);
    //iterate over blur samples
    float npoints = 0.01;
    float samples =10;
    float newpixelHeight = getHeight(newUV)/samples;
    for (int i = 1; i< samples;i++)
    {
	 pixelHeight += getHeight(newUV + vec2(npoints,npoints)); //upper row
	 pixelHeight += getHeight(newUV + vec2(0.0,npoints));
	 pixelHeight += getHeight(newUV+ vec2(-npoints,npoints));
 
	 pixelHeight += getHeight(newUV + vec2(npoints,0.0)); //middle row
	 //pixelHeight += getHeight(newUV + vec2(0.0,0.0));
	 pixelHeight += getHeight(newUV + vec2(-npoints,0.0));
 
	 pixelHeight += getHeight(newUV + vec2(npoints,-npoints)); //lower row
	 pixelHeight += getHeight(newUV + vec2(0.0,-npoints));
	 pixelHeight += getHeight(newUV + vec2(-npoints,-npoints));

     npoints =  npoints + 0.001;
     weight = weight + 8;
     newpixelHeight += (pixelHeight / 8)/samples;
     pixelHeight = 0;
    //divide the sum of values by the amount of samples

     }


    

    


    
    Normal = normal;
    result.y = result.y*0.1 + 1;
    depth = result.y - newpixelHeight;
  
    
    gl_Position = MVP * vec4(result, 1.0);
}


/*
    vec3 newPos = vertexPosition_ocs;

    // Adjust the parameters to control the wave movement
    float waveHeight = 0.4 * scale;
    float waveFrequency = 0.5;
    float waveSpeed = 0.2;

    // Calculate wave motion in the Y direction based on time
    newPos.y = waveHeight + (sin(newPos.x * waveFrequency + time * waveSpeed) + cos(newPos.z * waveFrequency + time * waveSpeed))/15;
    float randomOffsetX = rand(vertexPosition_ocs.yz);
    float randomOffsetZ = rand(vertexPosition_ocs.xy);

    newPos.y = waveHeight + (sin((newPos.x + randomOffsetX) * waveFrequency + time * waveSpeed) +
                            cos((newPos.z + randomOffsetZ) * waveFrequency + time * waveSpeed)) /10;

    newPos.y = 0.3 * scale + newwaveHeight(newPos.x,newPos.z)/20;

    newPos.y = 1.3 * scale;

    fragPos = vec3(skyView * vec4(newPos, 1));
    gl_Position = MVP * vec4(newPos, 1);
    fragPos = gl_Position.xyz;
    UV = vertexUV;
    

    lightDir = vec4(lightPos,0);*/