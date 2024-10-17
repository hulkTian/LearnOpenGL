//
// Created by ts on 2024/10/17.
//

#include "AdvancedLighting.h"

void AdvancedLighting::Create() {
    GLUtils::printGLInfo();

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // build and compile shaders
    // -------------------------
    m_ProgramObj = GLUtils::createProgram("shaders/vs_advanced_lighting.glsl", "shaders/fs_advanced_lighting.glsl");
}

void AdvancedLighting::Draw() {

}

void AdvancedLighting::Shutdown() {
    GLBaseSample::Shutdown();
}
