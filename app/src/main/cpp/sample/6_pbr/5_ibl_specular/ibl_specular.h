//
// Created by ts on 2024/11/28.
//

#ifndef LEARNOPENGL_IBL_SPECULAR_H
#define LEARNOPENGL_IBL_SPECULAR_H
#include "GLBaseSample.h"

class ibl_specular : public GLBaseSample {
public:
    ibl_specular() = default;

    virtual ~ibl_specular() = default;

    virtual void Create();

    virtual void Draw();

    virtual void Shutdown();

private:
    GLuint sphereVAO;
    unsigned int indexCount;
    // lights
    // ------
    glm::vec3 lightPositions[4];
    glm::vec3 lightColors[4];
    int nrRows = 7;
    int nrColumns = 7;
    float spacing = 2.5;
    glm::mat4 projection;

    GLuint m_ProgramObj_Equirectangular_To_Cubemap, m_ProgramObj_Background, m_ProgramObj_Irradiance,
    m_ProgramObj_Prefilter, m_ProgramObj_Brdf;
    GLuint captureFBO;
    GLuint captureRBO;
    GLuint hdrTexture;
    GLuint envCubemap;
    GLuint irradianceMap;
    GLuint prefilterMap;
    GLuint brdfLUTTexture;

    glm::mat4 captureProjection;
    glm::mat4 captureViews[6];

    unsigned int cubeVAO, cubeVBO;
    unsigned int quadVAO, quadVBO;

    void renderSphere();

    void renderCube();

    void renderQuad();
};


#endif //LEARNOPENGL_IBL_SPECULAR_H
