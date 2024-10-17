#version 300 es
precision mediump float;

//输出的片段颜色
out vec4 FragColor;
//顶点着色器输入的顶点颜色
in vec3 ourColor;
//顶点着色器输入的纹理坐标
in vec2 TexCoord;
//内置的全局纹理对象
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    //使用多个纹理，mix函数需要接受两个值作为参数，并对它们根据第三个参数进行线性插值。
    // 0.2 表示第二个参数占比20%，第一个参数占比80%。
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, vec2(TexCoord.x, TexCoord.y)), 0.2);
}