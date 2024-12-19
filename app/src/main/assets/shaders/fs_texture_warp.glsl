#version 300 es
precision mediump float;

//输出的片段颜色
out vec4 FragColor;
//顶点着色器输入的顶点颜色
in vec3 ourColor;
//顶点着色器输入的纹理颜色
in vec2 TexCoord;
//内置的全局纹理对象
uniform sampler2D ourTexture;

void main()
{
    //使用纹理
    FragColor = texture(ourTexture, TexCoord);
    //FragColor = vec4(ourColor, 1.0);
}