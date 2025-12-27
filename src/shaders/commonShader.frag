#version 330 core
in vec2 texCoord;
out vec4 fragColor;

uniform vec3 uColor;
uniform sampler2D ourTexture;

void main() 
{
	fragColor = vec4(uColor, 1.0);
}

