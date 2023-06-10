#version 430 core

out vec4 color;

in mat4 fragView;
in mat4 fragProj;

in vec3 fragLineColor;
in vec2 viewportDim;
in vec4 worldPos;

in float fMaxRenderDistance;


void main()
{
	vec2 p = worldPos.xz;
	float d = length(p) / fMaxRenderDistance;
	d = clamp(d, 0.0, 1.0);
    color = mix(vec4(fragLineColor, 1.0), vec4(0.0), d);
}
