/******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "ball_object.h"


BallObject::BallObject()
    : GameObject(), Radius(12.5f), Stuck(true) { }

BallObject::BallObject(b2World* world,glm::vec2 pos, float radius, Texture2D sprite, BodyState* state)
    : GameObject(world, pos, glm::vec2(radius * 2.0f, radius * 2.0f), sprite, state, glm::vec3(1.0f)), Radius(radius), Stuck(true), Sticky(false), PassThrough(false)
{
    uint16 ballEntity = EntityCategory::BALL;
    uint16 ballMask = EntityCategory::WALL | EntityCategory::BRICK | EntityCategory::PADDLE | EntityCategory::BLOCK;
    this->gamebox.CreateDynamicCircleBody(world, Position.x, Position.y, radius, radius, state, ballEntity, ballMask);
}


// resets the ball to initial Stuck Position (if ball is outside window bounds)
void BallObject::Reset(glm::vec2 position, glm::vec2 velocity)
{
    this->Position = position;
    this->Stuck = true;
    this->Sticky = true;
    this->PassThrough = false;

}