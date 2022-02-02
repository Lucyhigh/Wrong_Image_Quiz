#include "Stdafx.h"
#include "GameNode.h"

HRESULT GameNode::init(void)
{
	SetTimer(_hWnd, 1, 10, NULL);
	KEYMANAGER->init();
	RND->init();

	//����� �ʱ�ȭ
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
	//�̱�������
	KEYMANAGER->releaseSingleton();
	RND->releaseSingleton();
	//����� �̹��� ���� - ���⵵ ������ ���������..? ������ �ʿ��ϰ� �ϴ°� �´µ� ���߿� ����ٲ� ���ݾ��� �츰 �ؾ���
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

