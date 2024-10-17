#version 300 es

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

//输出纹理坐标
out vec2 TexCoord;

//用这个变量接受变换矩阵，来改变顶点坐标
uniform mat4 transform;

void main() {
    // gl_Position = vec4(aPos, 1.0);
    gl_Position = transform * vec4(aPos, 1.0f);
    TexCoord = aTexCoord;
}
