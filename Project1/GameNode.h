#pragma once
#include"Image.h"
class GameNode
{
private:
	Image* _backBuffer; //포인터형 변수	
	void setBackBuffer(void);//이미지 백버퍼 생성으로 이미지는 게임노드를 거쳐야함
public:
	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(HDC hdc);
	//백버퍼 이미지 얻기
	Image* getBackBuffer(void) { return _backBuffer; }//접근자get 설정자set

	LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

	GameNode() {};
	virtual ~GameNode() {};
};

