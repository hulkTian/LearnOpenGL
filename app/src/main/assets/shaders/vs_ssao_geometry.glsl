#version 300 es
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec2 TexCoords;
out vec3 Normal;

uniform bool invertedNormals;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // 顶点在视图空间的位置
    vec4 viewPos = view * model * vec4(aPos, 1.0);
    // 保存顶点在视图空间的位置向量到gBuffer，Z分量作为深度
    FragPos = viewPos.xyz;
    // 正常保存纹理坐标，几何处理阶段也没有用到
    TexCoords = aTexCoords;

    // 先反转再置换视图矩阵，用于把法线也变换到视图空间
    mat3 normalMatrix = transpose(inverse(mat3(view * model)));
    // 视图空间的法线向量
    Normal = normalMatrix * (invertedNormals ? -aNormal : aNormal);

    // 正常转换顶点坐标到裁剪空间，保证渲染管线正常执行
    gl_Position = projection * viewPos;
}