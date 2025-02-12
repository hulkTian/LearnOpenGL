#version 300 es
//设置着色器语言版本为：3.0

//声明着色器中浮点变量的默认精度
precision mediump float;

in vec3 Color;

//声明一个输出变量，用作颜色缓冲器
out vec4 fragColor;

void main() {
    /**
     * 颜色在计算器图形中被定义为有4个分量的向量：红、绿、蓝和透明度
     * 每个分量的范围在0.0到1.0之间
     * 下面的代码设置了红色分量为1.0，透明度分量为1.0，显示的颜色就是红色
     */
    fragColor = vec4(Color, 1.0f);
}
