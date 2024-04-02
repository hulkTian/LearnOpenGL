#version 300 es
//设置着色器语言版本为：3.0

//声明一个输入变量，用作缓存顶点位置
//对于顶点着色器而言，输入的变量都叫顶点属性，OpenGL 一般最大允许16个顶点属性
layout(location = 0) in vec4 vPosition;

/**
* 向量的使用：
* 1，向量的四个分量分别可以通过vec.x,vec.y,vec.z,vec.w来获取
* 2，向量也可以使用重组的方式选择分量
* 3，为减少需要的参数数量，可以直接使用向量来给属性赋值
*/

void main() {
    //gl_Position 是预定义的变量，用作输出顶点位置数据，它是一个vec4类型的变量。
    //每个顶点着色器都必须在gl_Position变量中输出一个位置，给下一阶段使用。
    gl_Position = vPosition;
}
