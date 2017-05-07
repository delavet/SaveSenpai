#pragma once

#ifndef LEAP_MOTION_H
#define LEAP_MOTION_H

#include <Leap.h>
#include <mutex>

class LeapMotionInterface {
private:
	std::mutex mtx;
	Leap::Gesture gesture;
public:
	LeapMotionInterface() {
		gesture = NULL;
	}
	int setGesture(Leap::Gesture g) {
		mtx.lock();
		gesture = g;
		mtx.unlock();
		return 0;
	}
	Leap::Gesture getGesture() {
		mtx.lock();
		Leap::Gesture g = gesture;
		mtx.unlock();
		return g;
	}
	float getSwipeSpeed() {
		mtx.lock();
		if (gesture.type() != gesture.TYPE_SWIPE)
			return -1;
		Leap::SwipeGesture sg = Leap::SwipeGesture(gesture);
		mtx.unlock();
		float speed = sg.speed();
		return speed;
	}
	Leap::Vector getSwipeDirection() {
		mtx.lock();
		if (gesture.type() != gesture.TYPE_SWIPE)
			return Leap::Vector(0, 0, 0);
		Leap::SwipeGesture sg = Leap::SwipeGesture(gesture);
		mtx.unlock();
		Leap::Vector direction = sg.direction();
		return direction;
		float x = direction.x; if (x != 0) x = x > 0 ? 1 : -1;
		float y = direction.y; if (y != 0) y = y > 0 ? 1 : -1;
		float z = direction.z; if (z != 0) z = z > 0 ? 1 : -1;
	}
};

LeapMotionInterface LMI;

#endif