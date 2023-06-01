#version 430 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 uvCoords;
layout (location = 4) in vec4 pos;

uniform mat4 view;
uniform mat4 projection;

const float scale = 3.0;

out vec2 uv;
out float lifetimeInS;

void main()
{
	uv = uvCoords;
	lifetimeInS = pos.w;
	vec4 positionViewspace = view * vec4(pos.xyz, 1.0);
    positionViewspace.xy += (scale) * (vertexPosition.xy - vec2(0.5));
	gl_Position = projection * positionViewspace;
}
