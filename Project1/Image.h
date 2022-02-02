#pragma once
/*
알파값 0~255
투명도 - 페이드 인 페이드 아웃


불러올때는 투명도 100으로 불러와놓고
코드에서 알파값 조절하는게 조음

알파값은 내장되어잇음 
외장 라이브러리엔 뭐가 잇을가
-c++ boost 총알 라이브러리 ㅋㅋ

*/
class Image
{
	//일단 공통적으로 쓰이는 것들 정의를 해둠
public:
	enum IMAGE_LOAD_KIND//이미지종류 소스 파일 빈거 아무것도안가져옴 - 디렉터를 통해 가져올거임,로드할건데 파일로가져올건지 소스로가져올건지 빈거로 가져올건지 ...
	{
		LOAD_RESOURCE = 0,
		LOAD_FILE,
		LOAD_EMPTY,
		LOAD_END
	};
	/*많이쓰니 알고있기
	DWORD : unsigened long
	WORD : unsigened short
	BYTE : unsigened char -원래 음수값해서 126까지인데 음수안쓰니까256까지 쓸수잇음 255색이랑 같네 데이터값 어떻게 정하느냐 중요...이미 다 정의가 되어있으니
	*/

	typedef struct tagImage //typedef 별칭
	{
		DWORD	resID;		//DWORD UNSIGHED LONG을 재정의한거임 리소스 아이디 
		HDC		hMemDC;		//메모리디시 멤
		HBITMAP hBit;		//비트맵 가져옴
		HBITMAP hOBit;		//올드비트맵 - 다음에 불러올 이미지를 미리 불러옴 -이미지 병목현상 방지
		float x;
		float y;
		int		width;		//이미지 가로
		int		height;		//이미지세로
		int		currentFrameX;
		int		currentFrameY;
		int		maxFrameX;
		int		maxFrameY;
		int		frameWidth;
		int		frameHeight;
		BYTE	loadType;	//이미지 로드타입 정의
		tagImage()//초기값 넣어줌
		{
			resID = 0;
			hMemDC = NULL;//시작값이라 널널
			hBit = NULL;
			hOBit = NULL;
			x = y = 0;
			width = 0;
			height = 0;
			currentFrameX = 0;
			currentFrameY = 0;
			maxFrameX = 0;
			maxFrameY = 0;
			frameWidth = 0;
			frameHeight = 0;
			loadType = LOAD_RESOURCE;
		}
	}IMAGE_INFO, *LPIMAGE_INFO;//포인트타입을 만들거라 포인트타입을 받을수있는 변수를 받음 - 태그이미지의 별칭

public:
	//초기화 내용이 다 로드 카인드 넘 클래스에 들어있음...!

	//일단 빈 비트맵 초기화 -코드의 재활용을 위해 오버로딩해서 넘클래스 값들 쓸거임
	HRESULT init(int width, int height);
	//이미지 리소스 초기화
	HRESULT init(const DWORD resID, int width, int height, BOOL isTrans = FALSE, COLORREF transColor =RGB(0,0,0));//너 색깔빼거야? 초기값은 안뺄거야라고 넣음
	//이미지 파일로 초기화
	HRESULT init(const char* fileName, int width, int height, BOOL isTrans = FALSE, COLORREF transColor =RGB(0,0,0));//리소스가 아니라 파일맵으로 가져올거야

	//프레임 이미지 파일로 초기화
	HRESULT init(const char* fileName, float x, float y, int width, int height, BOOL isTrans = FALSE, COLORREF transColor = RGB(0, 0, 0));

   HRESULT init(const char* fileName, int width, int height, int maxFrameX, int maxFrameY, BOOL isTrans = FALSE, COLORREF transColor = RGB(0, 0, 0));

   HRESULT init(const char* fileName, float x, float y, int width, int height, int maxFrameX, int maxFrameY, BOOL isTrans = FALSE, COLORREF transColor = RGB(0, 0, 0));
																																   //알파 블렌드 초기화
	HRESULT initForAlphaBlend(void);

	//투명 컬러키 셋팅(배경색 날릴건지 어떤색)
	void setTransColor(BOOL isTrans, COLORREF transColor);
	//해제
	void release(void);//***....메모리관리차원이기에 제일중요 비워줘야 다시 넣을수있으니까,,,

	//랜더
	void render(HDC hdc);
	void render(HDC hdc, int destX, int destY);
	//클리핑-자르기,미니맵 구현시 많이씀
	void render(HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight);

	//알파 렌더
	void alphaRender(HDC hdc, BYTE alpha);		//배경
	void alphaRender(HDC hdc, int destX, int destY, BYTE alpha);//배경캐릭더 다
	void alphaRender(HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight, BYTE alpha);//클리핑알파렌더

	//프레임 렌더
	void frameRender(HDC hdc, int destX, int destY);
	void frameRender(HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY);

	//인라인 함수=============
	//DC 얻기
	inline HDC getMemDC(void) { return _imageInfo->hMemDC; }//처리가 좀 빨랐으면 해서 인라인으로 가져옴

	//이미지 X좌표
	inline float getX(void) { return _imageInfo->x; }
	inline void setX(float x) { _imageInfo->x = x; }

	//이미지 Y좌표
	inline float getY(void) { return _imageInfo->y; }
	inline void setY(float y) { _imageInfo->y = y; }

	//이미지 센터좌표
	inline void setCenter(float x, float y)
	{
		_imageInfo->x = x - (_imageInfo->width / 2);
		_imageInfo->y = y - (_imageInfo->height / 2);
	}

	//이미지 가로,세로크기
	inline int getWidth(void) { return _imageInfo->width; }
	inline int getHeight(void) { return _imageInfo->height; }

	//바운딩 박스(충돌용 렉트)
	inline RECT boundingBox(void)
	{
		RECT rc =
		{
			(int)_imageInfo->x,
			(int)_imageInfo->y,
			(int)_imageInfo->x+_imageInfo->width,
			(int)_imageInfo->y+_imageInfo->height,
		};
		return rc;
	}

	//바운딩 박스 프레임
	inline RECT boundingBoxWithFrame(void)
	{
		RECT rc =
		{
			(int)_imageInfo->x,
			(int)_imageInfo->y,
			(int)_imageInfo->x + _imageInfo->width,
			(int)_imageInfo->y + _imageInfo->height,
		};
		return rc;
	}


	//프레임 X
	inline int getFrameX(void){ return _imageInfo->currentFrameX; }
	inline void setFrameX(int frameX)
	{
		_imageInfo->currentFrameX = frameX;
		if (frameX > _imageInfo->maxFrameX)
		{
			_imageInfo->currentFrameX = _imageInfo->maxFrameX;
		}
	}

	//프레임 Y
	inline int getFrameY(void) { return _imageInfo->currentFrameY; }
	inline void setFrameY(int frameY)
	{
		_imageInfo->currentFrameY = frameY;
		if (frameY > _imageInfo->maxFrameY)
		{
			_imageInfo->currentFrameY = _imageInfo->maxFrameY;
		}
	}

	//이미지 1프레임 가로, 세로크기
	inline int getFrameWidth(void) { return _imageInfo->frameWidth; }
	inline int getFrameWidthHeight(void) { return _imageInfo->frameHeight; }
	//이미지 최대 프레임 x, y 갯수
	inline int getMaxFrameX(void) { return _imageInfo ->maxFrameX; }
	inline int getMaxFrameY(void) { return _imageInfo ->maxFrameY; }

public:
	Image();
	~Image() {}

private:	//왜 얘네는 멤버 이니셜라이징으로 불러오는걸가? 상속과 관계가 있음...!여기는 그려줄게아니고 그려주기위한 셋팅을 할뿐임! 그러니 상속이 발생한다. 
			//new는 동적할당이 되어 코드를 읽으면 생성자 호출이 발생됨 멤버이니셜라이징은 호출됨과 동시에 생성자 코드를 읽기전 메메버 이니셜라이징 값을 초기화 시키고 코드를 읽는다 
			//	그러니 초기화를 제일 먼저 시키겠다는 뜻!!!!

	LPIMAGE_INFO _imageInfo; //이미지들의 주소값 - 포인트는 항상 조건문으로 물어봐주고 지우던지해라...싱글톤해제 참고
	CHAR*		 _fileName; //이미지의 이름
	BOOL		 _isTrans; //배경색을 없앨건지?
	COLORREF	 _transColor;//위에꺼랑 같이돔 배경색을 없앨 rgb값이 모임 컬러레퍼런스 32비트 정수형  rgb(각각 1바이트씩 가지고 있고 0~255 농도표현 가능) 더 색을 쪼개고싶으면 바이트쓰면됨
	//-코드최적화를 위해 쪼개기 위해 이거 씀 알파값줄때 0~255만 표현가능인데 이거 unsigened char니까 딱임..낭비없이! 데이터타입을 정하는것도 중요!

	BLENDFUNCTION	_blendFunc;
	LPIMAGE_INFO	_blendImage;



};

