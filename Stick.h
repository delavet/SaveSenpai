#pragma once
#include "Box2D\Box2D.h"
class Stick
{
public:
	Stick(b2Body* _b, float _w, float _h);
	~Stick();
	b2Body* rect;
	float width;
	float height;
};



