#version 300 es
precision mediump float;

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D image;
uniform vec3 spriteColor;

void main()
{
    FragColor = vec4(spriteColor, 1.0f) * texture(image, TexCoords);
}