#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 2) in float aAlpha;

uniform mat4 mvp;
out float vAlpha;

void main()
{
	gl_Position = mvp * vec4(pos, 1.0f);
	vAlpha = aAlpha;
}

