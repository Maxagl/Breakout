#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform vec3 spriteColor;

void main()
{    
    vec2 centerCoodinate = TexCoords.xy;
    float distance = length(centerCoodinate);
    color = vec4(spriteColor, 0.5 - distance);
}  