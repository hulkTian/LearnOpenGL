#version 300 es
// 深度计算必须用高精度！
precision highp float;

out vec4 FragColor;
in vec2 TexCoords;
uniform sampler2D texture1;

void main() {
    // 方案1：适配默认GL_LESS规则——设置更小的深度值（确保通过测试）
    // 微小偏移（0.001）避免Z-fighting，且不越界
    gl_FragDepth = clamp(gl_FragCoord.z - 0.01, 0.0, 1.0);

    // 方案2：若非要设置更大深度值（需配合应用层改深度规则为GL_GREATER）
    // gl_FragDepth = clamp(gl_FragCoord.z + 0.1, 0.0, 1.0);

    // 修正颜色通道：取正确的rgb通道
    FragColor = vec4(texture(texture1, TexCoords).rgb, 1.0);
}