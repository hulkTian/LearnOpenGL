//
// Created by ts on 2024/10/24.
//

#ifndef LEARNOPENGL_SHADOW_MAPPING_DEPTH_H
#define LEARNOPENGL_SHADOW_MAPPING_DEPTH_H

#include "GLBaseSample.h"

class shadow_mapping_depth : public GLBaseSample {
public:
    shadow_mapping_depth() = default;

    virtual ~shadow_mapping_depth()=default;
    virtual void Create();
    virtual void Draw();
    virtual void Shutdown();

private:
    GLuint planeVAO, planeVBO;
    GLuint quadVAO, quadVBO;
    unsigned int depthMapFBO;
    unsigned int depthMap;
    GLuint m_ProgramObj_debug_quad;
    GLuint woodTexture;
    GLuint cubeVAO;
    GLuint cubeVBO;
    void renderScene();
    void renderCube();
    void renderQuad();
};


#endif //LEARNOPENGL_SHADOW_MAPPING_DEPTH_H
