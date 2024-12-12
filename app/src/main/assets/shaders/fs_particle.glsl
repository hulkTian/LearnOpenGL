#version 300 es
precision mediump float;

out vec4 FragColor;

in vec2 TexCoords;
in vec4 ParticleColor;

uniform sampler2D sprite;

void main()
{
    FragColor = (texture(sprite, TexCoords) * ParticleColor);
}