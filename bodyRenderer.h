#pragma once
#include <gdiplus.h>
#include "Box2D\Box2D.h"
#include "Stick.h"
#include "GameBall.h"

class bodyRenderer
{
public:
	bodyRenderer();
	~bodyRenderer();

	static int Xcoordinate_phy2vir(int x);
	static int Ycoordinate_phy2vir(int y);
	static int Size_transform(int dat);
	static void RenderStick(Gdiplus::Graphics* graphic,Stick* stick);
	static void RenderBall(Gdiplus::Graphics* graphic, GameBall* ball);
};

