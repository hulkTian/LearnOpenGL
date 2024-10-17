#version 300 es
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// 法向量传递到片段着色器
out vec3 Normal;
// 顶点在世界坐标位置
out vec3 FragPos;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    //Normal = aNormal;
    Normal = mat3(transpose(inverse(model))) * aNormal;
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
