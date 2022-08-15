#pragma once
#include<box2d/box2d.h>
#include<box2d/b2_world_callbacks.h>
#include "box.h"
#include<vector>
#include<iostream>
#include <irrklang/irrKlang.h>
using namespace irrklang;

extern std::vector<box*> boxstoDestory_brick;
std::vector<box*> boxstoDestory_brick;

extern std::vector<box*> boxstoDestory_powerup;
std::vector<box*> boxstoDestory_powerup;

extern ISoundEngine* SoundEngine;
ISoundEngine* SoundEngine = createIrrKlangDevice();
extern PostProcessor* Effects;
PostProcessor* Effects;
extern float ShakeTime;
float ShakeTime = 0.0f;
extern bool PADDLE_BALL;
bool PADDLE_BALL = false;

class Contactlistener:public b2ContactListener
{
public:
	void BeginContact(b2Contact* contact)
	{
		b2BodyUserData bodyUserDataA = contact->GetFixtureA()->GetBody()->GetUserData();
		//box* myboxA = static_cast<box*>(reinterpret_cast<void*>(bodyUserDataA.pointer));
		box* myboxA = reinterpret_cast<box*>(bodyUserDataA.pointer);

		b2BodyUserData bodyUserDataB = contact->GetFixtureB()->GetBody()->GetUserData();
		box* myboxB = reinterpret_cast<box*>(bodyUserDataB.pointer);

		if (myboxA->getbodystate() == BodyState::POWER and myboxB->getbodystate() == BodyState::PADDLE)
		{
			SoundEngine->play2D("audio/bleep.mp3", false);
			boxstoDestory_powerup.push_back(myboxA);
		}
		if (myboxA->getbodystate() == BodyState::PADDLE and myboxB->getbodystate() == BodyState::POWER)
		{
			SoundEngine->play2D("audio/bleep.mp3", false);
			boxstoDestory_powerup.push_back(myboxB);
		}
		
		if (myboxA->getpowertype() == "sticky" || myboxB->getpowertype() == "sticky")
		{
			if (myboxA->getbodystate() == BodyState::BALL and myboxB->getbodystate() == BodyState::PADDLE)
			{
				PADDLE_BALL = true;
				SoundEngine->play2D("audio/bleep.wav", false);
			}
			if (myboxA->getbodystate() == BodyState::PADDLE and myboxB->getbodystate() == BodyState::BALL)
			{
				PADDLE_BALL = true;
				SoundEngine->play2D("audio/bleep.wav", false);
			}
		}

	}

	void EndContact(b2Contact* contact)
	{
		b2BodyUserData bodyUserDataA = contact->GetFixtureA()->GetBody()->GetUserData();
		//box* myboxA = static_cast<box*>(reinterpret_cast<void*>(bodyUserDataA.pointer));
		box* myboxA = reinterpret_cast<box*>(bodyUserDataA.pointer);

		b2BodyUserData bodyUserDataB = contact->GetFixtureB()->GetBody()->GetUserData();
		box* myboxB = reinterpret_cast<box*>(bodyUserDataB.pointer);
		if (myboxA->getbodystate() == BodyState::BALL and myboxB->getbodystate() == BodyState::BRICK)
		{
			SoundEngine->play2D("audio/bleep.mp3", false);
			boxstoDestory_brick.push_back(myboxB);
		}
		if (myboxA->getbodystate() == BodyState::BRICK and myboxB->getbodystate() == BodyState::BALL)
		{
			SoundEngine->play2D("audio/bleep.mp3", false);
			boxstoDestory_brick.push_back(myboxA);
		}
		if (myboxA->getbodystate() == BodyState::BALL and myboxB->getbodystate() == BodyState::SOLIDBRICK)
		{
			ShakeTime = 0.05f;
			Effects->Shake = true;
			SoundEngine->play2D("audio/solid.wav", false);
		}
		if (myboxA->getbodystate() == BodyState::SOLIDBRICK and myboxB->getbodystate() == BodyState::BALL)
		{
			ShakeTime = 0.05f;
			Effects->Shake = true;
			SoundEngine->play2D("audio/solid.wav", false);
		}
		
		if (myboxA->getpowertype() != "sticky" && myboxB->getpowertype() != "sticky")
		{
			if (myboxA->getbodystate() == BodyState::BALL and myboxB->getbodystate() == BodyState::PADDLE)
			{
				PADDLE_BALL = true;
				SoundEngine->play2D("audio/bleep.wav", false);
			}
			if (myboxA->getbodystate() == BodyState::PADDLE and myboxB->getbodystate() == BodyState::BALL)
			{
				PADDLE_BALL = true;
				SoundEngine->play2D("audio/bleep.wav", false);
			}
		}
		

	}

};

