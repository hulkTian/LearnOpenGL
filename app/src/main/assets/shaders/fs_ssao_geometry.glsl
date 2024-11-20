#version 300 es
precision mediump float;

// gBuffer 需要保存的向量：视图空间的顶点坐标、视图空间的法向量、立方体和背包模型的颜色向量
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gAlbedo;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

void main()
{
    // 存储片段的位置向量到gBuffer
    gPosition = FragPos;
    // 存储法线信息到gBuffer
    gNormal = normalize(Normal);
    // 存储一个偏白的颜色作为每个片段颜色
    gAlbedo.rgb = vec3(0.95);
}