#pragma once
struct Vertex
{
	struct Position
	{
		float x;
		float y;
	}position;
	struct Texcoords
	{
		float x;
		float y;
	}texcoords;

	void setposition(float x, float y)
	{
		position.x = x;
		position.y = y;
	}
	void settexcoords(float x, float y)
	{
		texcoords.x = x;
		texcoords.y = y;
	}
};

struct Rect
{
	float positionx;
	float positiony;
	float hwidth;
	float hheight;

	void setRect(float x, float y, float w, float h)
	{
		positionx = x;
		positiony = y;
		hwidth = w;
		hheight = h;
	}
};

