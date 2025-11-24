#version 300 es
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix; // 预先计算好的法线矩阵

// 法向量传递到片段着色器
out vec3 Normal;
// 顶点在世界坐标位置
out vec3 FragPos;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    // Normal = aNormal; 模型空间的法向量无法正确反映光照效果，需要转换到世界空间
    // 注意在着色器中进行矩阵的逆转置运算开销较大，建议在应用程序中预先计算好并传递给着色器
    Normal = normalMatrix * aNormal;
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
