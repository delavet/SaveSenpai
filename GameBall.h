#pragma once
#include "Box2D\Box2D.h"
class GameBall
{
public:
	GameBall(b2Body* _b, int _t, float _r);
	~GameBall();
	b2Body* ball;
	int type;//red or black
	float radius;//in physical world size
};


