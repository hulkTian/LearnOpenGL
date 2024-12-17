#version 300 es

//声明着色器中浮点变量的默认精度
precision mediump float;

out vec4 fragColor;

//Uniform是一种从CPU中的应用向GPU中的着色器发送数据的方式，但uniform和顶点属性有些不同。
//首先，uniform是全局的(Global)。全局意味着uniform变量必须在每个着色器程序对象中都是独一无二的，而且它可以被着色器程序的任意着色器在任意阶段访问。
//第二，无论你把uniform值设置成什么，uniform会一直保存它们的数据，直到它们被重置或更新。
uniform vec4 ourColor;

void main() {
    fragColor = ourColor;
}
