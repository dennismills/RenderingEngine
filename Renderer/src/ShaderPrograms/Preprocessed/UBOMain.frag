#version 430 core
#define MAX_LIGHTS 128

#define DIRECTIONAL 0
#define POINT 1
#define SPOTLIGHT 2

struct Light
{
	vec4 position;
	vec4 color;
	vec4 falloff;
    vec4 direction;
    vec4 cutoff;
	vec4 type;
};

layout(std140, binding = 0) uniform LightsBlock {
  Light lights[MAX_LIGHTS];
};

in vec2 fragUV;
in vec3 surfaceNormal;
in vec3 toCameraVector;
in vec4 fragWorldPosition;

in float fragHasTexture;

uniform sampler2D textureData;

out vec4 color;

vec3 computeDiffuseLighting(Light l)
{
    vec3 unitNormal = normalize(surfaceNormal);
    vec3 lightDir = normalize(l.position.xyz - fragWorldPosition.xyz);
    float diff = max(dot(unitNormal, lightDir), 0.0);
    vec3 diffuse = (diff * l.color).xyz;
    return diffuse;
}

vec3 computeSpecularLighting(Light l)
{
    float specularStrength = 0.5;
    vec3 viewDir = normalize(-fragWorldPosition.xyz);
    vec3 lightDir = normalize(l.position.xyz - fragWorldPosition.xyz);
    vec3 unitNormal = normalize(surfaceNormal);
    vec3 reflectDir = reflect(-lightDir, unitNormal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = specularStrength * (spec * l.color).xyz;
    return specular;
}

float computeFalloff(Light l)
{
    float dist = length(l.position.xyz - fragWorldPosition.xyz);
    return 1.0 / (l.falloff.x + (l.falloff.y * dist) + (l.falloff.z * dist * dist));
}

void main()
{
    vec3 directionalColor = vec3(0.0);
    vec3 pointColor = vec3(0.0);
    vec3 spotColor = vec3(0.0);
    vec3 textureColor = vec3(0.0);

    for(int i = 0; i < MAX_LIGHTS; i++)
    {
        if(lights[i].color == vec4(0.0))
        {
            continue; // skip inactive lights
        }
        if (lights[i].type.x == DIRECTIONAL)
        {
            directionalColor += computeDiffuseLighting(lights[i]);
            directionalColor += computeSpecularLighting(lights[i]);
        }
        if (lights[i].type.x == POINT)
        {
            vec3 contribution = vec3(0.0);
            contribution += computeDiffuseLighting(lights[i]);
            contribution += computeSpecularLighting(lights[i]);

            float falloff = computeFalloff(lights[i]);
            contribution *= falloff;
            pointColor += contribution;
        }
        if (lights[i].type.x == SPOTLIGHT)
        {
            vec3 contribution = vec3(0.0);
            vec3 lightDir = normalize(lights[i].position.xyz - fragWorldPosition.xyz);
            float theta = dot(lightDir, normalize(-lights[i].direction.xyz));
            if(theta > lights[i].cutoff.x) 
            {       
                contribution += computeDiffuseLighting(lights[i]);
                contribution += computeSpecularLighting(lights[i]);

                float falloff = computeFalloff(lights[i]);
                contribution *= falloff;
                spotColor += contribution;
            }
        }
    }

    // Simulate bool since they don't exist
    if (fragHasTexture > 0.5)
    {
        textureColor = texture2D(textureData, fragUV).xyz;
    }
    color = vec4((directionalColor + pointColor + spotColor + textureColor), 1.0);
}
