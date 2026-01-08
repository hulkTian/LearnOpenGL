#version 320 es
precision mediump float;
in vec4 FragPos;

uniform vec3 lightPos;
uniform float far_plane;

void main() {
    // 获取片段到光源的距离
    float lightDistance = length(FragPos.xyz - lightPos);

    // 将距离映射到 [0.0, 1.0] 范围内
    // 片段到光源的距离范围是 [0, far_plane]，这里的 far_plane 是我们在创建深度立方体贴图时设置的远裁剪面距离。
    // 这里我们将距离除以 far_plane，以确保距离在 [0, far_plane] 范围内被映射到 [0.0, 1.0]。
    lightDistance = lightDistance / far_plane;

    // write this as modified depth
    gl_FragDepth = lightDistance;
}