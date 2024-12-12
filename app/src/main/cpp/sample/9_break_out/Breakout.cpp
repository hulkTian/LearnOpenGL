//
// Created by ts on 2024/12/6.
//

#include "Breakout.h"
#include "ResourceManager.h"

void Breakout::Create()
{
    // OpenGL configuration
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    game.Init();
}

void Breakout::Draw()
{
    //计算每一帧绘制的时间：先记录当前在开始时间
    float currentFrame = TimeUtils::currentTimeSeconds();

    // Update Game state
    game.Update(deltaTime);

    // Render
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    game.Render();

    // 计算每一帧绘制的时间，再计算当前帧结束时间
    deltaTime = TimeUtils::currentTimeSeconds() - currentFrame;
}

void Breakout::Shutdown()
{
    // Delete all resources as loaded using the resource manager
    ResourceManager::Clear();
}

void Breakout::ProcessInput(int i) {
    //deltaTime = 0.001f;
    // Manage user input
    game.ProcessInput(deltaTime, i);
}
