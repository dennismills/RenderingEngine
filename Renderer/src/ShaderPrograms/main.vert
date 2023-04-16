#version 330 core

#define MAX_LIGHTS 4

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;


uniform mat4 model;
uniform mat4 view;
uniform mat4 invView;
uniform mat4 projection;


uniform int lightCount;
uniform vec3 lightPositions[MAX_LIGHTS];
uniform vec3 lightColors[MAX_LIGHTS];
uniform vec3 lightFalloffs[MAX_LIGHTS];
uniform int lightTypes[MAX_LIGHTS];

flat out int numLights;
out vec3 surfaceNormal;
out vec3 toCameraVector;
out vec4 fragWorldPosition;

out vec3 toLightVector[MAX_LIGHTS];
out vec3 fragLightPositions[MAX_LIGHTS];
out vec3 fragLightColors[MAX_LIGHTS];
out vec3 fragLightFalloffs[MAX_LIGHTS];
flat out int fragLightTypes[MAX_LIGHTS];

void main()
{
    numLights = lightCount;
    vec4 worldPosition = model * vec4(pos, 1.0);
    fragWorldPosition = worldPosition;
    surfaceNormal = (model * vec4(normal, 0.0)).xyz;
    toCameraVector = (invView * vec4(0.0, 0.0, 0.0, 1.0)).xyz - worldPosition.xyz;
    
    for (int i = 0; i < lightCount; i++)
    {
        toLightVector[i] = lightPositions[i] - worldPosition.xyz;
        fragLightPositions[i] = lightPositions[i];
        fragLightColors[i] = lightColors[i];
        fragLightFalloffs[i] = lightFalloffs[i];
        fragLightTypes[i] =  lightTypes[i];
    }
    gl_Position = projection * view * worldPosition;
}