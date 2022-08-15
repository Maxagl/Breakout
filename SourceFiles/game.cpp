/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "../HeaderFiles/game.h"
#include "../HeaderFiles/resource_manager.h"
#include "../HeaderFiles/sprite_renderer.h"
#include "../HeaderFiles/game_level.h"
#include <tuple>
#include "../HeaderFiles/particle_generator.h"
#include "../HeaderFiles/post_processor.h"
#include <irrklang/irrKlang.h>
#include "../HeaderFiles/text_renderer.h"
#include <sstream>
#include <iostream>
#include <math.h> 
#include "../HeaderFiles/Contactlistener.h"
#include "../HeaderFiles/light2d.h"
using namespace irrklang;
#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

const glm::vec2 PLAYER_SIZE(10.0f, 2.0f);
const float PLAYER_VELOCITY(30.0f);

SpriteRenderer* Renderer;
GameObject* WallUp;
GameObject* WallLeft;
GameObject* WallRight;
GameObject* block;
ParticleGenerator* Particles;
light2d* ballLight;

TextRenderer* Text;
Contactlistener mylistener;

float timeStep = 1.0f / 60.0f;
int32 velocityIterations = 8;
int32 positionIterations = 3;

BodyState Wall = BodyState::WALL;
BodyState ballstate = BodyState::BALL;
BodyState blockState = BodyState::BLOCK;
BodyState playerstate = BodyState::PADDLE;

uint16 wallEntity = EntityCategory::WALL;
uint16 playerEntity = EntityCategory::PADDLE;
uint16 playerMask = EntityCategory::WALL | EntityCategory::BALL | EntityCategory::POWER;
uint16 blockEntity = EntityCategory::BLOCK;
uint16 blockMask = EntityCategory::WALL | EntityCategory::BALL;



Game::Game(unsigned int width, unsigned int height)
    : State(GAME_MENU), Keys(), Width(width), Height(height), Level(0), Lives(3)
{

    b2Vec2 gravity(0.0f, 2.0f);
    myworld = std::make_unique<b2World>(gravity);
    myworld->SetContactListener(&mylistener);
    BodyState mybodystate = BodyState::WALL;
    if (mybodystate == BodyState::WALL)
    {
        std::cout << "WALL" << std::endl;
    }
    else if (mybodystate == BodyState::BRICK)
        std::cout << "BRICK" << std::endl;
    else if (mybodystate == BodyState::PADDLE)
        std::cout << "PADDLE" << std::endl;
    else if (mybodystate == BodyState::DEFAULT)
        std::cout << "DEFAULT" << std::endl;
    SoundEngine->setSoundVolume(1.0);
}

Game::~Game()
{
    delete Renderer;
    delete Player;
    delete Ball;
    delete Particles;
    delete Effects;
    SoundEngine->drop();
}

void Game::Init()
{

    // load shaders
    ResourceManager::LoadShader("Shaders/sprite.vs", "Shaders/sprite.frag", nullptr, "sprite");
    ResourceManager::LoadShader("Shaders/particle.vs", "Shaders/particle.frag", nullptr, "particle");
    ResourceManager::LoadShader("Shaders/post_processing.vs", "Shaders/post_processing.frag", nullptr, "postprocessing");
    ResourceManager::LoadShader("Shaders/light2d.vs", "Shaders/light2d.frag", nullptr, "light");

    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width / 10.0f),
        static_cast<float>(this->Height / 10.0f), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);

    ResourceManager::GetShader("particle").Use().SetMatrix4("projection", projection);
    ResourceManager::GetShader("particle").SetInteger("sprite", 0);

    ResourceManager::GetShader("light").Use().SetMatrix4("projection", projection);


    // load textures
    ResourceManager::LoadTexture("images/background.jpg", false, "background");
    ResourceManager::LoadTexture("images/awesomeface.png", true, "face");
    ResourceManager::LoadTexture("images/block.png", false, "block");
    ResourceManager::LoadTexture("images/block_solid.png", false, "block_solid");
    ResourceManager::LoadTexture("images/paddle.png", true, "paddle");
    ResourceManager::LoadTexture("images/particle.png", true, "particle");
    ResourceManager::LoadTexture("images/powerup_speed.png", true, "powerup_speed");
    ResourceManager::LoadTexture("images/powerup_sticky.png", true, "powerup_sticky");
    ResourceManager::LoadTexture("images/powerup_increase.png", true, "powerup_increase");
    ResourceManager::LoadTexture("images/powerup_confuse.png", true, "powerup_confuse");
    ResourceManager::LoadTexture("images/powerup_chaos.png", true, "powerup_chaos");
    ResourceManager::LoadTexture("images/powerup_passthrough.png", true, "powerup_passthrough");
    ResourceManager::LoadTexture("images/cai.png", true, "cai");

    // set render-specific controls
    Shader myShader;
    myShader = ResourceManager::GetShader("sprite");
    Renderer = new SpriteRenderer(myShader);
    Effects = new PostProcessor(ResourceManager::GetShader("postprocessing"), this->Width, this->Height);

    // configure game objects



    
    WallUp = new GameObject(myworld.get(), glm::vec2(0.0f, -1.0f), glm::vec2(80.0f, 2.0f),
        ResourceManager::GetTexture("paddle"), &Wall); 
    WallUp->gamebox.CreateKinematicBody(myworld.get(), 0.0f, -1.0f, 80.0f, 1.0f, &Wall, wallEntity);

    WallLeft = new GameObject(myworld.get(), glm::vec2(-1.0f, 0.0f), glm::vec2(1.0f, 60.0f),
        ResourceManager::GetTexture("paddle"), &Wall);
    WallLeft->gamebox.CreateKinematicBody(myworld.get(), -1.0f, 0.0f, 0.5f, 30.0f, &Wall, wallEntity);

    WallRight = new GameObject(myworld.get(), glm::vec2(81.0f, 0.0f), glm::vec2(1.0f, 60.0f),
        ResourceManager::GetTexture("paddle"), &Wall);
    WallRight->gamebox.CreateKinematicBody(myworld.get(), 81.0f, 0.0f, 0.5f, 30.0f, &Wall, wallEntity);


    glm::vec2 playerPos = glm::vec2(
        this->Width / 2.0f /10 - PLAYER_SIZE.x / 2.0f,
        this->Height / 10 - PLAYER_SIZE.y
    );
    Player = new GameObject(myworld.get(), playerPos, PLAYER_SIZE, 
        ResourceManager::GetTexture("paddle"), &playerstate);
    Player->gamebox.CreateKinematicBody(myworld.get(), playerPos.x, playerPos.y, PLAYER_SIZE.x / 2, PLAYER_SIZE.y / 2, &playerstate, playerEntity, playerMask);
    
    glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS,
        -BALL_RADIUS * 2.0f);
    Ball = new BallObject(myworld.get(), ballPos, BALL_RADIUS,
        ResourceManager::GetTexture("face"), &ballstate);

    
    glm::vec2 blockPos = glm::vec2(
        this->Width / 2.0f / 10 - PLAYER_SIZE.x / 2.0f,
        this->Height / 10 - PLAYER_SIZE.y - this->Height / 30
    );
    block = new GameObject(myworld.get(), playerPos, PLAYER_SIZE,
        ResourceManager::GetTexture("paddle"), &playerstate);
    block->gamebox.CreateKinematicBody(myworld.get(), blockPos.x, blockPos.y, PLAYER_SIZE.x / 2, PLAYER_SIZE.y / 2, &blockState, blockEntity, blockMask);
    block->gamebox.getbody()->SetAngularVelocity(90 * DEGTORAD);

    glm::vec3 lightcolor = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec2 lightsize = glm::vec2(Ball->Size.x * 10.0f, Ball->Size.y * 10.0f);
    glm::vec2 lightposition = glm::vec2((Ball->Position.x - lightsize.x/2 + Ball->Size.x/2.0f), (Ball->Position.y - lightsize.y/2 + Ball->Size.y/2.0f));
    ballLight = new light2d(ResourceManager::GetShader("light"), lightsize, lightposition, lightcolor);
    std::cout << Ball->Position.x << std::endl;
    std::cout << Ball->Position.y << std::endl;


    Particles = new ParticleGenerator(
        ResourceManager::GetShader("particle"), 
        ResourceManager::GetTexture("particle"),
        500);
    ISound* backm = SoundEngine->play2D("audio/man-is-he-mega-glbml-22045.mp3", true, false, true);
    if(backm)
        backm->setVolume(0.2);
    Text = new TextRenderer(this->Width, this->Height);
    Text->Load("font/PixeloidSansBold-RpeJo.ttf", 24);
    RenderingLevel.Load(myworld.get(), "levels/three.lvl", this->Width / 10, this->Height / 2 / 10);
    this->Level = 2;
}

void Game::Update(float dt)
{
    Particles->Update(dt, *Ball, 5, glm::vec2(Ball->Radius / 2.0f));
    this->UpdatePowerUps(dt);

    if (ShakeTime > 0.0f)
    {
        ShakeTime -= dt;
        if (ShakeTime <= 0.0f)
            Effects->Shake = false;
    }

    if (Ball->Position.y >= this->Height / 10.0f) // did ball reach bottom edge?
    {
        --this->Lives;
        // did the player lose all his lives? : Game over
        if (this->Lives == 0)
        {
            this->ResetLevel();
            this->State = GAME_LOST;
        }
        this->ResetPlayer();
    }
    if (this->State == GAME_ACTIVE && RenderingLevel.IsCompleted())
    {
        this->ResetLevel();
        this->ResetPlayer();
        Effects->Chaos = true;
        this->State = GAME_WIN;
    }
    myworld.get()->Step(timeStep, velocityIterations, positionIterations);
    for (auto dbox : boxstoDestory_brick)
    {
        this->SpawnPowerUps(*dbox);
        myworld->DestroyBody(dbox->getbody());
        dbox->Destroyed = true;
        
    }
    for (auto dbox : boxstoDestory_powerup)
    {
        ActivatePowerUp(*dbox);
        dbox->setisactivated(true);
        // Change all bricks to sensor, ball seems to destory every brick but not effected
        if (Ball->PassThrough == true && !Ball->gamebox.getsensor())
        {
            for (GameObject*& tile : RenderingLevel.Bricks)
                if (!tile->gamebox.Destroyed)
                {
                    if(tile->gamebox.getbodystate() == BodyState::BRICK)
                        tile->gamebox.getbody()->GetFixtureList()[0].SetSensor(true);
                }
        }
        myworld->DestroyBody(dbox->getbody());
        dbox->Destroyed = true;
        
    }
    if (PADDLE_BALL)
    {
        b2Vec2 velocity = Ball->gamebox.getbody()->GetLinearVelocity();
        if (Ball->Sticky == true)
        {
            b2Vec2 ballp = Ball->gamebox.getbody()->GetPosition();
            Ball->gamebox.getbody()->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
            Ball->gamebox.getbody()->SetTransform(b2Vec2(ballp.x, this->Height / 10 - PLAYER_SIZE.y - BALL_RADIUS), 0.0f);
            Ball->Stuck = true;
        }
        else
        {
            float percentage = (Ball->gamebox.getbody()->GetPosition().x - Player->gamebox.getbody()->GetPosition().x) / PLAYER_SIZE.x;
            Ball->gamebox.getbody()->SetLinearVelocity(b2Vec2(PLAYER_VELOCITY * percentage, -40.0f));
        }
        PADDLE_BALL = false;
    }
    boxstoDestory_brick.clear();
    boxstoDestory_powerup.clear();
}

void Game::ProcessInput(float dt)
{
    if (this->State == GAME_MENU || this->State == GAME_LOST)
    {
        if (this->Keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER])
        {
            this->State = GAME_ACTIVE;
            this->KeysProcessed[GLFW_KEY_ENTER] = true;
            this->ResetPlayer();
        }
        if (this->Keys[GLFW_KEY_W] && !this->KeysProcessed[GLFW_KEY_W])
        {
            this->Level = (this->Level + 1) % 5;
            this->KeysProcessed[GLFW_KEY_W] = true;
            if (this->Level == 0)
                RenderingLevel.Load(myworld.get(), "one.lvl", this->Width / 10.0f, this->Height / 2 / 10.0f);
            else if (this->Level == 1)
                RenderingLevel.Load(myworld.get(), "two.lvl", this->Width / 10.0f, this->Height / 2 / 10.0f);
            else if (this->Level == 2)
                RenderingLevel.Load(myworld.get(), "three.lvl", this->Width / 10.0f, this->Height / 2 / 10.0f);
            else if (this->Level == 3)
                RenderingLevel.Load(myworld.get(), "four.lvl", this->Width / 10.0f, this->Height / 2 / 10.0f);
            else if (this->Level == 4)
                RenderingLevel.Load(myworld.get(), "five.lvl", this->Width / 10.0f, this->Height / 2 / 10.0f);
        }
        if (this->Keys[GLFW_KEY_S] && !this->KeysProcessed[GLFW_KEY_S])
        {
            if (this->Level > 0)
                --this->Level;
            else
                this->Level = 4;
            //this->Level = (this->Level - 1) % 4;
            this->KeysProcessed[GLFW_KEY_S] = true;
            if (this->Level == 0)
                RenderingLevel.Load(myworld.get(), "one.lvl", this->Width / 10.0f, this->Height / 2 / 10.0f);
            else if (this->Level == 1)
                RenderingLevel.Load(myworld.get(), "two.lvl", this->Width / 10.0f, this->Height / 2 / 10.0f);
            else if (this->Level == 2)
                RenderingLevel.Load(myworld.get(), "three.lvl", this->Width / 10.0f, this->Height / 2 / 10.0f);
            else if (this->Level == 3)
                RenderingLevel.Load(myworld.get(), "four.lvl", this->Width / 10.0f, this->Height / 2 / 10.0f);
            else if (this->Level == 4)
                RenderingLevel.Load(myworld.get(), "five.lvl", this->Width / 10.0f, this->Height / 2 / 10.0f);
        }
    }

    if (this->State == GAME_WIN)
    {
        if (this->Keys[GLFW_KEY_ENTER])
        {
            this->KeysProcessed[GLFW_KEY_ENTER] = true;
            Effects->Chaos = false;
            this->State = GAME_MENU;
        }
    }


    if (this->State == GAME_ACTIVE)
    {
        float velocity = PLAYER_VELOCITY * dt * 2;
        // move playerboard
        float x = Player->gamebox.getbody()->GetPosition().x;
        float y = Player->gamebox.getbody()->GetPosition().y;
        float left_bound = Player->Size.x / 2;
        std::cout << left_bound << std::endl;
        float right_bound = this->Width / 10.0f - Player->Size.x / 2.0f;
        if (this->Keys[GLFW_KEY_A] && !this->KeysProcessed[GLFW_KEY_A])
        {
            x = x - velocity;
            if (x > left_bound)
            {
                if (Ball->Stuck == true)
                {
                    float x_ball = Ball->gamebox.getbody()->GetPosition().x;
                    float y_ball = Ball->gamebox.getbody()->GetPosition().y;
                    x_ball = x_ball - velocity;
                    Ball->gamebox.getbody()->SetTransform(b2Vec2(x_ball, y_ball), 0.0f);
                }
                Player->gamebox.getbody()->SetTransform(b2Vec2(x, y), 0.0f);
            }
            else
            {
                Player->gamebox.getbody()->SetTransform(b2Vec2(Player->Size.x / 2.0f, y), 0.0f);
            }
                

        }
        if (this->Keys[GLFW_KEY_D] && !this->KeysProcessed[GLFW_KEY_D])
        {
            x = x + velocity;
            if (x < right_bound)
            {
                Player->gamebox.getbody()->SetTransform(b2Vec2(x, y), 0.0f);
                if (Ball->Stuck == true)
                {
                    float x_ball = Ball->gamebox.getbody()->GetPosition().x;
                    float y_ball = Ball->gamebox.getbody()->GetPosition().y;
                    x_ball = x_ball + velocity;
                    Ball->gamebox.getbody()->SetTransform(b2Vec2(x_ball, y_ball), 0.0f);
                }
            }
            else
            {
                Player->gamebox.getbody()->SetTransform(b2Vec2(right_bound, y), 0.0f);
            }
                

        }
        if (this->Keys[GLFW_KEY_SPACE] && !this->KeysProcessed[GLFW_KEY_SPACE] && Ball->Stuck)
        {
            Ball->Stuck = false;
            float time = glfwGetTime();
            float vx = 40.0f * sin(time);
            float vy = -40.0f;
            Ball->gamebox.getbody()->SetLinearVelocity(b2Vec2(vx, vy));

        }

    }
}

void Game::Render()
{
    if (this->State == GAME_ACTIVE || this->State == GAME_MENU || this->State == GAME_WIN || this->State == GAME_LOST)
    {
        Effects->BeginRender();
        // draw background
        Texture2D mytex = ResourceManager::GetTexture("background");
        Renderer->DrawSprite(mytex,
            glm::vec2(0.0f, 0.0f), glm::vec2(this->Width/10.0f, this->Height/10.0f), 0.0f
        );
        
        // draw level
        RenderingLevel.Draw(*Renderer);
        
        // draw player
        
        Player->Draw(*Renderer);
        block->Draw(*Renderer);
        //Ground->Draw(*Renderer);
        Particles->Draw();
        Ball->Draw(*Renderer);
        ballLight->Draw();
        //std::cout << Ball->gamebox.getangle() << std::endl;
        for (PowerUp*& powerUp : this->PowerUps)
            if (!powerUp->gamebox.Destroyed)
            {
                powerUp->Draw(*Renderer);
            }
        
        Effects->EndRender();
        Effects->Render(glfwGetTime());
        std::stringstream ss; 
        ss << this->Lives;
        Text->RenderText("Lives:" + ss.str(), 5.0f, 5.0f, 1.0f);
        
        
    }

    if (this->State == GAME_MENU)
    {
        Text->RenderText("Press ENTER to start", 250.0f, this->Height / 2.0f, 1.0f);
        Text->RenderText("Press W or S to select level", 245.0f, this->Height / 2.0f + 20.0f, 0.75f);
        Text->RenderText("Press A or D to move paddle", 245.0f, this->Height / 2.0f + 40.0f, 0.75f);
    }
    if (this->State == GAME_WIN)
    {
        Text->RenderText("You WON!!!", 320.0f, this->Height / 2.0f - 20.0f, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        Text->RenderText("Press ENTER to retry or ESC to quit", 130.0f, this->Height / 2.0f, 1.0f, glm::vec3(1.0f, 1.0f, 0.0f));
    }
    if (this->State == GAME_LOST)
    {
        Texture2D mytex = ResourceManager::GetTexture("cai");
        Renderer->DrawSprite(mytex,
            glm::vec2(0.0f, 0.0f), glm::vec2(this->Width / 10.0f, this->Height / 10.0f), 0.0f
        );
        Text->RenderText("Press ENTER to start", 250.0f, this->Height / 2.0f + 60.0f, 1.0f);
        Text->RenderText("Press W or S to select level", 245.0f, this->Height / 2.0f + 80.0f, 0.75f);
        Text->RenderText("Press A or D to move paddle", 245.0f, this->Height / 2.0f + 100.0f, 0.75f);
    }
    
}

void Game::ResetLevel()
{
    if (this->Level == 0)
        RenderingLevel.Load(myworld.get(), "one.lvl", this->Width / 10.0f, this->Height / 2 / 10.0f);
    else if (this->Level == 1)
        RenderingLevel.Load(myworld.get(), "two.lvl", this->Width / 10.0f, this->Height / 2 / 10.0f);
    else if (this->Level == 2)
        RenderingLevel.Load(myworld.get(), "three.lvl", this->Width / 10.0f, this->Height / 2 / 10.0f);
    else if (this->Level == 3)
        RenderingLevel.Load(myworld.get(), "four.lvl", this->Width / 10.0f, this->Height / 2 / 10.0f);
    else if (this->Level == 4)
        RenderingLevel.Load(myworld.get(), "five.lvl", this->Width / 10.0f, this->Height / 2 / 10.0f);
    this->Lives = 3;
}

void Game::ResetPlayer()
{
    // reset player/ball stats
    Player->Size = PLAYER_SIZE;
    //Player->Position = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, this->Height - PLAYER_SIZE.y);
    Player->gamebox.getbody()->SetTransform(b2Vec2(this->Width / 2.0f / 10.0f , this->Height / 10.0f - PLAYER_SIZE.y / 2), Player->gamebox.getbody()->GetAngle());
    Ball->gamebox.getbody()->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
    Ball->gamebox.getbody()->SetTransform(b2Vec2(this->Width / 2.0f / 10.0f, this->Height / 10.0f - PLAYER_SIZE.y  - BALL_RADIUS), Player->gamebox.getbody()->GetAngle());
    Ball->Stuck = true;
    //Ball->Reset(Player->Position + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -(BALL_RADIUS * 2.0f)), INITIAL_BALL_VELOCITY);

    // also disable all active powerups
    Effects->Chaos = Effects->Confuse = false;
    Ball->PassThrough = Ball->Sticky = false;

    // Ball and gamebox are different, we need to set this to default.Ball powerup is for outside check, gamebox is for contactlistener
    Ball->gamebox.setpowertype("default");
    Player->Color = glm::vec3(1.0f);
    Ball->Color = glm::vec3(1.0f);
}

// powerups
bool IsOtherPowerUpActive(std::vector<PowerUp*>& powerUps, std::string type);

void Game::UpdatePowerUps(float dt)
{
    for (PowerUp*& powerUp : this->PowerUps)
    {
        if (powerUp->gamebox.isactivated())
        {
            std::cout << "powerup time" << powerUp->gamebox.getduration() << std::endl;
            powerUp->gamebox.setduration(powerUp->gamebox.getduration() - dt);
            if (powerUp->gamebox.getduration() <= 0.0f)
            {
                // remove powerup from list (will later be removed)
                powerUp->gamebox.setisactivated(false);
                // deactivate effects
                if (powerUp->Type == "sticky")
                {
                    if (!IsOtherPowerUpActive(this->PowerUps, "sticky"))
                    {	// only reset if no other PowerUp of type sticky is active
                        Ball->Sticky = false;
                        Ball->gamebox.setpowertype("default");
                        Player->Color = glm::vec3(1.0f);
                    }
                }
                else if (powerUp->Type == "pass-through")
                {
                    if (!IsOtherPowerUpActive(this->PowerUps, "pass-through"))
                    {	// only reset if no other PowerUp of type pass-through is active
                        Ball->PassThrough = false;
                        Ball->Color = glm::vec3(1.0f);
                        Ball->gamebox.setpowertype("default");
                        for (GameObject*& tile : RenderingLevel.Bricks)
                            if (!tile->gamebox.Destroyed)
                            {
                                tile->gamebox.getbody()->GetFixtureList()[0].SetSensor(false);
                            }
                    }
                }
                else if (powerUp->Type == "pad-size-increase")
                {
                    if (!IsOtherPowerUpActive(this->PowerUps, "pad-size-increase"))
                    {	// only reset if no other PowerUp of type pass-through is active
                        Ball->gamebox.setpowertype("default");
                        glm::vec2 playerp = Player->Position;
                        //Player size changed we need to change the size in Player
                        Player->gamebox.CreateKinematicBody(myworld.get(), playerp.x, playerp.y, PLAYER_SIZE.x / 2, PLAYER_SIZE.y / 2, &playerstate, playerEntity, playerMask);
                        Player->Size = glm::vec2(PLAYER_SIZE.x, PLAYER_SIZE.y);
                    }
                }
                else if (powerUp->Type == "confuse")
                {
                    if (!IsOtherPowerUpActive(this->PowerUps, "confuse"))
                    {	// only reset if no other PowerUp of type confuse is active
                        Ball->gamebox.setpowertype("default");
                        Effects->Confuse = false;
                    }
                }
                else if (powerUp->Type == "chaos")
                {
                    if (!IsOtherPowerUpActive(this->PowerUps, "chaos"))
                    {	// only reset if no other PowerUp of type chaos is active
                        Ball->gamebox.setpowertype("default");
                        Effects->Chaos = false;
                    }
                }
            }
        }
    }
    // Remove all PowerUps from vector that are destroyed AND !activated (thus either off the map or finished)
    // Note we use a lambda expression to remove each PowerUp which is destroyed and not activated
    this->PowerUps.erase(std::remove_if(this->PowerUps.begin(), this->PowerUps.end(),
        [](PowerUp*& powerUp) { return powerUp->gamebox.Destroyed && !powerUp->gamebox.isactivated(); }
    ), this->PowerUps.end());
}

bool ShouldSpawn(unsigned int chance)
{
    unsigned int random = rand() % chance;
    return random == 0;
}

void Game::SpawnPowerUps(box& box)
{
    glm::vec2 position = glm::vec2(box.getposition().x, box.getposition().y);
    // dont need speed change
    /*
    if (ShouldSpawn(10)) // 1 in 75 chance
    {
        PowerUp* power = new PowerUp(myworld.get(), "speed", glm::vec3(0.5f, 0.5f, 1.0f), 0.0f, position, ResourceManager::GetTexture("powerup_speed"));
        power->gamebox.getbody()->SetLinearVelocity(b2Vec2(0.0f, 5.0f));
        this->PowerUps.push_back(power);
    }
    */
        
    if (ShouldSpawn(70))
    {
        PowerUp* power = new PowerUp(myworld.get(), "sticky", glm::vec3(1.0f, 0.5f, 1.0f), 10.0f, position, ResourceManager::GetTexture("powerup_sticky"));
        power->gamebox.getbody()->SetLinearVelocity(b2Vec2(0.0f, 10.0f));
        this->PowerUps.push_back(power);
    }
    if (ShouldSpawn(70))
    {
        PowerUp* power = new PowerUp(myworld.get(), "pass-through", glm::vec3(0.5f, 1.0f, 0.5f), 5.0f, position, ResourceManager::GetTexture("powerup_passthrough"));
        power->gamebox.getbody()->SetLinearVelocity(b2Vec2(0.0f, 10.0f));
        this->PowerUps.push_back(power);
    }
    if (ShouldSpawn(70))
    {
        PowerUp* power = new PowerUp(myworld.get(), "pad-size-increase", glm::vec3(1.0f, 0.6f, 0.4), 10.0f, position, ResourceManager::GetTexture("powerup_increase"));
        power->gamebox.getbody()->SetLinearVelocity(b2Vec2(0.0f, 10.0f));
        this->PowerUps.push_back(power);
    }
    if (ShouldSpawn(20)) // Negative powerups should spawn more often
    {
        PowerUp* power = new PowerUp(myworld.get(), "confuse", glm::vec3(1.0f, 0.3f, 0.3f), 5.0f, position, ResourceManager::GetTexture("powerup_confuse"));
        power->gamebox.getbody()->SetLinearVelocity(b2Vec2(0.0f, 10.0f));
        this->PowerUps.push_back(power);
    }
    if (ShouldSpawn(20))
    {
        PowerUp* power = new PowerUp(myworld.get(), "chaos", glm::vec3(0.9f, 0.25f, 0.25f), 5.0f, position, ResourceManager::GetTexture("powerup_chaos"));
        power->gamebox.getbody()->SetLinearVelocity(b2Vec2(0.0f, 10.0f));
        this->PowerUps.push_back(power);
    }
}

void Game::ActivatePowerUp(box& box)
{
    // no need to change velocity
    /*
    if (box.getpowertype() == "speed")
    {
        b2Vec2 vel = b2Vec2(Ball->gamebox.getbody()->GetLinearVelocity().x * 2, Ball->gamebox.getbody()->GetLinearVelocity().y * 2);
        Ball->gamebox.getbody()->SetLinearVelocity(vel);
    }
    */
    if (box.getpowertype() == "sticky")
    {
        
        Ball->Sticky = true;
        Ball->gamebox.setpowertype("sticky");
        Player->Color = glm::vec3(1.0f, 0.5f, 1.0f);
    }
    else if (box.getpowertype() == "pass-through")
    {
        Ball->PassThrough = true;
        Ball->gamebox.setpowertype("pass-through");
        Ball->Color = glm::vec3(1.0f, 0.5f, 0.5f);
    }
    else if (box.getpowertype() == "pad-size-increase")
    {
        glm::vec2 playerp = Player->Position;
        Ball->gamebox.setpowertype("pad-size-increase");
        //Player size changed we need to change the size in Player
        Player->gamebox.CreateKinematicBody(myworld.get(), playerp.x, playerp.y, PLAYER_SIZE.x, PLAYER_SIZE.y/2, &playerstate, playerEntity, playerMask);
        Player->Size = glm::vec2(PLAYER_SIZE.x * 2, PLAYER_SIZE.y);
    }
    else if (box.getpowertype() == "confuse")
    {
        Ball->gamebox.setpowertype("confuse");
        if (!Effects->Chaos)
            Effects->Confuse = true; // only activate if chaos wasn't already active
    }
    else if (box.getpowertype() == "chaos")
    {
        Ball->gamebox.setpowertype("chaos");
        if (!Effects->Confuse)
            Effects->Chaos = true;
    }
}

bool IsOtherPowerUpActive(std::vector<PowerUp*>& powerUps, std::string type)
{
    // Check if another PowerUp of the same type is still active
    // in which case we don't disable its effect (yet)
    for (PowerUp*& powerUp : powerUps)
    {
        if (powerUp->gamebox.isactivated())
            if (powerUp->Type == type)
                return true;
    }
    return false;
}