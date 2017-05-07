#include "stdafx.h"
#include "Stick.h"


Stick::Stick(b2Body* _b, float _w, float _h)
{
	rect = _b;
	width = _w;
	height = _h;
}


Stick::~Stick()
{
}
