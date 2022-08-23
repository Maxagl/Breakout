/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "game_level.h"

#include <fstream>
#include <sstream>
#include <iostream>

uint16 brickEntity = EntityCategory::BRICK;
uint16 brickMask = EntityCategory::WALL | EntityCategory::BALL;
void GameLevel::Load(b2World* world, const char* file, unsigned int levelWidth, unsigned int levelHeight)
{
    // clear old data
    
    for (GameObject*& tile : this->Bricks)
    {
        if(!tile->gamebox.Destroyed)
            world->DestroyBody(tile->gamebox.getbody());
    }
    this->Bricks.clear();
    // load from file
    unsigned int tileCode;
    GameLevel level;
    std::string line;
    std::ifstream fstream(file);
    std::vector<std::vector<unsigned int>> tileData;
    if (fstream)
    {
        while (std::getline(fstream, line)) // read each line from level file
        {
            std::istringstream sstream(line);
            std::vector<unsigned int> row;
            while (sstream >> tileCode) // read each word separated by spaces
                row.push_back(tileCode);
            tileData.push_back(row);
        }
        if (tileData.size() > 0)
            this->init(world, tileData, levelWidth, levelHeight);
    }
}

void GameLevel::Draw(SpriteRenderer& renderer)
{
    int i = 0;
    for (GameObject*& tile : this->Bricks)
        if (!tile->gamebox.Destroyed)
        {
            tile->Draw(renderer);
            i++;
        }
    //std::cout << i << std::endl;
}

bool GameLevel::IsCompleted()
{
    for (GameObject*& tile : this->Bricks)
        if (!tile->gamebox.IsSolid && !tile->gamebox.Destroyed)
            return false;
    return true;
}

void GameLevel::init(b2World* world, std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight)
{
    // calculate dimensions
    unsigned int height = tileData.size();
    unsigned int width = tileData[0].size(); // note we can index vector at [0] since this function is only called if height > 0
    float unit_width = levelWidth / static_cast<float>(width), unit_height = levelHeight / static_cast<float>(height);
    // initialize level tiles based on tileData
    for (unsigned int y = 0; y < height; ++y)
    {
        for (unsigned int x = 0; x < width; ++x)
        {
            // check block type from level data (2D level array)
            if (tileData[y][x] == 1) // solid
            {
                BodyState solid = BodyState::SOLIDBRICK;
                glm::vec2 pos(unit_width * x, unit_height * y);
                glm::vec2 size(unit_width, unit_height);
                GameObject* obj = new GameObject(world, pos, size, ResourceManager::GetTexture("block_solid"), &solid, glm::vec3(0.8f, 0.8f, 0.7f), brickEntity, brickMask);
                obj->gamebox.IsSolid = true;
                this->Bricks.push_back(obj);
            }
            else if (tileData[y][x] > 1)	// non-solid; now determine its color based on level data
            {
                BodyState brick = BodyState::BRICK;
                glm::vec3 color = glm::vec3(1.0f); // original: white
                
                if (tileData[y][x] == 2)
                    color = glm::vec3(0.2f, 0.6f, 1.0f);
                else if (tileData[y][x] == 3)
                    color = glm::vec3(0.0f, 0.7f, 0.0f);
                else if (tileData[y][x] == 4)
                    color = glm::vec3(0.8f, 0.8f, 0.4f);
                else if (tileData[y][x] == 5)
                    color = glm::vec3(1.0f, 0.0f, 0.0f);
                

                glm::vec2 pos(unit_width * x, unit_height * y);
                glm::vec2 size(unit_width, unit_height);
                GameObject* obj = new GameObject(world, pos, size, ResourceManager::GetTexture("block"), &brick, color, brickEntity, brickMask);
                this->Bricks.push_back(obj);
            }
       
        }
    }
}