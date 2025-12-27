#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 textureCoord;

uniform mat4 transform;
out vec2 texCoord;

void main()
{
	gl_Position = transform * vec4(pos, 1.0f);
	texCoord = vec2(texCoord.x, texCoord.y);
}

