//
// Created by ts on 2024/12/3.
//

#ifndef LEARNOPENGL_IBL_SPECULAR_TEXTURED_H
#define LEARNOPENGL_IBL_SPECULAR_TEXTURED_H

#include "GLBaseSample.h"

class ibl_specular_textured : public GLBaseSample {
public:
    ibl_specular_textured() = default;

    virtual ~ibl_specular_textured() = default;

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

    void generateEnvironmentMap();
    void generateIrradianceMap();
    void generatePreFilterMap();
    void generateBrdfLUT();

    unsigned int captureFBO, captureRBO;

    GLuint ironAlbedoMap;
    GLuint ironNormalMap;
    GLuint ironMetallicMap;
    GLuint ironRoughnessMap;
    GLuint ironAOMap;

    GLuint goldAlbedoMap;
    GLuint goldNormalMap;
    GLuint goldMetallicMap;
    GLuint goldRoughnessMap;
    GLuint goldAOMap;

    GLuint grassAlbedoMap;
    GLuint grassNormalMap;
    GLuint grassMetallicMap;
    GLuint grassRoughnessMap;
    GLuint grassAOMap;

    GLuint plasticAlbedoMap;
    GLuint plasticNormalMap;
    GLuint plasticMetallicMap;
    GLuint plasticRoughnessMap;
    GLuint plasticAOMap;

    GLuint wallAlbedoMap;
    GLuint wallNormalMap;
    GLuint wallMetallicMap;
    GLuint wallRoughnessMap;
    GLuint wallAOMap;
};


#endif //LEARNOPENGL_IBL_SPECULAR_TEXTURED_H
