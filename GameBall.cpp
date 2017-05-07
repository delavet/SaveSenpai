#include "stdafx.h"
#include "GameBall.h"


GameBall::GameBall(b2Body* _b, int _t, float _r)
{
	ball = _b;
	type = _t;
	radius = _r;
}


GameBall::~GameBall()
{
}
