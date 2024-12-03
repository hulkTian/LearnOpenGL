//
// Created by ts on 2024/11/28.
//

#ifndef LEARNOPENGL_IBL_IRRADIANCE_H
#define LEARNOPENGL_IBL_IRRADIANCE_H

#include "GLBaseSample.h"

class ibl_irradiance : public GLBaseSample {
public:
    ibl_irradiance() = default;

    virtual ~ibl_irradiance() = default;

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

    GLuint m_ProgramObj_Equirectangular_To_Cubemap, m_ProgramObj_Background, m_ProgramObj_Irradiance;
    GLuint captureFBO;
    GLuint captureRBO;
    GLuint hdrTexture;
    GLuint envCubemap;
    GLuint irradianceMap;

    glm::mat4 captureProjection;
    glm::mat4 captureViews[6];

    unsigned int cubeVAO, cubeVBO;

    void renderSphere();

    void renderCube();
};


#endif //LEARNOPENGL_IBL_IRRADIANCE_H
