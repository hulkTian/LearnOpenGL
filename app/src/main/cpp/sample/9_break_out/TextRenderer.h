//
// Created by ts on 2024/12/12.
//

#ifndef LEARNOPENGL_TEXTRENDERER_H
#define LEARNOPENGL_TEXTRENDERER_H

#include <map>

#include <GLES3/gl32.h>
#include "Texture2D.h"
#include "Shader.h"
#include "freetype/ft2build.h"
#include FT_FREETYPE_H

/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
    GLuint TextureID;   // ID handle of the glyph texture
    glm::ivec2 Size;    // Size of glyph
    glm::ivec2 Bearing; // Offset from baseline to left/top of glyph
    FT_Pos Advance;     // Horizontal offset to advance to next glyph
};

class TextRenderer {
public:
    // Holds a list of pre-compiled Characters
    std::map<GLchar, Character> Characters;
    // Shader used for text rendering
    Shader TextShader;
    // Constructor
    TextRenderer(GLuint width, GLuint height);
    // Pre-compiles a list of characters from the given font
    void Load(std::string font, GLuint fontSize);
    // Renders a string of text using the precompiled list of characters
    void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color = glm::vec3(1.0f));
private:
    // Render state
    GLuint VAO, VBO;
};


#endif //LEARNOPENGL_TEXTRENDERER_H
