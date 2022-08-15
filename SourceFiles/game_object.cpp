/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "../HeaderFiles/game_object.h"


GameObject::GameObject()
    : Position(0.0f, 0.0f), Size(1.0f, 1.0f), Color(1.0f), Rotation(0.0f), Sprite(), State(nullptr){}

GameObject::GameObject(b2World* myworld,glm::vec2 pos, glm::vec2 size, Texture2D sprite, BodyState* state, glm::vec3 color, uint16 categoryBits, uint16 maskBits)
    : Position(pos), Size(size), Color(color), Rotation(0.0f), Sprite(sprite), State(state)
{
	gamebox.CreateKinematicBody(myworld, Position.x, Position.y, Size.x / 2, Size.y / 2, State, categoryBits, maskBits);
    this->Position = glm::vec2(gamebox.getposition().x - this->Size.x / 2, gamebox.getposition().y - this->Size.y / 2);
}

void GameObject::Draw(SpriteRenderer& renderer)
{
    this->Position = glm::vec2(gamebox.getposition().x - this->Size.x/2, gamebox.getposition().y - this->Size.y/2);
    this->Rotation = gamebox.getangle();
    renderer.DrawSprite(this->Sprite, this->Position, this->Size, this->Rotation, this->Color);
}
