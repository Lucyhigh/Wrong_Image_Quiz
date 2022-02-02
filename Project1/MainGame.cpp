#include "Stdafx.h"
#include "MainGame.h"

#define PI 3.141592653 

HRESULT MainGame::init(void)
{
	GameNode::init();
	
	
	_alphaA,_alphaB = 0;
	_bgStart1Image = new Image;
	_bgStart1Image->init("Resources/Images/BackGround/stage1start.bmp", WINSIZE_X, WINSIZE_Y);
	_bgEnd1Image = new Image;
	_bgEnd1Image->init("Resources/Images/BackGround/stage1end.bmp", WINSIZE_X, WINSIZE_Y);
	_bgStart2Image = new Image;
	_bgStart2Image->init("Resources/Images/BackGround/stage2start.bmp", WINSIZE_X, WINSIZE_Y);
	_bgEnd2Image = new Image;
	_bgEnd2Image->init("Resources/Images/BackGround/stage2end.bmp", WINSIZE_X, WINSIZE_Y);

	for (int i = 0; i < 10; i++)
	{
		_checkBox.push_back(new checkBox);
		_checkBox[i]->_isBoxActive = false;
	}
	_checkBox[0]->_check = RectMakeCenter(616, 206, 105, 105);		//눈
	_checkBox[1]->_check = RectMakeCenter(395, 293, 105, 105);		//냥이
	_checkBox[2]->_check = RectMakeCenter(641, 541, 105, 105);		//가슴냥
	_checkBox[3]->_check = RectMakeCenter(405, 746, 105, 105);		//붓칠
	_checkBox[4]->_check = RectMakeCenter(241,716, 105, 105);		//냥옷 - stage1

	_checkBox[5]->_check = RectMakeCenter(228,480, 55,55);		//헬프  - stage2
	_checkBox[6]->_check = RectMakeCenter(514,220, 55,55);		//단추  - stage2
	_checkBox[7]->_check = RectMakeCenter(128,525, 55,55);		//일러스트그림  - stage2
	_checkBox[8]->_check = RectMakeCenter(666, 764, 55,55);		//로고의 눈 - stage2
	_checkBox[9]->_check = RectMakeCenter(105,261, 55,55);		//목카라 - stage2
	
	_checkImage = new Image;
	_checkImage->init("Resources/Images/Object/check1.bmp", 161, 99,true,RGB(255,0,255));
	_endingImage = new Image;
	_endingImage->init("Resources/Images/BackGround/bgbg.bmp", WINSIZE_X, WINSIZE_Y);
	_stageLevel = _alphaNum = 1;

	_checkIndex =_alphaA =_count = _bonusCount = 0;

	return S_OK;
}

void MainGame::release(void)
{
	GameNode::release();
	
	SAFE_DELETE(_bgStart1Image);
	SAFE_DELETE(_bgEnd1Image);
	SAFE_DELETE(_bgStart2Image);
	SAFE_DELETE(_bgEnd2Image);
	SAFE_DELETE(_checkImage);
	SAFE_DELETE(_endingImage);
}

void MainGame::update(void)
{
	GameNode::update();

	_count++;

	if (_checkIndex >= 5)
	{
		_stageLevel++;
		_checkIndex = 0;
		_alphaA = 0;
		_count = 0;
	}

	if (_alphaA < 255 &&_count >=100)
	{
		_alphaA++;
	}
	if (_count >= 500)
	{
		_stageLevel = 0;
	}
	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		if (_stageLevel == 1)
		{
			for (int i = 0; i < 5; i++)
			{
				if (PtInRect(&_checkBox[i]->_check, _ptMouse) && !_checkBox[i]->_isBoxActive)
				{
					_checkBox[i]->_isBoxActive = true;
					_checkIndex++;
					_count -= 200;
					if (_count < 0)_count = 0;
				}
				else _count+=20;
			}
		}
		else if (_stageLevel == 2)
		{
			
			for (int i = 5; i < 10; i++)
			{
				if (PtInRect(&_checkBox[i]->_check, _ptMouse) && !_checkBox[i]->_isBoxActive)
				{
					_checkBox[i]->_isBoxActive = true;
					_checkIndex++;
					_count -= 200;
					if (_count < 0)_count = 0;
				}
				else _count += 20;
			}
		}
	}	
}

void MainGame::render(HDC hdc)
{
	HDC memDC = this->getBackBuffer()->getMemDC();
	PatBlt(memDC, 0, 0, WINSIZE_X, WINSIZE_Y, BLACKNESS);

	char strInfo[128];
	char strCount[128];

	if (_stageLevel == 1)
	{
		_bgStart1Image->render(memDC, 0, 0);
		_bgEnd1Image->alphaRender(memDC, _alphaA);

		if (KEYMANAGER->isStayKeyDown(VK_F1))
		{
			_bgStart1Image->render(memDC, 0, 0);
		}

		for (int i = 0; i < 5; i++)
		{
			if (_checkBox[i]->_isBoxActive)
			{
				_checkImage->render(memDC, _checkBox[i]->_check.left,
											_checkBox[i]->_check.top);
			}
		}
		wsprintf(strInfo, "!!스테이지 1!! 5초 후에 이미지가 바뀝니다! 틀린곳 5개를 찾아보세요");
		TextOut(memDC, 50, 30, strInfo, strlen(strInfo));
		wsprintf(strInfo, "500초 드립니다 맞추면 시간 추가 잘못 누르면 시간 감소합니다");
		TextOut(memDC, 50, 50, strInfo, strlen(strInfo));
		wsprintf(strCount, "Count : %d",_count);
		TextOut(memDC, 50,70, strCount, strlen(strCount));
	}

	else if(_stageLevel ==2)
	{
		_bgStart2Image->render(memDC, 0, 0);
		_bgEnd2Image->alphaRender(memDC, _alphaA);
		if (KEYMANAGER->isStayKeyDown(VK_F1))
		{
			_bgStart2Image->render(memDC, 0, 0);
		}
		wsprintf(strInfo, "!!스테이지 2!!5초 후에 이미지가 바뀝니다! 틀린곳 5개를 찾아보세요");
		TextOut(memDC, 50, 30, strInfo, strlen(strInfo));
		wsprintf(strInfo, "500초 드립니다 맞추면 시간 추가 잘못 누르면 시간 감소");
		TextOut(memDC, 50, 50, strInfo, strlen(strInfo));
		wsprintf(strCount, "Count : %d", _count);
		TextOut(memDC, 50, 70, strCount, strlen(strCount));
		for (int i = 5; i < 10; i++)
		{
			if (_checkBox[i]->_isBoxActive)
			{
				_checkImage->render(memDC, _checkBox[i]->_check.left-20,
											_checkBox[i]->_check.top);
			}
		}
	}
	else if (_stageLevel == 3)
	{
		_endingImage->render(memDC, 0, 0);
		wsprintf(strInfo, "!!축!! 클리어!!!");
		TextOut(memDC, 350, 400, strInfo, strlen(strInfo));
	}
	else if (_stageLevel == 0)
	{
		PatBlt(memDC, 0, 0, WINSIZE_X, WINSIZE_Y, BLACKNESS);
		wsprintf(strInfo, "!!실-패!!");
		TextOut(memDC, 350, 400, strInfo, strlen(strInfo));
	}
	this->getBackBuffer()->render(hdc);
}
