#include "box.h"
box::box(std::string type, float duration, bool activated, bool sensor)
{
	_Type = type;
	_Duration = duration;
	_Activated = activated;
	IsSolid = false;
	Destroyed = false;
	_body = nullptr;
	_bodystate = BodyState::DEFAULT;
	_sensor = sensor;
	boxdef.userData.pointer = reinterpret_cast<uintptr_t>(this);
}
box::~box()
{

}
void box::CreateStaticBody(b2World* myworld, float x, float y, float hwidth, float hheight, BodyState* state, uint16 categoryBits, uint16 maskBits)
{
	if (_body != nullptr)
	{
		myworld->DestroyBody(_body);
	}
	boxdef.type = b2_staticBody;
	boxdef.position.Set(x + hwidth, y + hheight);
	b2Body* groundBody = myworld->CreateBody(&boxdef);
	b2PolygonShape groundBox;
	groundBox.SetAsBox(hwidth, hheight);

	b2FixtureDef fixturedef;
	fixturedef.shape = &groundBox;
	fixturedef.isSensor = _sensor;
	fixturedef.filter.categoryBits = categoryBits;
	fixturedef.filter.maskBits = maskBits;
	groundBody->CreateFixture(&groundBox, 0.0f);
	_body = groundBody;
	_bodystate = *state;
}
void box::CreateDynamicBody(b2World* myworld, float x, float y, float hwidth, float hheight, BodyState* state, uint16 categoryBits, uint16 maskBits)
{
	if (_body != nullptr)
	{
		myworld->DestroyBody(_body);
	}
	
	boxdef.type = b2_dynamicBody;
	boxdef.position.Set(x + hwidth, y + hheight);
	boxdef.userData.pointer = reinterpret_cast<uintptr_t>(this);
	b2Body* body = myworld->CreateBody(&boxdef);
	b2PolygonShape box;
	box.SetAsBox(hwidth, hheight);

	b2FixtureDef fixturedef;
	fixturedef.isSensor = _sensor;
	fixturedef.shape = &box;
	fixturedef.density = 1.0f;
	fixturedef.friction = 0.0f;
	fixturedef.restitution = 0.0f;
	fixturedef.filter.categoryBits = categoryBits;
	fixturedef.filter.maskBits = maskBits;
	body->CreateFixture(&fixturedef);
	_body = body;
	_bodystate = *state;
}
void box::CreateKinematicBody(b2World* myworld, float x, float y, float hwidth, float hheight, BodyState* state, uint16 categoryBits, uint16 maskBits)
{
	if (_body != nullptr)
	{
		myworld->DestroyBody(_body);
	}
	boxdef.type = b2_kinematicBody;
	boxdef.position.Set(x + hwidth, y + hheight);
	boxdef.userData.pointer = reinterpret_cast<uintptr_t>(this);
	b2Body* body = myworld->CreateBody(&boxdef);
	b2PolygonShape box;
	box.SetAsBox(hwidth, hheight);

	b2FixtureDef fixturedef;
	fixturedef.isSensor = _sensor;
	fixturedef.shape = &box;
	fixturedef.filter.categoryBits = categoryBits;
	fixturedef.filter.maskBits = maskBits;
	body->CreateFixture(&fixturedef);
	_body = body;
	_bodystate = *state;
}
void box::CreateDynamicCircleBody(b2World* myworld, float x, float y, float hwidth, float hheight, BodyState* state, uint16 categoryBits, uint16 maskBits)
{
	if (_body != nullptr)
	{
		myworld->DestroyBody(_body);
	}

	boxdef.type = b2_dynamicBody;
	boxdef.position.Set(x + hwidth, y + hheight);
	boxdef.userData.pointer = reinterpret_cast<uintptr_t>(this);
	b2Body* body = myworld->CreateBody(&boxdef);
	b2CircleShape circle;
	circle.m_p.Set(0.0f, 0.0f);
	circle.m_radius = hwidth;

	b2FixtureDef fixturedef;
	fixturedef.isSensor = _sensor;
	fixturedef.shape = &circle;
	fixturedef.density = 1.0f;
	fixturedef.friction = 0.0f;
	fixturedef.restitution = 1.0f;
	fixturedef.filter.categoryBits = categoryBits;
	fixturedef.filter.maskBits = maskBits;
	body->CreateFixture(&fixturedef);
	_body = body;
	_bodystate = *state;
}

b2Vec2 box::getposition()
{
	return _body->GetPosition();
}

float box::getangle()
{
	return _body->GetAngle();
}

b2Body* box::getbody()
{
	return _body;
}

BodyState box::getbodystate()
{
	return _bodystate;
}

void box::setsensor(bool sensor)
{
	_sensor = sensor;
}

void box::setpowertype(std::string type)
{
	_Type = type;
}
void box::setduration(float duration)
{
	_Duration = duration;
}
void box::setisactivated(bool activated)
{
	_Activated = activated;
}

bool box::getsensor()
{
	return _sensor;
}
std::string box::getpowertype()
{
	return _Type;
}
float box::getduration()
{
	return _Duration;
}
bool box::isactivated()
{
	return _Activated;
}


