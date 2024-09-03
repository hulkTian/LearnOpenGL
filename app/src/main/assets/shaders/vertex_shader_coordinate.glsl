#version 300 es

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

//输出纹理坐标
out vec2 TexCoord;

//用这个变量接受变换矩阵，来改变顶点坐标
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    // 从右向左变换顶点坐标，最终输出裁剪空间的坐标。OpenGL然后对裁剪坐标执行透视除法从而将它们变换到标准化设备坐标。
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}
