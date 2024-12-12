//
// Created by ts on 2024/12/11.
//

#ifndef LEARNOPENGL_POSTPROCESSOR_H
#define LEARNOPENGL_POSTPROCESSOR_H

#include "Texture2D.h"
#include "SpriteRenderer.h"
#include "Shader.h"

// PostProcessor hosts all PostProcessing effects for the Breakout Game. It renders the game on a
// textured quad after which one can enable specific effects by enabling either the Confuse, Chaos
// or Shake boolean.
// It is required to call BeginRender() before rendering the game and EndRender() after rendering
// the game for the class to work.
// PostProcessor类包含Breakout游戏的所有后期处理。它将游戏渲染在一个四边形纹理上，之后就可以通过布尔值启用指定的特效，包括：迷宫特效、混沌特效和摇晃特效。
// 在渲染游戏前需要调用BeginRender()，并且在渲染游戏后需要调用EndRender()。
class PostProcessor {
public:
    Shader PostProcessingShader;
    Texture2D Texture;
    GLuint Width, Height;
    GLboolean Confuse, Chaos, Shake;
    PostProcessor(Shader shader, GLuint width, GLuint height);
    void BeginRender();
    void EndRender();
    void Render(GLfloat time);
private:
    GLuint MSFBO, FBO;
    GLuint RBO;
    GLuint VAO;
    void initRenderData();
};
#endif //LEARNOPENGL_POSTPROCESSOR_H
