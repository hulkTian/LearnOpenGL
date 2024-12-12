//
// Created by ts on 2024/12/6.
//
#include "Game.h"

SpriteRenderer    *Renderer;
GameObject        *Player;
BallObject        *Ball;
ParticleGenerator *Particles;
PostProcessor     *Effects;
GLfloat            ShakeTime = 0.0f;
TextRenderer  *Text;

Game::Game(GLuint width, GLuint height) : State(GAME_MENU), Width(width), Height(height), Level(0), Lives(3) {

}

Game::~Game() {
    delete Renderer;
    delete Player;
    delete Ball;
    delete Particles;
    delete Effects;
    delete Text;
}

void Game::Init() {
    // 加载着色器
    ResourceManager::LoadShader("shaders/vs_rendering_sprites.glsl", "shaders/fs_rendering_sprites.glsl", nullptr, "sprite");
    ResourceManager::LoadShader("shaders/vs_particle.glsl", "shaders/fs_particle.glsl", nullptr, "particle");
    ResourceManager::LoadShader("shaders/vs_post_processing.glsl", "shaders/fs_post_processing.glsl", nullptr, "postprocessing");
    // 加载纹理
    ResourceManager::LoadTexture("/data/data/com.example.learnopengl/textures/background.png", GL_FALSE, "background");
    ResourceManager::LoadTexture("/data/data/com.example.learnopengl/textures/awesomeface.png", GL_TRUE, "face");
    ResourceManager::LoadTexture("/data/data/com.example.learnopengl/textures/block.jpeg", GL_FALSE, "block");
    ResourceManager::LoadTexture("/data/data/com.example.learnopengl/textures/block_solid.jpeg", GL_FALSE, "block_solid");
    ResourceManager::LoadTexture("/data/data/com.example.learnopengl/textures/paddle.png", GL_TRUE, "paddle");
    ResourceManager::LoadTexture("/data/data/com.example.learnopengl/textures/particle.png", GL_TRUE, "particle");
    ResourceManager::LoadTexture("/data/data/com.example.learnopengl/textures/powerup_speed.png", GL_TRUE, "powerup_speed");
    ResourceManager::LoadTexture("/data/data/com.example.learnopengl/textures/powerup_sticky.png", GL_TRUE, "powerup_sticky");
    ResourceManager::LoadTexture("/data/data/com.example.learnopengl/textures/powerup_increase.png", GL_TRUE, "powerup_increase");
    ResourceManager::LoadTexture("/data/data/com.example.learnopengl/textures/powerup_confuse.png", GL_TRUE, "powerup_confuse");
    ResourceManager::LoadTexture("/data/data/com.example.learnopengl/textures/powerup_chaos.png", GL_TRUE, "powerup_chaos");
    ResourceManager::LoadTexture("/data/data/com.example.learnopengl/textures/powerup_passthrough.png", GL_TRUE, "powerup_passthrough");
    // 配置着色器
    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    ResourceManager::GetShader("particle").Use().SetInteger("sprite", 0);
    ResourceManager::GetShader("particle").SetMatrix4("projection", projection);
    // 设置专用于渲染的控制
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    // 初始化粒子生成器
    Particles = new ParticleGenerator(ResourceManager::GetShader("particle"), ResourceManager::GetTexture("particle"), 500);
    // 初始化后期处理器
    Effects = new PostProcessor(ResourceManager::GetShader("postprocessing"), this->Width, this->Height);
    // 加载关卡
    GameLevel one; one.Load("/data/data/com.example.learnopengl/game_level/one.lvl", this->Width, this->Height * 0.5);
    GameLevel two; two.Load("/data/data/com.example.learnopengl/game_level/two.lvl", this->Width, this->Height * 0.5);
    GameLevel three; three.Load("/data/data/com.example.learnopengl/game_level/three.lvl", this->Width, this->Height * 0.5);
    GameLevel four; four.Load("/data/data/com.example.learnopengl/game_level/four.lvl", this->Width, this->Height * 0.5);
    this->Levels.push_back(one);
    this->Levels.push_back(two);
    this->Levels.push_back(three);
    this->Levels.push_back(four);
    this->Level = 0;

    // 设置挡板的初始位置，使其中心与场景的水平中心对齐
    glm::vec2 playerPos = glm::vec2(this->Width / 2 - PLAYER_SIZE.x / 2, this->Height - PLAYER_SIZE.y);
    Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));

    // 初始化球对象，并设置球的位置
    glm::vec2 ballPos = playerPos + glm::vec2 (PLAYER_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2);
    Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::GetTexture("face"));

    Text = new TextRenderer(this->Width, this->Height);
    Text->Load("/data/data/com.example.learnopengl/fonts/OCRAEXT.TTF", 50);
}

void Game::Update(GLfloat dt) {
    Ball->Move(dt, this->Width);
    // 检测碰撞
    this->DoCollisions();
    // 更新所有粒子
    Particles->Update(dt, *Ball, 5, glm::vec2(Ball->Radius / 2));
    // 更新道具
    this->UpdatePowerUps(dt);
    // 更新ShakeTime，直到停止特效
    if (ShakeTime > 0.0f) {
        ShakeTime -= dt;
        if (ShakeTime <= 0.0f)
            Effects->Shake = GL_FALSE;
    }
    // 球是否接触底部边界
    if (Ball->Position.y >= this->Height) {
        --this->Lives;
        // 玩家是否已经失去所有生命值，如果是游戏结束
        if (this->Lives == 0) {
            this->ResetLevel();
            this->State = GAME_MENU;
        }
        // 当球接触屏幕底部，就需要重置挡板位置
        this->ResetPlayer();
    }
    LOGE("state = %d", this->State)
    if (this->State == GAME_ACTIVE && this->Levels[this->Level].IsCompleted()) {
        this->ResetLevel();
        this->ResetPlayer();
        Effects->Chaos = GL_TRUE;
        this->State = GAME_WIN;
    }
}

void Game::ProcessInput(GLfloat dt, int key) {
    if (this->State == GAME_MENU) {
        if (key == KEY_A) {
            this->State = GAME_ACTIVE;
        }
        if (key == KEY_W) {
            this->Level = (this->Level + 1) % 4;
        }
        if (key == KEY_S) {
            if (this->Level > 0)
                --this->Level;
            else
                this->Level = 3;
        }
    } else if (this->State == GAME_WIN) {
        if (key == KEY_A) {
            Effects->Chaos = GL_FALSE;
            this->State = GAME_MENU;
        }
    } else if (this->State == GAME_ACTIVE) {
        GLfloat  velocity = PLAYER_VELOCITY * dt;
        // 移动玩家挡板
        if (key == KEY_A) {
            if (Player->Position.x >= 0) {
                Player->Position.x -= velocity;
                // 球固定在挡板上时，移动挡板球也要一起移动
                if (Ball->Stuck)
                    Ball->Position.x -= velocity;
            }
        }
        if (key == KEY_D) {
            if (Player->Position.x <= this->Width - Player->Size.x) {
                Player->Position.x += velocity;
                if (Ball->Stuck)
                    Ball->Position.x += velocity;
            }
        }
        if (key == KEY_S)
            Ball->Stuck = false;
    }
}

void Game::Render() {
    if (this->State == GAME_ACTIVE || this->State == GAME_MENU || this->State == GAME_WIN) {
        // 绑定多重采样帧缓冲，准备缓冲游戏渲染场景
        Effects->BeginRender();
        // 绘制背景
        Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0, 0),
                             glm::vec2(this->Width, this->Height), 0.0f);
        this->Levels[this->Level].Draw(*Renderer);
        Player->Draw(*Renderer);
        // 绘制道具
        for (PowerUp &powerUp : this->PowerUps) {
            if (!powerUp.Destroyed) {
                powerUp.Draw(*Renderer);
            }
        }
        // 粒子效果需要在所有物体前面并在球之后显示
        Particles->Draw();
        Ball->Draw(*Renderer);
        // 游戏渲染场景结束，多重采样帧缓冲将场景存放到了颜色缓冲附件。将多重采样帧缓冲的颜色附件作为读取目标提供给中间帧缓冲作为纹理附件保存
        Effects->EndRender();
        // 将中间帧缓冲中的纹理交给后期处理着色器进行特效处理后显示
        Effects->Render(TimeUtils::currentTimeSeconds());

        // 文字不需要做后期处理
        std::stringstream ss;
        ss << this->Lives;
        Text->RenderText("Lives:" + ss.str(), 70.0f, 25.0f, 1.0f);
    }
    if (this->State == GAME_MENU) {
        Text->RenderText("Press ENTER to start", 670.0f, this->Height / 2, 1.5f);
        Text->RenderText("Press W or S to select level", 700.0f, this->Height / 2 + 60.0f, 1.0f);
    }
    if (this->State == GAME_WIN) {
        Text->RenderText("You WON!!!", 950.0f, this->Height / 2 - 50.0f, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        Text->RenderText("Press ENTER to retry or ESC to quit", 570.0f, this->Height / 2, 1.0f, glm::vec3(1.0f, 1.0f, 0.0f));
    }
}

void Game::ResetLevel() {
    if (this->Level == 0)
        this->Levels[0].Load("/data/data/com.example.learnopengl/game_level/one.lvl", this->Width, this->Height * 0.5f);
    else if (this->Level == 1)
        this->Levels[1].Load("/data/data/com.example.learnopengl/game_level/two.lvl", this->Width, this->Height * 0.5f);
    else if (this->Level == 2)
        this->Levels[2].Load("/data/data/com.example.learnopengl/game_level/three.lvl", this->Width, this->Height * 0.5f);
    else if (this->Level == 3)
        this->Levels[3].Load("/data/data/com.example.learnopengl/game_level/four.lvl", this->Width, this->Height * 0.5f);
    /// 重置玩家生命值
    this->Lives = 3;
}

void Game::ResetPlayer() {
    // Reset player/ball stats
    Player->Size = PLAYER_SIZE;
    Player->Position = glm::vec2(this->Width / 2 - PLAYER_SIZE.x / 2, this->Height - PLAYER_SIZE.y);
    Ball->Reset(Player->Position + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -(BALL_RADIUS * 2)), INITIAL_BALL_VELOCITY);
}

// 此方法用于判断是否存在相同类型且是激活状态的道具
GLboolean IsOtherPowerUpActive(std::vector<PowerUp> &powerUps, std::string type);

// 更新道具位置，并持续减少道具的持续时间，当时间减少到小于0时，使道具失效
void Game::UpdatePowerUps(GLfloat dt) {
    for (PowerUp &powerUp : this->PowerUps) {
        powerUp.Position += powerUp.Velocity * dt;
        if (powerUp.Activated) {
            powerUp.Duration -= dt;
            if (powerUp.Duration <= 0.0f) {
                // 之后会将这个道具移除
                powerUp.Activated = GL_FALSE;
                // 停用效果
                if (powerUp.Type == "sticky") {
                    if (!IsOtherPowerUpActive(this->PowerUps, "sticky")) {
                        // 仅当没有其他sticky效果处于激活状态时重置，以下同理
                        Ball->Sticky = GL_FALSE;
                        Player->Color = glm::vec3(1.0f);
                    }
                } else if (powerUp.Type == "pass-through") {
                    if (!IsOtherPowerUpActive(this->PowerUps, "pass-through")) {
                        Ball->PassThrough = GL_FALSE;
                        Ball->Color = glm::vec3(1.0f);
                    }
                } else if (powerUp.Type == "confuse") {
                    if (!IsOtherPowerUpActive(this->PowerUps, "confuse")) {
                        Effects->Confuse = GL_FALSE;
                    }
                } else if (powerUp.Type == "chaos") {
                    if (!IsOtherPowerUpActive(this->PowerUps, "chaos")) {
                        Effects->Chaos = GL_FALSE;
                    }
                }
            }
        }
    }
    // 移除所有无效且以及消失的道具
    this->PowerUps.erase(std::remove_if(this->PowerUps.begin(), this->PowerUps.end(),
                                        [](const PowerUp &powerUp) {
                                            return powerUp.Destroyed && !powerUp.Activated;
                                        }), this->PowerUps.end());
}

// 几率生成方法
GLboolean ShouldSpawn(GLuint chance)
{
    GLuint random = rand() % chance;
    return random == 0;
}

// 每次砖块被摧毁时我们希望以一定几率生成一个道具
void Game::SpawnPowerUps(GameObject &block) {
    if (ShouldSpawn(25)) // 普通股道具1/75的几率
        this->PowerUps.push_back(PowerUp("speed", glm::vec3(0.5f, 0.5f, 1.0f), 0.0f, block.Position, ResourceManager::GetTexture("powerup_speed")));
    if (ShouldSpawn(25))
        this->PowerUps.push_back(PowerUp("sticky", glm::vec3(1.0f, 0.5f, 1.0f), 20.0f, block.Position, ResourceManager::GetTexture("powerup_sticky")));
    if (ShouldSpawn(25))
        this->PowerUps.push_back(PowerUp("pass-through", glm::vec3(0.5f, 1.0f, 0.5f), 10.0f, block.Position, ResourceManager::GetTexture("powerup_passthrough")));
    if (ShouldSpawn(25))
        this->PowerUps.push_back(PowerUp("pad-size-increase", glm::vec3(1.0f, 0.6f, 0.4), 0.0f, block.Position, ResourceManager::GetTexture("powerup_increase")));
    if (ShouldSpawn(15)) // 负面道具被更频繁地生成
        this->PowerUps.push_back(PowerUp("confuse", glm::vec3(1.0f, 0.3f, 0.3f), 15.0f, block.Position, ResourceManager::GetTexture("powerup_confuse")));
    if (ShouldSpawn(15))
        this->PowerUps.push_back(PowerUp("chaos", glm::vec3(0.9f, 0.25f, 0.25f), 15.0f, block.Position, ResourceManager::GetTexture("powerup_confuse")));
}

// 根据道具类型激活道具效果
void ActivatePowerUp(PowerUp &powerUp) {
    if (powerUp.Type == "speed") {
        Ball->Velocity *= 1.2; // 增加球速
    } else if (powerUp.Type == "sticky") {
        Ball->Sticky = GL_TRUE; // 球可以粘连在挡板上
        Player->Color = glm::vec3(1.0f, 0.5f, 1.0f);
    } else if (powerUp.Type == "pass-through") {
        Ball->PassThrough = GL_TRUE; // 球可以穿过实心砖块
        Ball->Color = glm::vec3(1.0f, 0.5f, 0.5f);
    } else if (powerUp.Type == "pad-size-increase") {
        Player->Size.x += 50;
    } else if (powerUp.Type == "confuse") {
        if (!Effects->Chaos)
            Effects->Confuse = GL_TRUE; // 只在chaos未激活时生效，chaos同理
    } else if (powerUp.Type == "chaos") {
        if (!Effects->Confuse)
            Effects->Chaos = GL_TRUE;
    }
}

// 遍历道具数组，查看是否存在有指定类型的道具是有效的
GLboolean IsOtherPowerUpActive(std::vector<PowerUp> &powerUps, std::string type) {
    for (const PowerUp &powerUp: powerUps) {
        if (powerUp.Activated)
            if (powerUp.Type == type)
                return GL_TRUE;
    }
    return GL_FALSE;
}

// 碰撞检测
GLboolean CheckCollision(GameObject &one, GameObject &two);
Collision CheckCollision(BallObject &one, GameObject &two);
Direction VectorDirection(glm::vec2 closest);

// 检查球与关卡中的砖块是否发生碰撞。如果检测到碰撞，将砖块的Destroyed属性设置为true，砖块将不会被渲染
void Game::DoCollisions() {
    for (GameObject &box: this->Levels[this->Level].Bricks) {
        if (!box.Destroyed) {
            Collision collision = CheckCollision(*Ball, box);
            // 如果collision 是 true
            if (std::get<0>(collision)) {
                // 如果砖块不是实心就销毁砖块
                if (!box.IsSolid) {
                    box.Destroyed = GL_TRUE;
                    // 砖块被销毁后随机生成道具
                    this->SpawnPowerUps(box);
                } else {
                    // 如果是实心砖块就激活Shake特效
                    ShakeTime = 0.05f;
                    Effects->Shake = true;
                }
                // 碰撞处理
                Direction dir = std::get<1>(collision);
                glm::vec2 diff_vector = std::get<2>(collision);
                // 当激活穿过道具后，非实心转不需要碰撞处理
                if (!(Ball->PassThrough && !box.IsSolid)) {
                    // 水平方向碰撞
                    if (dir == LEFT || dir == RIGHT)  {
                        Ball->Velocity.x = -Ball->Velocity.x; // 反转水平速度
                        // 重定位
                        GLfloat penetration = Ball->Radius - std::abs(diff_vector.x);
                        if (dir == LEFT)
                            Ball->Position.x += penetration; // 将球右移
                        else
                            Ball->Position.x -= penetration; // 将球左移
                    } else {// 垂直方向碰撞
                        Ball->Velocity.y = -Ball->Velocity.y; // 反转垂直速度
                        // 重定位
                        GLfloat penetration = Ball->Radius - std::abs(diff_vector.y);
                        if (dir == UP)
                            Ball->Position.y -= penetration; // 将球上移
                        else
                            Ball->Position.y += penetration; // 将球下移
                    }
                }
            }
        }
    }
    // 检测道具和挡板的碰撞
    for (PowerUp &powerUp : this->PowerUps) {
        // 道具还没有被销毁
        if (!powerUp.Destroyed) {
            // 首先检查道具位置是否到达屏幕底部，如果到了就设置为销毁状态
            if (powerUp.Position.y >= this->Height)
                powerUp.Destroyed = GL_TRUE;

            // 碰撞检测道具和挡板是否发生碰撞，如果碰撞就激活道具效果并不再渲染道具
            if (CheckCollision(*Player, powerUp)) {
                ActivatePowerUp(powerUp);
                powerUp.Destroyed = GL_TRUE;
                powerUp.Activated = GL_TRUE;
            }
        }
    }
    // 检测球和挡板的碰撞
    Collision result = CheckCollision(*Ball, *Player);
    if (!Ball->Stuck && std::get<0>(result)) {
        // 检查碰到了挡板的哪个位置，并根据碰到哪个位置来改变速度
        GLfloat centerBoard = Player->Position.x + Player->Size.x / 2;
        GLfloat distance = (Ball->Position.x + Ball->Radius) - centerBoard;
        GLfloat percentage = distance / (Player->Size.x / 2);
        // 依据结果移动
        GLfloat strength = 2.0f;
        glm::vec2 oldVelocity = Ball->Velocity;
        Ball->Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
        //粘板问题 Ball->Velocity.y = -Ball->Velocity.y;
        Ball->Velocity.y = -1 * abs(Ball->Velocity.y);
        Ball->Velocity = glm::normalize(Ball->Velocity) * glm::length(oldVelocity);
    }
}

// 矩形碰撞检测
GLboolean CheckCollision(GameObject &one, GameObject &two) // AABB - AABB collision
{
    // Collision x-axis?
    GLboolean collisionX = one.Position.x + one.Size.x >= two.Position.x &&
                           two.Position.x + two.Size.x >= one.Position.x;
    // Collision y-axis?
    GLboolean collisionY = one.Position.y + one.Size.y >= two.Position.y &&
                           two.Position.y + two.Size.y >= one.Position.y;
    // Collision only if on both axes
    return collisionX && collisionY;
}

// 圆碰撞检测，需要找到圆心与砖块最近的点P，并判断点P到圆心的距离是否小于半径
Collision CheckCollision(BallObject &one, GameObject &two) {
    // 获得圆的中心
    glm::vec2 center(one.Position + one.Radius);
    // 计算AABB的信息（中心、半边长）
    glm::vec2 aabb_half_extents(two.Size.x / 2, two.Size.y / 2);
    glm::vec2 aabb_center(two.Position.x + aabb_half_extents.x, two.Position.y + aabb_half_extents.y);
    // 获取两个中心的差矢量
    glm::vec2 difference = center - aabb_center;
    // 以砖块的中心为原点构建一个约束范围，使差矢量的两个分量都约束在这个范围内，便于找到最近点P
    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
    // AABB_center加上clamped这样就得到了碰撞箱上距离圆最近的点closest
    glm::vec2 closest = aabb_center + clamped;
    // 获得圆心center和最近点closest的矢量并判断是否 length <= radius
    difference = closest - center;
    if (glm::length(difference) < one.Radius) {
        return std::make_tuple(GL_TRUE, VectorDirection(difference), difference);
    } else {
        return std::make_tuple(GL_FALSE, UP, glm::vec2(0, 0));
    }
}

// 此函数比较了target矢量和compass数组中各方向矢量。compass数组中与target角度最接近的矢量，即是返回给函数调用者的Direction。
Direction VectorDirection(glm::vec2 target) {
    glm::vec2 compass[] = {
            glm::vec2(0.0f, 1.0f),  // 上
            glm::vec2(1.0f, 0.0f),  // 右
            glm::vec2(0.0f, -1.0f), // 下
            glm::vec2(-1.0f, 0.0f)  // 左
    };
    GLfloat max = 0.0f;
    GLuint best_match = -1;
    for (GLuint i = 0; i < 4; i++) {
        GLfloat dot_product = glm::dot(glm::normalize(target), compass[i]);
        if (dot_product > max) {
            max = dot_product;
            best_match = i;
        }
    }
    return (Direction) best_match;
}

