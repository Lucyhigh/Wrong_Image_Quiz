#include "Stdafx.h"
#include "Image.h"

//알파 블렌드를 사용하기 위한 라이브러리 추가 #pragma comment 명령어임 마이크로 소프트 이미지 32비트
#pragma comment (lib, "msimg32.lib")

Image::Image() :_imageInfo(NULL)
				,_fileName(NULL)
				,_isTrans(FALSE)
				,_transColor(RGB(0,0,0))
				,_blendImage(NULL)
				
{

}

HRESULT Image::init(int width, int height)
{
	//재초기화 방지-> 이미지 정보에 값이 들어 있다면 릴리즈부터 하겠다.포인트는 항상 예외를 걸자!!!
	if (_imageInfo != NULL) this->release();//너 널이 아니니?
	//assert(_imageInfo !=NULL)를 써도되지만...어설트는 디버깅에서 오류가 발생하면 코드를 죽임 -런타임에서 들어옴-경고창뜨면서...-원래는 런타임에서 오류가 뜨면 코드를 어쨋든 다 읽는데 이걸로 그런걸 막아줌
	//assert(_imageInfo !=nullptr)&&"노드를 쓰니 그렇징"); 
	//릴리즈할떄 오류나면서 밑코드 아예 안읽음..참일때 조건이 아니라 펄스가 조건임 반드시 잡아야하는 놈을 적어야함. 좋은데 추상화에서만 쓰는걸 ㅊㅊ)많이쓰지마 잘찍어서 쓰는것도 실력임 ㅇㅇ

	//컴파일에서 들어오는건
	//static_assert -c++에서 추가됨..장점도 있지만 이건 변수타입들은 못찾음 컴파일 타입이라서 상수만 잡아낼수있음

	//DC 가져오기
	HDC hdc = GetDC(_hWnd);

	/*
	resID = 0;
			hmemDC = NULL;//시작값이라 널널
			hBit = NULL;
			hOBit = NULL;
			width = 0;
			height = 0;
			loadType = LOAD_RESOURCE;
	
	*/
	//이미지 정보 새로 생성후 초기화
	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_EMPTY;
	_imageInfo->resID = 0;
	//1 조사해
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);
	//2
	_imageInfo->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, width, height);
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	_imageInfo->width = width;
	_imageInfo->height = height;

	//생성했으니 초기화 ㄱ
	//파일이름
	_fileName = NULL;
	_isTrans = FALSE;
	_transColor = RGB(0, 0, 0);

	//리소스 얻어오는데 실패했다.
	if (_imageInfo->hBit == 0)
	{
		release();
		return E_FAIL;
	}

	//가져온 DC 해제
	ReleaseDC(_hWnd, hdc);


	return S_OK;
}
//오버로딩된 것들이 남앗다.
//특징에 맞게 오버로딩을 한거임 공통교집합 이미지 인포 -const DWORD resID 파일이름 /OOL isTrans, COLORREF transColor 추가된거써야지
HRESULT Image::init(const DWORD resID, int width, int height, BOOL isTrans, COLORREF transColor)
{
	if (_imageInfo != NULL) this->release();//너 널이 아니니?

	//DC 가져오기
	HDC hdc = GetDC(_hWnd);

	//이미지 정보 새로 생성후 초기화
	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_RESOURCE;//
	_imageInfo->resID = resID;//
	//1 조사해
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);
	//2
	//3 로드비트맵 LoadBitmap
	//4 리소스를 가져오는데 핵심적인 함수 MAKEINTRESOURCE
	_imageInfo->hBit = (HBITMAP)LoadBitmap(_hInstance,MAKEINTRESOURCE(_imageInfo->resID));
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	_imageInfo->width = width;
	_imageInfo->height = height;

	//파일이름
	_fileName = NULL;
	_isTrans = isTrans;
	_transColor =transColor;

	//리소스 얻어오는데 실패했다.
	if (_imageInfo->hBit == 0)
	{
		release();
		return E_FAIL;
	}

	//가져온 DC 해제
	ReleaseDC(_hWnd, hdc);

	return S_OK;
}
//const char * filrName 이 다른 오버로딩
HRESULT Image::init(const char * fileName, int width, int height, BOOL isTrans, COLORREF transColor)
{
	if (_imageInfo != NULL) this->release();//너 널이 아니니?

	//DC 가져오기
	HDC hdc = GetDC(_hWnd);

	//이미지 정보 새로 생성후 초기화
	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_FILE;//
	_imageInfo->resID = 0;//
	//1 조사해
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);
	//2
	//3 로드비트맵 LoadBitmap
	//4 리소스를 가져오는데 핵심적인 함수 MAKEINTRESOURCE
	//5 LoadImage
	//6 LR_LOADFROMFILE
	_imageInfo->hBit = (HBITMAP)LoadImage(_hInstance, fileName,IMAGE_BITMAP,width,height,LR_LOADFROMFILE);
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	_imageInfo->width = width;
	_imageInfo->height = height;

	int len = strlen(fileName);

	_fileName = new CHAR[len + 1]; //len+1 에 1은 오타방지 로 넣어주는거임
	strcpy_s(_fileName,len+1,fileName);//복사해서 넣어주겠다~ strcpy_s 종류별로 다 공부해라******************************
	//파일이름
	//둘다 포인튼데 왜 안들어감? 문자타입이라고 안들어가; 컴파일러 입장에서는 디렉토리만 적어주면 우리가 알겟니? 파일네임이 문자인지 아닌지 생성좀 해서 넣어줄래 라고할거임
	_isTrans = isTrans;
	_transColor = transColor;

	//리소스 얻어오는데 실패했다.
	if (_imageInfo->hBit == 0)
	{
		release();
		return E_FAIL;
	}

	//가져온 DC 해제
	ReleaseDC(_hWnd, hdc);

	return S_OK;
}

HRESULT Image::init(const char * fileName, float x, float y, int width, int height, BOOL isTrans, COLORREF transColor)
{
	if (_imageInfo != NULL) this->release();//너 널이 아니니?

	//DC 가져오기
	HDC hdc = GetDC(_hWnd);

	//이미지 정보 새로 생성후 초기화
	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_FILE;//
	_imageInfo->resID = 0;//
	//1 조사해
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);
	//2
	//3 로드비트맵 LoadBitmap
	//4 리소스를 가져오는데 핵심적인 함수 MAKEINTRESOURCE
	//5 LoadImage
	//6 LR_LOADFROMFILE
	_imageInfo->hBit = (HBITMAP)LoadImage(_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);

	_imageInfo->x = x;
	_imageInfo->y = y;
	_imageInfo->width = width;
	_imageInfo->height = height;

	int len = strlen(fileName);

	_fileName = new CHAR[len + 1]; //len+1 에 1은 오타방지 로 넣어주는거임
	strcpy_s(_fileName, len + 1, fileName);//복사해서 넣어주겠다~ strcpy_s 종류별로 다 공부해라******************************
	//파일이름
	//둘다 포인튼데 왜 안들어감? 문자타입이라고 안들어가; 컴파일러 입장에서는 디렉토리만 적어주면 우리가 알겟니? 파일네임이 문자인지 아닌지 생성좀 해서 넣어줄래 라고할거임
	_isTrans = isTrans;
	_transColor = transColor;

	//리소스 얻어오는데 실패했다.
	if (_imageInfo->hBit == 0)
	{
		release();
		return E_FAIL;
	}

	//가져온 DC 해제
	ReleaseDC(_hWnd, hdc);

	return S_OK;
}

HRESULT Image::init(const char* fileName, int width, int height, int maxFrameX, int maxFrameY, BOOL isTrans, COLORREF transColor)
{
	if (_imageInfo != NULL) this->release();

	HDC hdc = GetDC(_hWnd);

	//이미지 정보 새로 생성 후 초기화
	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_FILE;
	_imageInfo->resID = 0;
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);
	_imageInfo->hBit = (HBITMAP)LoadImage(_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE); //LR_LOADFROMFILE : 로드 리소스
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	_imageInfo->width = width;
	_imageInfo->height = height;
	_imageInfo->currentFrameX = 0;
	_imageInfo->currentFrameY = 0;
	_imageInfo->maxFrameX = maxFrameX - 1;
	_imageInfo->maxFrameY = maxFrameY - 1;
	_imageInfo->frameWidth = width / maxFrameX;
	_imageInfo->frameHeight = height / maxFrameY;

	int len = strlen(fileName);

	_fileName = new CHAR[len + 1];
	strcpy_s(_fileName, len + 1, fileName);

	//파일 이름
	_isTrans = isTrans;
	_transColor = transColor;

	// 리소스 얻어오는데 실패했다.
	if (_imageInfo->hBit == 0)
	{
		release();
		return E_FAIL;
	}
	//DC 해제
	ReleaseDC(_hWnd, hdc);
	return S_OK;
}

HRESULT Image::init(const char * fileName, float x, float y, int width, int height, int maxFrameX, int maxFrameY, BOOL isTrans, COLORREF transColor)
{
	if (_imageInfo != NULL) this->release();//너 널이 아니니?

	//DC 가져오기
	HDC hdc = GetDC(_hWnd);

	//이미지 정보 새로 생성후 초기화
	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_FILE;//
	_imageInfo->resID = 0;//
	//1 조사해
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);
	//2
	//3 로드비트맵 LoadBitmap
	//4 리소스를 가져오는데 핵심적인 함수 MAKEINTRESOURCE
	//5 LoadImage
	//6 LR_LOADFROMFILE
	_imageInfo->hBit = (HBITMAP)LoadImage(_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	_imageInfo->width = width;
	_imageInfo->height = height;
	_imageInfo->x = x;
	_imageInfo->y = y;
	_imageInfo->currentFrameX = 0;
	_imageInfo->currentFrameY = 0;
	_imageInfo->maxFrameX = maxFrameX - 1;
	_imageInfo->maxFrameY = maxFrameY - 1;
	_imageInfo->frameWidth = width / maxFrameX;
	_imageInfo->frameHeight = height / maxFrameY;

	int len = strlen(fileName);

	_fileName = new CHAR[len + 1]; //len+1 에 1은 오타방지 로 넣어주는거임
	strcpy_s(_fileName, len + 1, fileName);//복사해서 넣어주겠다~ strcpy_s 종류별로 다 공부해라******************************
	//파일이름
	//둘다 포인튼데 왜 안들어감? 문자타입이라고 안들어가; 컴파일러 입장에서는 디렉토리만 적어주면 우리가 알겟니? 파일네임이 문자인지 아닌지 생성좀 해서 넣어줄래 라고할거임
	_isTrans = isTrans;
	_transColor = transColor;

	//리소스 얻어오는데 실패했다.
	if (_imageInfo->hBit == 0)
	{
		release();
		return E_FAIL;
	}

	//가져온 DC 해제
	ReleaseDC(_hWnd, hdc);
	return S_OK;
}

HRESULT Image::initForAlphaBlend(void)
{
	HDC hdc = GetDC(_hWnd);
	
	_blendFunc.BlendFlags = 0;			//혼합 알파값을 주지않겠다 만든사람도 걍 0으로쓰래 ㅋ 0은 사용하지 않겠다
	_blendFunc.AlphaFormat = 0;			//소스랑 비트맵 혼합시킬거임? 이미지자체로 처리할건지 +소스 해서 제어할건지
	_blendFunc.BlendOp = AC_SRC_OVER;	//비트맵 우리쓰는거 34비트로 할거임?64비트임? 우린상관없이 둘다 쓸거임이 OVER


	//이미지 정보 새로 생성후 초기화
	_blendImage = new IMAGE_INFO;
	_blendImage->loadType = LOAD_FILE;
	_blendImage->resID = 0; 
	_blendImage->hMemDC = CreateCompatibleDC(hdc);
	_blendImage->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, _imageInfo->width, _imageInfo->height);
	_blendImage->hOBit = (HBITMAP)SelectObject(_blendImage->hMemDC, _blendImage->hBit);
	_blendImage->width = WINSIZE_X;
	_blendImage->height = WINSIZE_Y;

	//가져온 DC 해제
	ReleaseDC(_hWnd, hdc);
	return S_OK;
}

void Image::setTransColor(BOOL isTrans, COLORREF transColor)
{
	_isTrans = isTrans;
	_transColor = transColor;
}

void Image::release(void)
{
	if (_imageInfo)//이미지정보가 남아있는지 묻고 삭제
	{
		SelectObject(_imageInfo->hMemDC, _imageInfo->hOBit);
		DeleteObject(_imageInfo->hBit);
		DeleteDC(_imageInfo->hMemDC);

		SAFE_DELETE(_imageInfo);
		SAFE_DELETE_ARRAY(_fileName);

		_isTrans = FALSE;
		_transColor = RGB(0, 0, 0);
	}
	if (_blendImage)
	{
		SelectObject(_blendImage->hMemDC, _blendImage->hBit);
		DeleteObject(_blendImage->hBit);
		DeleteDC(_blendImage->hMemDC);

		SAFE_DELETE(_blendImage);
	}
}
//렌더(0,0 지점에 렌더)
void Image::render(HDC hdc)
{
	if (_isTrans)//색깔 뺄거임?
	{
		//GdiTransparentBlt() : 비트맵을 불러올떄 특정색상을 제외하고 복사해주는 함수 -그냥 색상제외안하고 그대로 이미지불러오는것보단 느림
		GdiTransparentBlt					// 복사후 해제될때까지 메모리에 상주할거임 -
		(
			hdc,							//복사할 장소의 DC(화면DC-화면에 보여줄)
			0,								//복사될 시작좌표 x,y
			0,
			_imageInfo->width,				//복사할 이미지 크기 가로세로
			_imageInfo->height,
			_imageInfo->hMemDC,				//복사될 대상 메모리DC
			0, 0,							//복사 시작지점
			_imageInfo->width,				//복사 영역 가로세로 크기
			_imageInfo->height,
			_transColor						//복사할때 제외할 색상
		);
	}
	else //맵같은 거는 색깔 안빼도 되징 - 특정색상제외할게 없어서 고속복사
	{
		//BitBlt() : DC간의 영역끼리 서로 고속복사해주는 함수
		//SRCCOPY : 소스영역을 대상영역에 복사한다
		BitBlt(hdc, 0, 0, _imageInfo->width, _imageInfo->height, _imageInfo->hMemDC, 0, 0, SRCCOPY);
	}
}

void Image::render(HDC hdc, int destX, int destY)//int destX, int destY 를 써줘야겠네~
{
	if (_isTrans)//색깔 뺄거임?
	{
		//GdiTransparentBlt() : 비트맵을 불러올떄 특정색상을 제외하고 복사해주는 함수 -그냥 색상제외안하고 그대로 이미지불러오는것보단 느림
		GdiTransparentBlt					// 복사후 해제될때까지 메모리에 상주할거임 -
		(
			hdc,							//복사할 장소의 DC(화면DC-화면에 보여줄)
			destX,							//복사될 시작좌표 x,y
			destY,
			_imageInfo->width,				//복사할 이미지 크기 가로세로
			_imageInfo->height,
			_imageInfo->hMemDC,				//복사될 대상 메모리DC
			0, 0,							//복사 시작지점
			_imageInfo->width,				//복사 영역 가로세로 크기
			_imageInfo->height,
			_transColor						//복사할때 제외할 색상
		);
	}
	else //맵같은 거는 색깔 안빼도 되징 - 특정색상제외할게 없어서 고속복사
	{
		//BitBlt() : DC간의 영역끼리 서로 고속복사해주는 함수
		//SRCCOPY : 소스영역을 대상영역에 복사한다
		BitBlt(hdc, destX, destY, _imageInfo->width, _imageInfo->height, _imageInfo->hMemDC, 0, 0, SRCCOPY);
	}
}

void Image::render(HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight)
{
	if (_isTrans)
	{
		GdiTransparentBlt		
		(
			hdc,				
			destX,				
			destY,
			sourWidth,
			sourHeight,
			_imageInfo->hMemDC,	
			sourX, sourY,
			sourWidth,
			sourHeight,
			_transColor			
		);
	}
	else
	{
		BitBlt(hdc, destX, destY, sourWidth, sourHeight, _imageInfo->hMemDC, sourX, sourY, SRCCOPY);
	}
}

void Image::alphaRender(HDC hdc, BYTE alpha)
{
	//알파블렌드 처음 사용하는지? 물어봄 없으면 초기화부터 하고오셈
	if (!_blendImage) initForAlphaBlend();

	_blendFunc.SourceConstantAlpha = alpha;

	if (_isTrans)//색은 뺄거닝
	{
		//1.출력해야 될 DC에 그려져 있는 내용을 블렌드 이미지에 그린다.
		BitBlt
		(
			_blendImage->hMemDC,
			0, 0,
			_imageInfo->width,
			_imageInfo->height,
			hdc,
			0, 0,
			SRCCOPY
		);

		//2.원본 이미지의 배경을 없앤 후 블렌드 이미지에 그린다.
		GdiTransparentBlt
		(
			_blendImage->hMemDC,
			0,0,
			_imageInfo->width,
			_imageInfo->height,
			_imageInfo->hMemDC,
			0,0,
			_imageInfo->width,
			_imageInfo->height,
			_transColor
		);
		//3. 블렌드 이미지를 화면에 그린다.
		AlphaBlend
		(
			hdc,
			0, 0,
			_imageInfo->width,
			_imageInfo->height,
			_blendImage->hMemDC,
			0, 0,
			_imageInfo->width,
			_imageInfo->height,
			_blendFunc
		);

	}
	else//바로 그려라
	{
		AlphaBlend
		(
			hdc,
			0, 0,
			_imageInfo->width,
			_imageInfo->height,
			_imageInfo->hMemDC,
			0, 0,
			_imageInfo->width,
			_imageInfo->height,
			_blendFunc
		);
	}
}

void Image::alphaRender(HDC hdc, int destX, int destY, BYTE alpha)
{
	if (!_blendImage) initForAlphaBlend();

	_blendFunc.SourceConstantAlpha = alpha;

	if (_isTrans)//색은 뺄거닝
	{
		//1.출력해야 될 DC에 그려져 있는 내용을 블렌드 이미지에 그린다.
		BitBlt
		(
			_blendImage->hMemDC,
			0, 0,
			_imageInfo->width,
			_imageInfo->height,
			hdc,
			destX, destY,
			SRCCOPY
		);

		//2.원본 이미지의 배경을 없앤 후 블렌드 이미지에 그린다.
		GdiTransparentBlt
		(
			_blendImage->hMemDC,
			0, 0,
			_imageInfo->width,
			_imageInfo->height,
			_imageInfo->hMemDC,
			0, 0,
			_imageInfo->width,
			_imageInfo->height,
			_transColor
		);
		//3. 블렌드 이미지를 화면에 그린다.
		AlphaBlend
		(
			hdc,
			destX, destY,
			_imageInfo->width,
			_imageInfo->height,
			_blendImage->hMemDC,
			0, 0,
			_imageInfo->width,
			_imageInfo->height,
			_blendFunc
		);

	}
	else//바로 그려라
	{
		AlphaBlend
		(
			hdc,
			destX, destY,
			_imageInfo->width,
			_imageInfo->height,
			_imageInfo->hMemDC,
			0, 0,
			_imageInfo->width,
			_imageInfo->height,
			_blendFunc
		);
	}
}

void Image::alphaRender(HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight, BYTE alpha)
{

}

void Image::frameRender(HDC hdc, int destX, int destY)
{
	if (_isTrans)//색깔 뺄거임?
	{
		GdiTransparentBlt			
		(
			hdc,						
			destX,						
			destY,
			_imageInfo->frameWidth,		
			_imageInfo->frameHeight,
			_imageInfo->hMemDC,			
			_imageInfo->currentFrameX * _imageInfo->frameWidth,							
			_imageInfo->currentFrameY * _imageInfo->frameHeight,						
			_imageInfo->frameWidth,			
			_imageInfo->frameHeight,
			_transColor						
		);
	}
	else 
	{
		BitBlt(hdc, destX, destY, _imageInfo->frameWidth, _imageInfo->frameHeight, _imageInfo->hMemDC,
			_imageInfo->currentFrameX * _imageInfo->frameWidth,
			_imageInfo->currentFrameY * _imageInfo->frameHeight, SRCCOPY);
	}
}

void Image::frameRender(HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY)
{
	//이미지 예외처리
	_imageInfo->currentFrameX = currentFrameX;
	_imageInfo->currentFrameY = currentFrameY;
	if (currentFrameX > _imageInfo->maxFrameX)
	{
		_imageInfo->currentFrameX = _imageInfo->maxFrameX;
	}
	if (currentFrameY > _imageInfo->maxFrameY)
	{
		_imageInfo->currentFrameY = _imageInfo->maxFrameY;
	}

	if (_isTrans)//색깔 뺄거임?
	{
		//GdiTransparentBlt() : 비트맵을 불러올떄 특정색상을 제외하고 복사해주는 함수 -그냥 색상제외안하고 그대로 이미지불러오는것보단 느림
		GdiTransparentBlt					// 복사후 해제될때까지 메모리에 상주할거임 -
		(
			hdc,							//복사할 장소의 DC(화면DC-화면에 보여줄)
			destX,							//복사될 시작좌표 x,y
			destY,
			_imageInfo->frameWidth,				//복사할 이미지 크기 가로세로
			_imageInfo->frameHeight,
			_imageInfo->hMemDC,				//복사될 대상 메모리DC
			_imageInfo->currentFrameX * _imageInfo->frameWidth,							//복사 시작지점
			_imageInfo->currentFrameY * _imageInfo->frameHeight,							//복사 시작지점
			_imageInfo->frameWidth,				//복사 영역 가로세로 크기
			_imageInfo->frameHeight,
			_transColor						//복사할때 제외할 색상
		);
	}
	else //맵같은 거는 색깔 안빼도 되징 - 특정색상제외할게 없어서 고속복사
	{
		//BitBlt() : DC간의 영역끼리 서로 고속복사해주는 함수
		//SRCCOPY : 소스영역을 대상영역에 복사한다
		BitBlt(hdc, destX, destY, _imageInfo->frameWidth, _imageInfo->frameHeight, _imageInfo->hMemDC, 
			_imageInfo->currentFrameX * _imageInfo->frameWidth, _imageInfo->currentFrameY * _imageInfo->frameHeight, SRCCOPY);
	}
}

