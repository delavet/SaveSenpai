#pragma once

#ifndef UTIL_H
#define UTIL_H

#define MAX_LOADSTRING 100



// ȫ�ֱ���: 
HINSTANCE hInst = 0;                                // ��ǰʵ��
HWND g_hWnd = 0;
WCHAR szTitle[MAX_LOADSTRING];                  // �������ı�
WCHAR szWindowClass[MAX_LOADSTRING];            // ����������
ULONG_PTR m_gdiplusToken;

//�����������ȫ�ֱ���,���񶨵�
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