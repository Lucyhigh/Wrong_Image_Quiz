#include "Stdafx.h"
#include "MainGame.h"

HINSTANCE _hInstance;
HWND _hWnd;
POINT _ptMouse = { 0,0 };

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
void setWindowSize(int x, int y, int width, int height);

MainGame* _mg;

int APIENTRY WinMain( HINSTANCE hInstance,
						HINSTANCE hPrevInstance,
						LPSTR    lpCmdLine,
						int       nCmdShow)
{
	_mg = new MainGame();

	_hInstance = hInstance;

	WNDCLASS wndClass;

	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)(GetStockObject(WHITE_BRUSH));
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.lpfnWndProc = WndProc;
	wndClass.hInstance = hInstance;
	wndClass.lpszClassName = WINNAME;
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClass(&wndClass);

	_hWnd = CreateWindow
	(
		WINNAME,
		WINNAME,
		WINSTYLE,
		WINSTART_X,
		WINSTART_Y,
		WINSIZE_X,
		WINSIZE_Y,
		NULL,
		(HMENU)NULL,
		hInstance,
		NULL
	);
	setWindowSize(WINSTART_X, WINSTART_Y, WINSIZE_X, WINSIZE_Y);

	ShowWindow(_hWnd, nCmdShow);

	if (FAILED(_mg->init()))
	{
		return 0;
	}

	MSG message;
	while (GetMessage(&message, 0, 0, 0))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	_mg->release();
	UnregisterClass(WINNAME, hInstance);

	return message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	return _mg->MainProc(hWnd,iMessage,wParam,lParam);
}

void setWindowSize(int x, int y, int width, int height)
{
	RECT rc = { 0,0,width,height };
	AdjustWindowRect(&rc, WINSTYLE, false);
	SetWindowPos(_hWnd, NULL, x, y,
		(rc.right - rc.left), 
		(rc.bottom - rc.top),
		SWP_NOZORDER | SWP_NOMOVE);
}