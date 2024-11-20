#version 300 es
layout (location = 0) in vec3 aPos; // 四边形的顶点
layout (location = 1) in vec2 aTexCoords; // 四边形的纹理坐标

out vec2 TexCoords;

void main()
{
    TexCoords = aTexCoords;
    // 由于不是三维的，所以不需要对顶点坐标进行空间转换
    gl_Position = vec4(aPos, 1.0);
}