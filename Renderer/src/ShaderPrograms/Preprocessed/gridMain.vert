#version 430 core

layout (location = 0) in vec3 pos;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;
uniform vec3 lineColor;

uniform float maxRenderDistance;

in vec2 viewport;
out vec2 viewportDim;

out float fMaxRenderDistance;
out vec3 fragLineColor;
out vec4 worldPos;

void main()
{
    viewportDim = viewport;
    worldPos = vec4(pos, 1.0);

    fragLineColor = lineColor;
    fMaxRenderDistance = maxRenderDistance;
    gl_Position = proj * view * model * vec4(pos.xyz, 1.0);
}
