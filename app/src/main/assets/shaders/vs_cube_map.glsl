#version 300 es
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    // 使用顶点坐标作为纹理坐标
    TexCoords = aPos;
    vec4 pos = projection * view * vec4(aPos, 1.0);
    // todo 优化：让天空盒子有最大的深度值1.0，再设置深度测试计算方式为GL_LEQUAL，在进行深度测试时只绘制没有箱子区域的片段
    gl_Position = pos.xyww;
}
