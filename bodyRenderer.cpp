#include "stdafx.h"
#include "bodyRenderer.h"


bodyRenderer::bodyRenderer()
{
}


bodyRenderer::~bodyRenderer()
{
}

int bodyRenderer::Xcoordinate_phy2vir(int phy_x)
{
	return phy_x + 712;
}

int bodyRenderer::Ycoordinate_phy2vir(int phy_y)
{
	return 350 - phy_y;
}

int bodyRenderer::Size_transform(int data)
{
	//return 3 * data;
	return data;
}

void bodyRenderer::RenderStick(Gdiplus::Graphics* graphic, Stick* stick) {
	Gdiplus::Image image(L"stick.png");
	b2Vec2 posVec = stick->rect->GetPosition();//physical vector
	int posX = bodyRenderer::Xcoordinate_phy2vir(posVec.x);
	int posY = bodyRenderer::Ycoordinate_phy2vir(posVec.y);
	int width = bodyRenderer::Size_transform(stick->width);
	int height = bodyRenderer::Size_transform(stick->height);
	float angle = stick->rect->GetAngle() * 180 / 3.1415926;
	float a = posVec.x*posVec.x + posVec.y*posVec.y;
	graphic->TranslateTransform(posX, posY);
	graphic->RotateTransform(-angle);
	graphic->TranslateTransform(-posX, -posY);
	
	//graphic->DrawImage(&image, posX-width/2, posY-height/2, width, height);
	graphic->DrawImage(&image, posX-width/2, posY-height/2, width, height);
	//graphic->RotateTransform(angle);
	graphic->ResetTransform();
}

void bodyRenderer::RenderBall(Gdiplus::Graphics* graphic, GameBall* ball) {
	Gdiplus::Image red(L"red_ball.png");
	Gdiplus::Image black(L"black_ball.png");
	b2Vec2 posVec = ball->ball->GetPosition();
	int posX = bodyRenderer::Xcoordinate_phy2vir(posVec.x);
	int posY = bodyRenderer::Ycoordinate_phy2vir(posVec.y);
	int radius = bodyRenderer::Size_transform(ball->radius);
	if (ball->type == 0) {
		graphic->DrawImage(&red, posX - radius, posY - radius, 2 * radius, 2 * radius);
	}
	else {
		graphic->DrawImage(&black, posX - radius, posY - radius, 2 * radius, 2 * radius);
	}
}
