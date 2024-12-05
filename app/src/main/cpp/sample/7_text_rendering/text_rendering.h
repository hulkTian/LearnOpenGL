//
// Created by ts on 2024/12/4.
//

#ifndef LEARNOPENGL_TEXT_RENDERING_H
#define LEARNOPENGL_TEXT_RENDERING_H

#include "GLBaseSample.h"
// 构建FreeType 的动态库后需要将头文件目录复制到项目的include目录下，不能包含到include的子目录，不然会找不到头文件
#include "freetype/ft2build.h"
#include FT_FREETYPE_H
#include <map>

class text_rendering : public GLBaseSample {

public:

    text_rendering() = default;

    ~text_rendering() {
        glDeleteBuffers(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    virtual void Create();
    virtual void Draw();
    virtual void Shutdown();

private:
    GLuint VAO, VBO;
    std::wstring text1;
    std::wstring text2;

    // 字形位图度量值结构体
    struct Character
    {
        GLuint TexturedID;   // 字形纹理ID
        glm::ivec2 Size;     // 字形的宽度和高度
        glm::ivec2 Bearing;  // 从基准线到字形左部/顶部的偏移值
        FT_Pos Advance;      // 原点据下一个字形原点的距离
    };

    std::map<wchar_t, Character> Characters;

    void loadText(const FT_Face face, const std::wstring text);

    void RenderText(std::wstring text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
};


#endif //LEARNOPENGL_TEXT_RENDERING_H
