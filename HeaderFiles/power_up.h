/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef POWER_UP_H
#define POWER_UP_H
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "game_object.h"
#include <box2d/box2d.h>


// The size of a PowerUp block
const glm::vec2 POWERUP_SIZE(6.0f, 2.0f);


// PowerUp inherits its state and rendering functions from
// GameObject but also holds extra information to state its
// active duration and whether it is activated or not. 
// The type of PowerUp is stored as a string.
class PowerUp : public GameObject
{
public:
    // powerup state
    BodyState powerupstate = BodyState::POWER;
    std::string Type;
    // constructor
    PowerUp(b2World* world,std::string type, glm::vec3 color, float duration, glm::vec2 position, Texture2D texture)
        : GameObject(world, position, POWERUP_SIZE, texture, &powerupstate, color), Type(type)
    {
        uint16 powerupEntity = EntityCategory::POWER;
        uint16 powerupMask = EntityCategory::WALL | EntityCategory::PADDLE;
        gamebox.setduration(duration);
        gamebox.setpowertype(type);
        gamebox.setisactivated(false);
        gamebox.CreateDynamicBody(world, Position.x, Position.y, POWERUP_SIZE.x / 2, POWERUP_SIZE.y / 2, State, powerupEntity, powerupMask);
    }
};

#endif