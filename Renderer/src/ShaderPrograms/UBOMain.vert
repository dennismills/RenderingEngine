#version 430 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 invView;
uniform mat4 projection;

out vec3 surfaceNormal;
out vec3 toCameraVector;
out vec4 fragWorldPosition;

void main()
{
    vec4 worldPosition = model * vec4(pos, 1.0);
    fragWorldPosition = worldPosition;
    surfaceNormal = (model * vec4(normal, 0.0)).xyz;
    toCameraVector = (invView * vec4(0.0, 0.0, 0.0, 1.0)).xyz - worldPosition.xyz;
    gl_Position = projection * view * worldPosition;
}