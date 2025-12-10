#version 300 es
precision mediump float;
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;

void main() {
    // 根据片段是否面向观察者设置颜色
    if (gl_FrontFacing) {
        FragColor = vec4(texture(texture1, TexCoords).rgb, 1.0); // 面向观察者的片段为绿色
    } else {
        FragColor = vec4(1.0, 0.0, 0.0, 1.0); // 背向观察者的片段为红色
    }
}