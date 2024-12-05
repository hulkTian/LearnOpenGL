#version 300 es
layout (location = 0) in vec4 vertex;

out vec2 TexCoords;

uniform mat4 projection;

void main()
{
    // 顶点坐标转换到屏幕坐标系
    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
    // 纹理坐标存放在顶点属性的z和w分量中
    TexCoords = vertex.zw;
}