#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "GameLevel.h"
#include "BallObject.h"
#include "PowerUp.h"

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

// Initial size of the player paddle
const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);
// Initial velocity of the player paddle
const float PLAYER_VELOCITY(500.0f);
using Collision = std::tuple<bool, Direction, glm::vec2>;

class Game
{
public:
    // game state
    GameState               State;
    bool                    Keys[1024];
    bool                    KeysProcessed[1024];
    unsigned int            Width, Height;
    std::vector<GameLevel>  Levels;
    std::vector<PowerUp>    PowerUps;
    unsigned int            Level;
    unsigned int            Lives;
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
    void SpawnPowerUps(GameObject& block);
    void UpdatePowerUps(float dt);
    bool IsOtherPowerUpActive(std::vector<PowerUp>& powerUps, std::string type);
    bool ShouldSpawn(unsigned int chance);
    void ActivatePowerUp(PowerUp& powerUp);

    Collision CheckCollision(BallObject& one, GameObject& two);
    bool CheckCollision(GameObject& one, GameObject& two);
    Direction VectorDirection(glm::vec2 target);
};

