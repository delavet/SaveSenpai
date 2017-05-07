#pragma once
#include "Box2D\Box2D.h"
#include "Stick.h"
#include <vector>
#include "GameBall.h"
class PhysicWorld
{
public:
	PhysicWorld();
	~PhysicWorld();
	b2AABB* worldAABB;
	b2Vec2 gravity;
	b2World world;


	
};

