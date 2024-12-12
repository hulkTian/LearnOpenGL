//
// Created by ts on 2024/12/6.
//

#ifndef LEARNOPENGL_GAME_H
#define LEARNOPENGL_GAME_H

#include <GLES3/gl32.h>
#include <sstream>
#include "SpriteRenderer.h"
#include "GameLevel.h"
#include "BallObject.h"
#include "ResourceManager.h"
#include "GLUtils.h"
#include "ParticleGenerator.h"
#include "PostProcessor.h"
#include "PowerUp.h"
#include "TextRenderer.h"

enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

// 为了更好的组织代码，碰撞相关的数据使用typedef定义，包含是否碰撞、碰撞方向和碰撞深度向量
typedef std::tuple<GLboolean, Direction, glm::vec2> Collision;
// 初始化挡板的大小
const glm::vec2 PLAYER_SIZE = glm::vec2(200, 40);
// 初始化挡板的速率
const GLfloat PLAYER_VELOCITY = 1000.0f;
// 初始化球的速度
const glm::vec2 INITIAL_BALL_VELOCITY = glm::vec2(100.0f, -350.0f);
// 球的半径
const GLfloat BALL_RADIUS = 27.0f;

class Game {
public:
    // 游戏状态
    GameState State;
    GLuint Width, Height;
    std::vector<GameLevel> Levels;
    GLuint Level;
    std::vector<PowerUp> PowerUps;
    GLuint Lives;

    // 构造函数/析构函数
    Game(GLuint width, GLuint height);
    ~Game();
    // 初始化游戏状态（加载所有的着色器/纹理/关卡）
    void Init();
    // 游戏循环
    void ProcessInput(GLfloat deltaTime, int key);
    void Update(GLfloat dt);
    void Render();
    void DoCollisions();
    // 重置
    void ResetLevel();
    void ResetPlayer();
    // 道具
    void SpawnPowerUps(GameObject &block);
    void UpdatePowerUps(GLfloat dt);
};

#endif //LEARNOPENGL_GAME_H
