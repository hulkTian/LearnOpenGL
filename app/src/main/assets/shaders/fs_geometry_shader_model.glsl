#version 320 es
precision mediump float;

out vec4 FragColor;

in vec2 texCoords;

uniform sampler2D texture_diffuse1;

void main()
{
    FragColor = texture(texture_diffuse1, texCoords);
}