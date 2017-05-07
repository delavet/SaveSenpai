#pragma once

#ifndef UTIL_H
#define UTIL_H

#define MAX_LOADSTRING 100



// 全局变量: 
HINSTANCE hInst = 0;                                // 当前实例
HWND g_hWnd = 0;
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名
ULONG_PTR m_gdiplusToken;

//物理世界相关全局变量,贾神定的
static float ground_gravity = -10;
const float GLOBAL_HEIGHT = 200;
const float GLOBAL_WIDTH = 200;
const int MOTION_LEFT = 1;
const int MOTION_RIGHT = 2;
const int MOTION_DOWN = 3;


// To mark if the game is processing
enum GAME_STATE { PLAYING, WAITING, STARTING };
int GAME = WAITING;

PhysicWorld box2DWorld;
MyBodysStruct MyBodys;

void applyMotion(int motion, int force) {
	if (GAME == STARTING)
		return;
	if (motion == MOTION_DOWN) {
		if (GAME == WAITING) {
			GAME = STARTING;
		}
		else if (GAME == PLAYING) {
			GAME = WAITING;
		}
	}
}

#endif