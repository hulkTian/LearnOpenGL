//
// Created by ts on 2024/11/18.
//

#ifndef LEARNOPENGL_DEFERRED_SHADING_H
#define LEARNOPENGL_DEFERRED_SHADING_H

#include "GLBaseSample.h"
#include "Model.h"

class deferred_shading : public GLBaseSample {
public:
    deferred_shading() = default;

    virtual ~deferred_shading() = default;

    virtual void Create();

    virtual void Draw();

    virtual void Shutdown();

private:
    GLuint m_ProgramObj_Geometry_Pass, m_ProgramObj_Lighting_Pass, m_ProgramObj_Light_Box, m_ProgramObj_FBO_Debug;
    Model ourModel;
    GLuint gBuffer, gPosition, gNormal, gAlbedoSpec, rboDepth;
    GLuint cubeVAO, cubeVBO;
    GLuint quadVAO, quadVBO;
    std::vector<glm::vec3> objectPositions;
    std::vector<glm::vec3> lightPositions;
    std::vector<glm::vec3> lightColors;

    void renderCube();

    void renderQuad();
};


#endif //LEARNOPENGL_DEFERRED_SHADING_H
