#version 300 es
layout (location = 0) in vec3 aPos;

out vec3 WorldPos;

uniform mat4 projection;
uniform mat4 view;

// 顶点着色器只需要按找原样渲染立方体，并将局部坐标作为3D采样向量传递给片元着色器
void main()
{
    WorldPos = aPos;
    gl_Position =  projection * view * vec4(WorldPos, 1.0);
}
