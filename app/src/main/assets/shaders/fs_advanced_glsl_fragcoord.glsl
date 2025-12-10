#version 300 es
precision mediump float;
out vec4 FragColor;

uniform float width;
uniform float height;

void main() {
    // 根据片段的窗口坐标设置渐变色
    vec2 color = gl_FragCoord.xy / vec2(width, height);
    FragColor = vec4(color.x, color.y, 0.5, 1.0);
}