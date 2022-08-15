/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "game_level.h"
#include "game_object.h"
#include "power_up.h"
#include <box2d/box2d.h>
#include <memory>
#include "ball_object.h"

// Represents the current state of the game
enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN,
    GAME_LOST
};

// Represents the four possible (collision) directions
enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

typedef std::tuple<bool, Direction, glm::vec2> Collision;

const glm::vec2 INITIAL_BALL_VELOCITY(10.0f, -35.0f);
// Radius of the ball object
const float BALL_RADIUS = 1.25f;
// Initial size of the player paddle
extern const glm::vec2 PLAYER_SIZE;
// Initial velocity of the player paddle
extern const float PLAYER_VELOCITY;
const float POWER_VELOCITY(20.0f);

// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class Game
{
public:
    // game state
    GameState               State;
    bool                    Keys[1024];
    bool                    KeysProcessed[1024];
    unsigned int            Width, Height;
    std::vector<GameLevel>  Levels;
    std::vector<PowerUp*>    PowerUps;
    GameLevel               RenderingLevel;
    unsigned int            Level;
    unsigned int Lives;
    std::unique_ptr<b2World> myworld;
    GameObject* Player;
    BallObject* Ball;
    GameObject* BrightBlock;

    // constructor/destructor
    Game(unsigned int width, unsigned int height);
    ~Game();
    // initialize game state (load all shaders/textures/levels)
    void Init();
    // game loop
    void ProcessInput(float dt);
    void Update(float dt);
    void Render();
    void DoCollisions();
    // reset
    void ResetLevel();
    void ResetPlayer();
    // powerups
    void SpawnPowerUps(box& box);
    void UpdatePowerUps(float dt);
    void ActivatePowerUp(box& box);

    // box2d
    void Initwolrd();

};

#endif
