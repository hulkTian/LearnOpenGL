#version 300 es
precision mediump float;

out vec4 fragColor;

in vec2 TexCoords;

uniform vec3 textColor;
uniform sampler2D text;

void main() {
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
    fragColor = vec4(textColor, 1.0) * sampled;
}
