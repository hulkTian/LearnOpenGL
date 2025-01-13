//
// Created by ts on 2024/10/11.
//

#ifndef LEARNOPENGL_CUBEMAPSREFLECTIONEXERCISE_H
#define LEARNOPENGL_CUBEMAPSREFLECTIONEXERCISE_H

#include "GLBaseSample.h"
#include "3_mesh/Model.h"

class CubeMapsReflectionExercise : public GLBaseSample {
public:
    CubeMapsReflectionExercise() = default;

    virtual ~CubeMapsReflectionExercise() = default;

    virtual void Create();

    virtual void Draw();

    virtual void Shutdown();
private:
    GLuint skyboxVAO, skyboxVBO;
    GLuint cubeVAO, cubeVBO;
    GLuint cubemapTexture;
    Model ourModel;
    GLuint m_ProgramObj_mode;
};


#endif //LEARNOPENGL_CUBEMAPSREFLECTIONEXERCISE_H
