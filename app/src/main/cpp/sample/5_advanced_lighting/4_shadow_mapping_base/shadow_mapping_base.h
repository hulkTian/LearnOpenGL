//
// Created by ts on 2024/10/25.
//

#ifndef LEARNOPENGL_SHADOW_MAPPING_BASE_H
#define LEARNOPENGL_SHADOW_MAPPING_BASE_H

#include "GLBaseSample.h"

class shadow_mapping_base : public GLBaseSample {
public:
    shadow_mapping_base()=default;

    virtual ~shadow_mapping_base()=default;
    virtual void Create();
    virtual void Draw();
    virtual void Shutdown();

private:
    GLuint simpleDepthShader, debugDepthQuad;
    GLuint planeVAO, planeVBO;
    GLuint woodTexture;
    GLuint depthMapFBO;
    GLuint depthMap;
    glm::vec3 lightPos = glm::vec3(-2.0f, 4.0f, -1.0f);
    GLuint cubeVAO, cubeVBO;
    GLuint quadVAO, quadVBO;
    void renderScene(GLuint programObj);
    void renderCube();
    void renderQuad();
};


#endif //LEARNOPENGL_SHADOW_MAPPING_BASE_H
