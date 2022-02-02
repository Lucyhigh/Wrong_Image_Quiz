#pragma once
#include"Image.h"
class GameNode
{
private:
	Image* _backBuffer; //�������� ����	
	void setBackBuffer(void);//�̹��� ����� �������� �̹����� ���ӳ�带 ���ľ���
public:
	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(HDC hdc);
	//����� �̹��� ���
	Image* getBackBuffer(void) { return _backBuffer; }//������get ������set

	LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

	GameNode() {};
	virtual ~GameNode() {};
};

