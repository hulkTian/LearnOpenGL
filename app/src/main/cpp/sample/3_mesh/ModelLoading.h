//
// Created by ts on 2024/9/25.
//

#ifndef LEARNOPENGL_MODELLOADING_H
#define LEARNOPENGL_MODELLOADING_H

#include "GLBaseSample.h"
#include "Model.h"

class ModelLoading : public GLBaseSample{
public:
    ModelLoading() = default;

    virtual ~ModelLoading() = default;

    virtual void Create();

    virtual void Draw();

    virtual void Shutdown();
private:
    Model ourModel;
    GLuint m_ProgramObj_border;
};


#endif //LEARNOPENGL_MODELLOADING_H
