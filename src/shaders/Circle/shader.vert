#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 col;

out vec3 vCol;

out vec2 vLocalPos;


void main()
{
    gl_Position = vec4(pos, 1.0);
    vCol = col;

    // aPos.xy should be in [-1, 1] for a unit quad
    vLocalPos = pos.xy;
}

