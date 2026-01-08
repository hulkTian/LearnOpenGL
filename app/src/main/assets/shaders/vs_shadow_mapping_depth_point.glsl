#version 320 es
layout (location = 0) in vec3 aPos;

uniform mat4 model;
//uniform mat4 shadowMatrix; // 当前面用的 view-proj 矩阵

//out vec4 FragPos;

void main() {
    //FragPos = model * vec4(aPos, 1.0);
    //gl_Position = shadowMatrix * FragPos;
    gl_Position = model * vec4(aPos, 1.0);
}