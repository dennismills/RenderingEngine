#version 430 core

out vec4 color;

in vec2 uv;
in float lifetimeInS;


uniform sampler2D textureSampler;

void main()
{
	if (lifetimeInS <= 0)
	{
		discard;
	}
	else
	{
		color = texture2D(textureSampler, uv);
	}
}