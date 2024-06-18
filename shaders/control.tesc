#version 410 core
//https://ogldev.org/www/tutorial30/tutorial30.html - tesselation code learned from this tutorial

// define the number of CPs in the output patch
layout (vertices = 3) out;

uniform vec3 postionCam;

// attributes of the input CPs
in vec3 WorldControl[];
in vec2 UVControl[];


// attributes of the output CPs
out vec3 WorldPosEval[];
out vec2 UVEval[];
out float tessLevel[];


float distanceForTess(float dist1, float dist2)
{
    float dis = (dist1 + dist2) / 2.0; 


    if (dis <= 1) {
        return 32.0;
    }
    else if (dis <= 2) {
        return 16.0;
    }
    else if (dis <= 5) {
        return 8.0;
    }

    else if (dis<= 10) {
        return 4.0;
    }
    else if (dis <= 25) {
        return 2.0;
    }
    else {
        return 1.0;
    }

}

float distanceForTess2(float dist1)
{
    float dis = dist1;


    if (dis <= 1) {
        return 32.0;
    }
    else if (dis <= 2) {
        return 16.0;
    }
    else if (dis <= 5) {
        return 8.0;
    }

    else if (dis<= 10) {
        return 4.0;
    }
    else if (dis <= 25) {
        return 2.0;
    }
    else {
        return 1.0;
    }

}
void main()
{
    // Set the control points of the output patch

   WorldPosEval[gl_InvocationID] = WorldControl[gl_InvocationID];
    UVEval[gl_InvocationID] = UVControl[gl_InvocationID];


    tessLevel[gl_InvocationID] = distanceForTess2(distance(postionCam, WorldControl[gl_InvocationID]));;


        // Calculate the distance from the camera to the three control points
    float Distance0 = distance(postionCam,WorldPosEval[0]);
    float Distance1 = distance(postionCam, WorldPosEval[1]);
    float Distance2 = distance(postionCam, WorldPosEval[2]); 

    // Calculate the tessellation levels
    gl_TessLevelOuter[0] =distanceForTess(Distance1, Distance2);
    gl_TessLevelOuter[1] =distanceForTess(Distance2, Distance0);
    gl_TessLevelOuter[2] =distanceForTess(Distance0, Distance1);
    gl_TessLevelInner[0] =distanceForTess(Distance0, Distance1);
    gl_TessLevelInner[1] =distanceForTess(Distance0, Distance1);
}