//
// Created by tzh on 2025/12/30.
//

#ifndef LEARNOPENGL_POINT_SHADOWS_H
#define LEARNOPENGL_POINT_SHADOWS_H

#include "GLBaseSample.h"

class point_shadows : public GLBaseSample {
public:
    point_shadows()=default;

    virtual ~point_shadows()=default;
    virtual void Create();
    virtual void Draw();
    virtual void Shutdown();

private:
    GLuint simpleDepthShader;
    GLuint depthMapFBO;
    GLuint depthCubeMap;
    GLuint woodTexture;
    GLsizei SHADOW_WIDTH = 1024;
    GLsizei SHADOW_HEIGHT = 1024;
    GLuint cubeVAO, cubeVBO;
    glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 0.0f);
    GLfloat aspect = (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT;
    GLfloat near_plane = 1.0f;
    GLfloat far_plane = 25.0f;
    void renderScene(GLuint programObj);
    void renderCube();

    GLuint debugDepthQuad;
};


#endif //LEARNOPENGL_POINT_SHADOWS_H
