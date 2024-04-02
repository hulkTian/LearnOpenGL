#version 300 es
//设置着色器语言版本为：3.0

//声明着色器中浮点变量的默认精度
precision mediump float;

in vec3 colour;
out vec4 frag_colour;

void main() {
    frag_colour = vec4(colour, 1.0);
}
