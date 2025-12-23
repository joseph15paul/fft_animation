#version 330 core
out vec4 fragColor;

in vec3 vCol;
in vec2 vLocalPos;

uniform vec2  uCenter;
uniform float uRadius;
uniform float uThickness;   // ring thickness [0-1.0]

void main()
{
    float dist = distance(vLocalPos, uCenter);

    float halfThickness = uThickness * 0.5;

    float inner = uRadius - halfThickness;
    float outer = uRadius + halfThickness;

    float softness = max(fwidth(dist), 0.001);

    float alphaOuter = 1.0 - smoothstep(outer - softness, outer, dist);
    float alphaInner = smoothstep(inner, inner + softness, dist);

    float alpha = alphaOuter * alphaInner;

    if (alpha <= 0.0)
        discard;

    fragColor = vec4(vCol, alpha);
}

