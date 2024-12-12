//
// Created by ts on 2024/12/9.
//

#ifndef LEARNOPENGL_SPRITERENDERER_H
#define LEARNOPENGL_SPRITERENDERER_H


#include "Shader.h"
#include "Texture2D.h"

class SpriteRenderer {
public:
    SpriteRenderer(Shader shader);

    ~SpriteRenderer();

    void DrawSprite(Texture2D &texture, glm::vec2 position, glm::vec2 size = glm::vec2(10, 10),
                    GLfloat rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));

private:
    Shader shader;
    GLuint quadVAO;

    void initRenderData();
};


#endif //LEARNOPENGL_SPRITERENDERER_H
