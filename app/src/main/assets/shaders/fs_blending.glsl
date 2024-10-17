#version 300 es
precision mediump float;

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;

void main()
{
    /*vec4 texColor = texture(texture1, TexCoords);
    // 直接丢弃透明片段
    if(texColor.a < 0.1)
    discard;
    FragColor = texColor;*/
    // 完全使用纹理的所有颜色通道
    FragColor = texture(texture1, TexCoords);
}