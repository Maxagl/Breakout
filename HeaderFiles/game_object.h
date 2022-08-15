/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "sprite_renderer.h"
#include "box.h"
#include <box2d/box2d.h>


// Container object for holding all state relevant for a single
// game object entity. Each object in the game likely needs the
// minimal of state as described within GameObject.

class GameObject
{
public:
    // object state
    glm::vec2   Position, Size;
    glm::vec3   Color;
    float       Rotation;

    // render state
    Texture2D   Sprite;

    //create box2d box
    box gamebox;
    BodyState *State;
    


    // constructor(s)
    GameObject();
    GameObject(b2World* myworld, glm::vec2 pos, glm::vec2 size, Texture2D sprite, BodyState* state, glm::vec3 color = glm::vec3(1.0f), uint16 categoryBits = 0x0001, uint16 maskBits = 0x0001);
    // draw sprite
    virtual void Draw(SpriteRenderer& renderer);

};

#endif
