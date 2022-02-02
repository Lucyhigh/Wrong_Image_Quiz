#include "Stdafx.h"
#include "GameNode.h"

HRESULT GameNode::init(void)
{
	SetTimer(_hWnd, 1, 10, NULL);
	KEYMANAGER->init();
	RND->init();

	//백버퍼 초기화
	this->setBackBuffer();
	return S_OK;
}

void GameNode::setBackBuffer(void)
{
	_backBuffer = new Image;
	_backBuffer->init(WINSIZE_X, WINSIZE_Y);
}
void GameNode::release(void)
{
	KillTimer(_hWnd, 1);
	//싱글톤해제
	KEYMANAGER->releaseSingleton();
	RND->releaseSingleton();
	//백버퍼 이미지 해제 - 여기도 정중히 물어볼가말가..? 원래는 필요하고 하는게 맞는데 나중에 내용바뀌어서 지금안함 우린 해야함
	SAFE_DELETE(_backBuffer);
}

void GameNode::update(void)
{
	InvalidateRect(_hWnd, NULL, FALSE);
}

void GameNode::render(HDC hdc)
{
}
LRESULT GameNode::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	switch (iMessage)
	{
	case WM_TIMER:
		this->update();
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		this->render(hdc);
		EndPaint(hWnd, &ps);
		break;

	case WM_MOUSEMOVE:
		_ptMouse.x = LOWORD(lParam);
		_ptMouse.y = HIWORD(lParam);
		break;

	case WM_KEYDOWN:
		switch (wParam) 
		{
		case VK_ESCAPE:
			PostMessage(hWnd, WM_DESTROY, 0, 0);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}


