//
// Created by ts on 2024/12/6.
//

#ifndef LEARNOPENGL_TEXTURE2D_H
#define LEARNOPENGL_TEXTURE2D_H

#include <GLES3/gl32.h>

class Texture2D {
public:
    // 纹理对象句柄，对当前纹理的所有操作都需要使用这个ID
    GLuint ID;
    GLuint Width, Height; // 图片分辨率
    GLuint Internal_Format; // 纹理对象格式
    GLuint Image_Format; // 加载的图片格式
    GLuint Wrap_S; // 水平轴的环绕方式
    GLuint Wrap_T; // 垂直轴的环绕方式
    GLuint Filter_Min; // 纹理像素 < 屏幕像素时的过滤模式
    GLuint Filter_Max; // 纹理像素 > 屏幕像素时的过滤模式

    // 构造方法
    Texture2D();

    // 从图片数据生成纹理
    void Generate(GLuint width, GLuint height, unsigned char *data);

    // 绑定纹理到激活的 GL_TEXTURE_2D 纹理对象
    void Bind() const;

};


#endif //LEARNOPENGL_TEXTURE2D_H
