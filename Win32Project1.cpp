
// Win32Project1.cpp : 定义应用程序的入口点。
//



#include "stdafx.h"
#include "Win32Project1.h"
#include "Box2D/Box2D.h"
#include "PhysicWorld.h"
#include "MyBodysStruct.h"
#include "bodyRenderer.h"
#include "Stick.h"
#include "GameBall.h"
#include <Leap.h>
#include <Windows.h>
#include <mmsystem.h>
#include <gdiplus.h>
#include <time.h>
#include <mutex>
#include <cmath>
#include <math.h>
#include <iostream>
#include <string>
#include <string.h>

using namespace Gdiplus;
using namespace Leap;


#define MAX_LOADSTRING 100



// 全局变量: 
HINSTANCE hInst = 0;                                // 当前实例
HWND g_hWnd = 0;
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名
ULONG_PTR m_gdiplusToken;

//物理世界相关全局变量,贾神定的
static float ground_gravity = -10;
const float GLOBAL_HEIGHT = 700;
const float GLOBAL_WIDTH = 400;

//[jlc]: gaming variable, HP and score.



int HP;
int score;


PhysicWorld box2DWorld;

Gdiplus::Image main(L"main.png");
Gdiplus::Image start(L"start.png");
Gdiplus::Image end(L"end.png");
Gdiplus::Image senpai(L"senpai.png");

class SampleListener : public Leap::Listener {
public:
	virtual void onConnect(const Leap::Controller&);
	virtual void onInit(const Leap::Controller&);
	virtual void onDisconnect(const Leap::Controller&);
	virtual void onExit(const Leap::Controller&);
	virtual void onFocusGained(const Leap::Controller&);
	virtual void onFocusLost(const Leap::Controller&);

	// main detect function
	virtual void onFrame(const Leap::Controller&);
};


void SampleListener::onConnect(const Leap::Controller& controller) {
	std::cout << "Connected" << std::endl;
	//controller.enableGesture(Gesture::TYPE_CIRCLE);
	controller.enableGesture(Leap::Gesture::TYPE_KEY_TAP);
	controller.enableGesture(Leap::Gesture::TYPE_SCREEN_TAP);
	controller.enableGesture(Leap::Gesture::TYPE_SWIPE);
}

void SampleListener::onInit(const Leap::Controller& controller) {
	std::cout << "Initialized" << std::endl;
}

void SampleListener::onDisconnect(const Leap::Controller& controller) {
	//Note: not dispatched when running in a debugger.  
	std::cout << "Disconnected" << std::endl;
}

void SampleListener::onExit(const Leap::Controller& controller) {
	std::cout << "Exited" << std::endl;
}

void SampleListener::onFocusGained(const Leap::Controller& controller) {
	std::cout << "Focus Gained" << std::endl;
}

void SampleListener::onFocusLost(const Leap::Controller& controller) {
	std::cout << "Focus Lost" << std::endl;
}

void SampleListener::onFrame(const Leap::Controller& controller) {
	const Leap::Frame frame = controller.frame();
	/*
	std::cout << "frame id: " << frame.id() << std::endl;
	std::cout << "frame timestamp: " << frame.timestamp() << std::endl;
	std::cout << "frame hands: " << frame.hands().count() << std::endl;
	std::cout << "frame fingers: " << frame.fingers().count() << std::endl;
	std::cout << "frame tools: " << frame.tools().count() << std::endl;
	std::cout << "frame gestures: " << frame.gestures().count() << std::endl;
	*/

	const Leap::GestureList gestures = frame.gestures();
	if (gestures.count()) {
		std::cout << "---------------------------------------" << std::endl;
		for (int i = 0; i < gestures.count(); i++) {
			Leap::Gesture gesture = gestures[i];
			switch (gesture.type()) {
			case Leap::Gesture::TYPE_SWIPE: {
				std::cout << "Catch SWIPE\t";
				Leap::SwipeGesture sg = Leap::SwipeGesture(gesture);
				Leap::Vector direction = sg.direction();
				float x = direction.x;

				int motion = 0;
				if (sg.state() == sg.STATE_UPDATE) {
					if (x < 0) {
						motion = MOTION_LEFT;
					}
					else if (x > 0) {
						motion = MOTION_RIGHT;
					}
				}
				float speed = sg.speed();
				speed = sqrt(speed);

				applyMotion(motion, (int)speed);
				break;
			}
			case Leap::Gesture::TYPE_KEY_TAP:
				//break;
			case Leap::Gesture::TYPE_SCREEN_TAP: {
				int motion = 3;
				Leap::HandList hl = gesture.hands();
				if (hl.count() != 1)
					return;
				Leap::Hand hand = hl[0];
				if (hand.isLeft())
					applyMotion(motion, -1);
				else if (hand.isRight())
					applyMotion(motion, -2);

				break;
			}
			default: break;
			}
		}
	}
}


// 此代码模块中包含的函数的前向声明: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
HWND                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void createBall(b2World *world) {
	b2BodyDef centerCircleBodyDef;
	centerCircleBodyDef.type = b2_dynamicBody;
	int posX = rand() % 300 - 150;
	centerCircleBodyDef.position.Set(posX, 350);
	b2Body* ball = world->CreateBody(&centerCircleBodyDef);
	b2CircleShape circle;
	circle.m_p.Set(0.0f, 0.0f);
	circle.m_radius = GLOBAL_WIDTH/18;
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &circle;
	fixtureDef.density = 0.1f;
	fixtureDef.friction = 0.3f;
	ball->CreateFixture(&fixtureDef);
	int type = rand() % 2;
	printf("create ball!\n");
	MyBodys.vec.push_back(GameBall(ball, type, circle.m_radius));
}

void gameInit() {
	//[jlc]: init HP and score.
	HP = 5;
	score = 0;

	srand(time(0));
	//circle in the center.
	b2BodyDef centerCircleBodyDef;
	centerCircleBodyDef.position.Set(0.0f, 0.0f);
	MyBodys.centerCircle = box2DWorld.world.CreateBody(&centerCircleBodyDef);
	b2CircleShape circle;
	circle.m_p.Set(0.0f, 0.0f);
	circle.m_radius = GLOBAL_WIDTH/18;
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &circle;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.0f;
	MyBodys.centerCircle->CreateFixture(&fixtureDef);

	//rect for rotating
	b2BodyDef rotateRectDef;
	rotateRectDef.type = b2_dynamicBody;
	rotateRectDef.position.Set(GLOBAL_WIDTH * 3 / 18, 0.0f);
	b2Body* rotateRect;
	rotateRect = box2DWorld.world.CreateBody(&rotateRectDef);
	b2PolygonShape polygon;
	polygon.SetAsBox(GLOBAL_WIDTH * 2 / 9, GLOBAL_WIDTH * 1 / 18);
	fixtureDef.shape = &polygon;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	rotateRect->CreateFixture(&fixtureDef);
	MyBodys.stick = new Stick(rotateRect, GLOBAL_WIDTH * 4 / 9, GLOBAL_WIDTH * 1 / 9);
	b2RevoluteJointDef jointDef;
	//joint
	jointDef.Initialize( MyBodys.stick->rect, MyBodys.centerCircle, MyBodys.centerCircle->GetWorldCenter());
	MyBodys.joint = (b2RevoluteJoint*)box2DWorld.world.CreateJoint(&jointDef);
	


}

void Logic(DWORD elapse) {
	switch (GAME)
	{
	case STARTING: {
		gameInit();
		GAME = PLAYING;
		break;
	}
	case WAITING: {
		// draw main menu
		break;
	}
	case PLAYING: {
		float32 timeStep = 1.0f / 60.0f;
		int32 velocityIterations = 6;
		int32 positionIterations = 2;

		box2DWorld.world.Step(timeStep, velocityIterations, positionIterations);
		b2Vec2 position = MyBodys.stick->rect->GetPosition();
		//printf("bar: %4.2f %4.2f\n", position.x, position.y);
		for (int i = 0; i < MyBodys.vec.size(); i++) {
			position = MyBodys.vec.at(i).ball->GetPosition();


			//[jlc]: remove the balls out of range, update HP and score.
			if (position.x<-GLOBAL_WIDTH / 2 || position.x>GLOBAL_WIDTH / 2
				|| position.y<-GLOBAL_HEIGHT / 2 || position.y>GLOBAL_HEIGHT / 2) {
				// RED ball goes down from the screen
				if (MyBodys.vec.at(i).type = TYPE_RED&&position.y < -GLOBAL_HEIGHT / 2) {
					score += 5;
				}
				if (MyBodys.vec.at(i).type == TYPE_BLACK || position.y < -GLOBAL_HEIGHT / 2) {
					HP -= 1;
				}
				box2DWorld.world.DestroyBody(MyBodys.vec.at(i).ball);
				MyBodys.vec.erase(MyBodys.vec.begin() + i);
				i--;
			}

			//printf("ball %d: %4.2f %4.2f\n", i, position.x, position.y);
		}
		if (HP <= 0) {
			//fail,destroy Everything
			//box2DWorld.world.DestroyJoint(MyBodys.joint);
			box2DWorld.world.DestroyBody(MyBodys.centerCircle);
			box2DWorld.world.DestroyBody(MyBodys.stick->rect);
			for (int i = 0; i < MyBodys.vec.size(); i++) {
				box2DWorld.world.DestroyBody(MyBodys.vec.at(i).ball);
				MyBodys.vec.erase(MyBodys.vec.begin() + i);
				i--;
			}
			GAME = FAILED;
		}
		if (elapse % 400<=45) {
			createBall(&(box2DWorld.world));
		}
		break;
	}
	default:
		break;
	}
	
}



void Render(HWND hwnd) {

	int gwidth, gheight;
	// TODO: 在此处添加使用 hdc 的任何绘图代码...
	//Gdiplus::Image main(L"main.png");

	Gdiplus::Image *image=NULL;
	Gdiplus::Image *senpai = NULL;
	HDC hdc = GetDC(hwnd);
	RECT rt;
	GetClientRect(hwnd, &rt);
	gwidth = rt.right - rt.left;
	gheight = rt.bottom - rt.top;
	HDC hMdc = CreateCompatibleDC(hdc);//创建内存dc
	HBITMAP hMbitmap = CreateCompatibleBitmap(hdc, gwidth, gheight);//创建内存绘图图片
	SelectObject(hMdc, hMbitmap);//关联dc和图片

								 //取得宽度和高度  
								 //width = image.GetWidth();
								 //height = image.GetHeight();

								 //绘图  
	Graphics graphics(hMdc);
	//graphics.DrawImage(&main, 0, 0, gwidth, gheight);

	switch (GAME)
	{
	case STARTING:
	{
		//image = Gdiplus::Image::FromFile(L"main.png");
		//graphics.DrawImage(image, 0, 0, gwidth, gheight);
	}break;
	case WAITING:
	{
		image = Gdiplus::Image::FromFile(L"start.png");
		graphics.DrawImage(image, 0, 0, gwidth, gheight);
	}break;
	case PLAYING:
	{
		image = Gdiplus::Image::FromFile(L"main.png");
		senpai= Gdiplus::Image::FromFile(L"senpai.png");
		graphics.DrawImage(image, 0, 0, gwidth, gheight);
		graphics.DrawImage(senpai, 700, 580, 100, 120);
		SolidBrush solidBrush(Color::Red);
		FontFamily fontFamily(L"Arial");
		Gdiplus::Font font(&fontFamily, 40, FontStyleRegular, UnitPixel);
		PointF pointf(10.0f, 100.0f);
		//显示字体显示大小 
		WCHAR infoString[100];
		swprintf(infoString,100, L"得分：%d", score);
		graphics.DrawString(infoString, wcslen(infoString), &font, pointf, &solidBrush);
		bodyRenderer::RenderStick(&graphics, MyBodys.stick);
		for (int i = 0; i < MyBodys.vec.size(); i++) {
			bodyRenderer::RenderBall(&graphics, &MyBodys.vec[i]);
		}
	}break;
	case FAILED: {
		image = Gdiplus::Image::FromFile(_T("end.png"));
		graphics.DrawImage(image, 0, 0, gwidth, gheight);
	}break;
	default:
		break;
	}
	if(image) delete image;
	if (senpai) delete senpai;
	BitBlt(hdc, 0, 0, gwidth, gheight, hMdc, 0, 0, SRCCOPY);
	ReleaseDC(hwnd, hdc);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WIN32PROJECT1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
	HDC hdc;
	MSG msg;
	HWND hwnd = InitInstance(hInstance, nCmdShow);
	
    // 执行应用程序初始化: 
    if (!(hwnd))
    {
        return FALSE;
    }


    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32PROJECT1));
	PlaySound(L"3.wav", NULL, SND_LOOP | SND_ASYNC);
    // 主消息循环: 
	ZeroMemory(&msg, sizeof(msg));
	//gameInit();
	
	SampleListener listener;
	Controller controller;
	controller.addListener(listener);
	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		static DWORD dwTime = timeGetTime();
		DWORD dwCurrentTime = timeGetTime();
		DWORD dwElapsedTime = dwCurrentTime - dwTime;
		float fElapsedTime = dwElapsedTime * 0.001f;



		Logic(dwElapsedTime);
		Render(hwnd);

		if (dwElapsedTime < 1000 / 60)
			Sleep(1000 / 60 - dwElapsedTime);
		dwTime = dwCurrentTime;
	}
    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32PROJECT1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = NULL;
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WIN32PROJECT1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}


//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
HWND InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中
   HDC hdc;

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPED,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return hWnd;
   }
   //onInit();
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return hWnd;
}



//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择: 
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
			int width, height;
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
			EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
