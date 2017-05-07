#pragma once

#include "Box2D\Box2D.h"
#include "Stick.h"
#include <vector>
#include "GameBall.h"
#include <math.h>
const int MOTION_LEFT = 1;
const int MOTION_RIGHT = 2;
const int MOTION_DOWN = 3;
const int TYPE_RED = 0;
const int TYPE_BLACK = 1;
const int FORCE = 10000000;
struct MyBodysStruct {
	b2Body* centerCircle;
	Stick* stick;

	b2RevoluteJoint* joint;
	std::vector<GameBall> vec;

	
};
MyBodysStruct MyBodys;
enum GAME_STATE { PLAYING, WAITING, STARTING, FAILED };
int GAME = WAITING;
void applyMotion(int motion, int force) {
	if (GAME == STARTING)
		return;
	if (motion == MOTION_DOWN) {
		if (GAME == WAITING) {
			GAME = STARTING;
		}
		else if (GAME == FAILED) {
			if (force == -1) {
				// back to main menu
				GAME = WAITING;
			}
			else if (force == -2) {
				//restart
				GAME = STARTING;
			}
		}
	}
	if (motion == MOTION_LEFT && GAME==PLAYING) {
		float angle = MyBodys.stick->rect->GetAngle();
		b2Vec2 vForce = b2Vec2(sin(angle)*FORCE, -cos(angle)*FORCE);
		b2Vec2 temp = MyBodys.stick->rect->GetPosition();
		MyBodys.stick->rect->ApplyForce(vForce, MyBodys.stick->rect->GetPosition(),true);
	}
	if (motion == MOTION_RIGHT && GAME == PLAYING) {
		float angle = MyBodys.stick->rect->GetAngle();
		b2Vec2 vForce = b2Vec2(-sin(angle)*FORCE, cos(angle)*FORCE);
		b2Vec2 temp = MyBodys.stick->rect->GetPosition();
		MyBodys.stick->rect->ApplyForce(vForce, MyBodys.stick->rect->GetPosition(),true);
	}
}