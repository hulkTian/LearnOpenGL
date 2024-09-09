#version 300 es
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
// 设置光源坐标到顶点着色器，转换为视图空间的坐标后输出给片段着色器
uniform vec3 lightPos;

// 法向量传递到片段着色器
out vec3 Normal;
// 输出视图空间的片段坐标
out vec3 FragPos;
// 输出视图空间的光源坐标
out vec3 LightPos;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    Normal = mat3(transpose(inverse(view * model))) * aNormal;
    FragPos = vec3(view * model * vec4(aPos, 1.0));
    LightPos = vec3(view * model * vec4(lightPos, 1.0));
}
