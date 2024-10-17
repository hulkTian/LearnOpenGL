//
// Created by ts on 2024/10/10.
//

#ifndef LEARNOPENGL_CUBEMAPSREFRACTION_H
#define LEARNOPENGL_CUBEMAPSREFRACTION_H

#include "GLBaseSample.h"
#include "Model.h"

class CubeMapsRefraction : public GLBaseSample {

public:
    CubeMapsRefraction() = default;

    virtual ~CubeMapsRefraction() = default;

    virtual void Create();

    virtual void Draw();

    virtual void Shutdown();

private:
    GLuint skyboxVAO, skyboxVBO;
    GLuint cubeVAO, cubeVBO;
    GLuint cubemapTexture;
    GLuint m_ProgramObj_cube;
    Model ourModel;
    GLuint m_ProgramObj_mode;

    virtual void DrawMode(glm::mat4 view, glm::mat4 projection);

    virtual void DrawCube(glm::mat4 view, glm::mat4 projection);

    virtual void DrawSkybox(glm::mat4 view, glm::mat4 projection);
};

#endif //LEARNOPENGL_CUBEMAPSREFRACTION_H
