#version 300 es
precision mediump float;

out vec4 FragColor;
in  vec2 TexCoords;
uniform sampler2D fboAttachment;

uniform bool isDepth;

void main()
{
    if(isDepth) {
        FragColor = vec4(vec3(texture(fboAttachment, TexCoords).r), 1.0f);
        //FragColor = vec4(1.0f);
    } else {
        FragColor = texture(fboAttachment, TexCoords);
    }
}