#pragma once
#include<box2d/box2d.h>
#include<iostream>

enum class BodyState {
	WALL,
	BALL,
	BRICK,
	SOLIDBRICK,
	PADDLE,
	POWER,
	BLOCK,
	DEFAULT,
};

enum EntityCategory {
	WALL = 0x0001,
	BALL = 0x0002,
	BRICK = 0x0004,
	PADDLE = 0x0006,
	POWER = 0x0008,
	BLOCK = 0x00010
};


class box
{
public:
	box(std::string type = "Default", float duration = 0.0f, bool activated = false, bool sensor = false);
	~box();
	bool        IsSolid;
	bool        Destroyed;

	b2BodyDef	boxdef;
	void CreateStaticBody(b2World* myworld, float x, float y, float hwidth, float hheight, BodyState* state, uint16 categoryBits = 0x0001, uint16 maskBits = 0xFFFF);
	void CreateDynamicBody(b2World* myworld, float x, float y, float hwidth, float hheight, BodyState* state, uint16 categoryBits = 0x0001, uint16 maskBits = 0xFFFF);
	void CreateKinematicBody(b2World* myworld, float x, float y, float hwidth, float hheight, BodyState* state, uint16 categoryBits = 0x0001, uint16 maskBits = 0xFFFF);
	void CreateDynamicCircleBody(b2World* myworld, float x, float y, float hwidth, float hheight, BodyState* state, uint16 categoryBits = 0x0001, uint16 maskBits = 0xFFFF);
	b2Vec2 getposition();
	float getangle();
	b2Body* getbody();
	BodyState getbodystate();

	void setpowertype(std::string type);
	void setduration(float duration);
	void setisactivated(bool activated);
	void setsensor(bool sensor);

	std::string getpowertype();
	float getduration();
	bool isactivated();
	bool getsensor();
	
	
private:
	b2Body*		_body;
	b2Vec2		_position;
	BodyState	_bodystate;
	std::string _Type;
	float       _Duration;
	bool        _Activated;
	bool		_sensor;

};

