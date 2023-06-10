#version 430 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;


uniform mat4 model;
uniform mat4 view;
uniform mat4 invView;
uniform mat4 projection;
uniform float hasTexture;
uniform float maxRenderDistance;

uniform vec3 modelAmbient;
uniform vec3 modelDiffuse;
uniform vec3 modelSpecular;

uniform vec3 cameraPos;

uniform float modelShininess;

out vec2 fragUV;
out vec3 surfaceNormal;
out vec3 toCameraVector;
out vec4 fragWorldPosition;
out float fragHasTexture;

out float fMaxRenderDistance;

out vec3 fragModelAmbient;
out vec3 fragModelDiffuse;
out vec3 fragModelSpecular;

out vec3 fragCameraPos;

out float fragModelShininess;

void main()
{
    vec4 worldPosition = model * vec4(pos, 1.0);
    fragWorldPosition = worldPosition;
    surfaceNormal = (model * vec4(normal, 0.0)).xyz;
    toCameraVector = (invView * vec4(0.0, 0.0, 0.0, 1.0)).xyz - worldPosition.xyz;
    fragUV = uv;
    fragHasTexture = hasTexture;

    fragModelAmbient = modelAmbient;
    fragModelDiffuse = modelDiffuse;
    fragModelSpecular = modelSpecular;
    fragModelShininess = modelShininess;

    fragCameraPos = cameraPos;

    fMaxRenderDistance = maxRenderDistance;

    gl_Position = projection * view * worldPosition;
}
