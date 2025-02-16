#version 300 es
precision mediump float;

out vec4 color;

in vec2 TexCoords;

uniform sampler2D scene;
uniform vec2      offsets[9];
uniform int       edge_kernel[9];
uniform float     blur_kernel[9];

uniform bool chaos;
uniform bool confuse;
uniform bool shake;

void main()
{
    color = vec4(0.0f);
    vec3 samples[9];
    // 如果使用卷积矩阵，则对纹理的偏移像素进行采样
    if (chaos || shake) {
        for (int i = 0; i < 9; i++){
            samples[i] = vec3(texture(scene, TexCoords.st + offsets[i]));
        }
    }
    // 处理特效
    if (chaos) {
        for (int i = 0; i < 9; i++) {
            color += vec4(samples[i] * float(edge_kernel[i]), 0.0f);
            color.a = 1.0f;
        }
    } else if (confuse) {
        color = vec4(1.0 - texture(scene, TexCoords).rgb, 1.0);
    } else if (shake) {
        for(int i = 0; i < 9; i++) {
            color += vec4(samples[i] * blur_kernel[i], 0.0f);
            color.a = 1.0f;
        }
    } else {
        color = texture(scene, TexCoords);
    }
}