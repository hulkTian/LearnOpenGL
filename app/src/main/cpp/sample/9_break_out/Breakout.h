//
// Created by ts on 2024/12/6.
//

#ifndef LEARNOPENGL_BREAKOUT_H
#define LEARNOPENGL_BREAKOUT_H

#include "GLBaseSample.h"
#include "Game.h"

class Breakout : public GLBaseSample {
public:
    Breakout() = default;

    ~Breakout() = default;

    virtual void Create();

    virtual void Draw();

    virtual void ProcessInput(int i);

    virtual void Shutdown();

private:
    Game game = Game(SCR_WIDTH, SCR_HEIGHT);

};


#endif //LEARNOPENGL_BREAKOUT_H
