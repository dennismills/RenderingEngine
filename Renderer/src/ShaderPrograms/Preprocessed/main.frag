#version 330 core
#define MAX_LIGHTS 4

#define DIRECTIONAL 0
#define POINT 1
#define SPOTLIGHT 2

out vec4 color;
flat in int numLights;
in vec3 surfaceNormal;
in vec3 toCameraVector;
in vec4 fragWorldPosition;

in vec3 toLightVector[MAX_LIGHTS];
in vec3 fragLightPositions[MAX_LIGHTS];
in vec3 fragLightColors[MAX_LIGHTS];
in vec3 fragLightFalloffs[MAX_LIGHTS];
flat in int fragLightTypes[MAX_LIGHTS];


vec3 computeDiffuseLighting(vec3 surfaceNormal, vec3 toLightVector, vec3 lightColor)
{
    vec3 unitNormal = normalize(surfaceNormal);
    vec3 unitToLightVector = normalize(toLightVector);
    float nDot = dot(unitNormal, unitToLightVector);
    float brightness = max(nDot, 0.0);

    return brightness * lightColor;
}

vec3 computeSpecularLighting(vec3 surfaceNormal, vec3 toLightVector, vec3 toCameraVector, vec3 lightColor)
{
    vec3 unitNormal = normalize(surfaceNormal);
    vec3 unitToCameraVector = normalize(toCameraVector);
    vec3 unitToLightVector = normalize(toLightVector);
    vec3 lightDirection = -unitToLightVector;
    vec3 reflectedLightDirection = reflect(lightDirection, unitNormal);
    float specularFactor = dot(reflectedLightDirection, unitToCameraVector);
    specularFactor = max(specularFactor, 0.0);
    float dampedFactor = pow(specularFactor, 32);

    return dampedFactor * lightColor;
}

float computeFalloff(vec3 lightPosition, vec3 falloffComponents)
{
    float dist = length(lightPosition - fragWorldPosition.xyz);
    return 1.0 / (falloffComponents.x + (falloffComponents.y * dist) + (falloffComponents.z * dist * dist));
}

void main()
{
    vec3 directionalTotalColor = vec3(0.0, 0.0, 0.0);
    vec3 pointTotalColor = vec3(0.0, 0.0, 0.0);

    for (int i = 0; i < numLights; i++)
    {
        if (fragLightTypes[i] == DIRECTIONAL)
        {
            directionalTotalColor += computeDiffuseLighting(surfaceNormal, toLightVector[i], fragLightColors[i]);
            directionalTotalColor += computeSpecularLighting(surfaceNormal,toLightVector[i], toCameraVector, fragLightColors[i]);
        }
        if (fragLightTypes[i] == POINT)
        {
            vec3 contribution = vec3(0.0, 0.0, 0.0);
            contribution += computeDiffuseLighting(surfaceNormal, toLightVector[i], fragLightColors[i]);
            contribution += computeSpecularLighting(surfaceNormal,toLightVector[i], toCameraVector, fragLightColors[i]);
            float falloff = computeFalloff(fragLightPositions[i], fragLightFalloffs[i]);
            contribution *= falloff;

            pointTotalColor += contribution;
        }
    }
    color = vec4(directionalTotalColor + pointTotalColor, 1.0);
}
