#pragma once
/*
���İ� 0~255
���� - ���̵� �� ���̵� �ƿ�


�ҷ��ö��� ���� 100���� �ҷ��ͳ���
�ڵ忡�� ���İ� �����ϴ°� ����

���İ��� ����Ǿ����� 
���� ���̺귯���� ���� ������
-c++ boost �Ѿ� ���̺귯�� ����

*/
class Image
{
	//�ϴ� ���������� ���̴� �͵� ���Ǹ� �ص�
public:
	enum IMAGE_LOAD_KIND//�̹������� �ҽ� ���� ��� �ƹ��͵��Ȱ����� - ���͸� ���� �����ð���,�ε��Ұǵ� ���Ϸΰ����ð��� �ҽ��ΰ����ð��� ��ŷ� �����ð��� ...
	{
		LOAD_RESOURCE = 0,
		LOAD_FILE,
		LOAD_EMPTY,
		LOAD_END
	};
	/*���̾��� �˰��ֱ�
	DWORD : unsigened long
	WORD : unsigened short
	BYTE : unsigened char -���� �������ؼ� 126�����ε� �����Ⱦ��ϱ�256���� �������� 255���̶� ���� �����Ͱ� ��� ���ϴ��� �߿�...�̹� �� ���ǰ� �Ǿ�������
	*/

	typedef struct tagImage //typedef ��Ī
	{
		DWORD	resID;		//DWORD UNSIGHED LONG�� �������Ѱ��� ���ҽ� ���̵� 
		HDC		hMemDC;		//�޸𸮵�� ��
		HBITMAP hBit;		//��Ʈ�� ������
		HBITMAP hOBit;		//�õ��Ʈ�� - ������ �ҷ��� �̹����� �̸� �ҷ��� -�̹��� �������� ����
		float x;
		float y;
		int		width;		//�̹��� ����
		int		height;		//�̹�������
		int		currentFrameX;
		int		currentFrameY;
		int		maxFrameX;
		int		maxFrameY;
		int		frameWidth;
		int		frameHeight;
		BYTE	loadType;	//�̹��� �ε�Ÿ�� ����
		tagImage()//�ʱⰪ �־���
		{
			resID = 0;
			hMemDC = NULL;//���۰��̶� �γ�
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
	}IMAGE_INFO, *LPIMAGE_INFO;//����ƮŸ���� ����Ŷ� ����ƮŸ���� �������ִ� ������ ���� - �±��̹����� ��Ī

public:
	//�ʱ�ȭ ������ �� �ε� ī�ε� �� Ŭ������ �������...!

	//�ϴ� �� ��Ʈ�� �ʱ�ȭ -�ڵ��� ��Ȱ���� ���� �����ε��ؼ� ��Ŭ���� ���� ������
	HRESULT init(int width, int height);
	//�̹��� ���ҽ� �ʱ�ȭ
	HRESULT init(const DWORD resID, int width, int height, BOOL isTrans = FALSE, COLORREF transColor =RGB(0,0,0));//�� ���򻩰ž�? �ʱⰪ�� �Ȼ��ž߶�� ����
	//�̹��� ���Ϸ� �ʱ�ȭ
	HRESULT init(const char* fileName, int width, int height, BOOL isTrans = FALSE, COLORREF transColor =RGB(0,0,0));//���ҽ��� �ƴ϶� ���ϸ����� �����ðž�

	//������ �̹��� ���Ϸ� �ʱ�ȭ
	HRESULT init(const char* fileName, float x, float y, int width, int height, BOOL isTrans = FALSE, COLORREF transColor = RGB(0, 0, 0));

   HRESULT init(const char* fileName, int width, int height, int maxFrameX, int maxFrameY, BOOL isTrans = FALSE, COLORREF transColor = RGB(0, 0, 0));

   HRESULT init(const char* fileName, float x, float y, int width, int height, int maxFrameX, int maxFrameY, BOOL isTrans = FALSE, COLORREF transColor = RGB(0, 0, 0));
																																   //���� ���� �ʱ�ȭ
	HRESULT initForAlphaBlend(void);

	//���� �÷�Ű ����(���� �������� ���)
	void setTransColor(BOOL isTrans, COLORREF transColor);
	//����
	void release(void);//***....�޸𸮰��������̱⿡ �����߿� ������ �ٽ� �����������ϱ�,,,

	//����
	void render(HDC hdc);
	void render(HDC hdc, int destX, int destY);
	//Ŭ����-�ڸ���,�̴ϸ� ������ ���̾�
	void render(HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight);

	//���� ����
	void alphaRender(HDC hdc, BYTE alpha);		//���
	void alphaRender(HDC hdc, int destX, int destY, BYTE alpha);//���ĳ���� ��
	void alphaRender(HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight, BYTE alpha);//Ŭ���ξ��ķ���

	//������ ����
	void frameRender(HDC hdc, int destX, int destY);
	void frameRender(HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY);

	//�ζ��� �Լ�=============
	//DC ���
	inline HDC getMemDC(void) { return _imageInfo->hMemDC; }//ó���� �� �������� �ؼ� �ζ������� ������

	//�̹��� X��ǥ
	inline float getX(void) { return _imageInfo->x; }
	inline void setX(float x) { _imageInfo->x = x; }

	//�̹��� Y��ǥ
	inline float getY(void) { return _imageInfo->y; }
	inline void setY(float y) { _imageInfo->y = y; }

	//�̹��� ������ǥ
	inline void setCenter(float x, float y)
	{
		_imageInfo->x = x - (_imageInfo->width / 2);
		_imageInfo->y = y - (_imageInfo->height / 2);
	}

	//�̹��� ����,����ũ��
	inline int getWidth(void) { return _imageInfo->width; }
	inline int getHeight(void) { return _imageInfo->height; }

	//�ٿ�� �ڽ�(�浹�� ��Ʈ)
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

	//�ٿ�� �ڽ� ������
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


	//������ X
	inline int getFrameX(void){ return _imageInfo->currentFrameX; }
	inline void setFrameX(int frameX)
	{
		_imageInfo->currentFrameX = frameX;
		if (frameX > _imageInfo->maxFrameX)
		{
			_imageInfo->currentFrameX = _imageInfo->maxFrameX;
		}
	}

	//������ Y
	inline int getFrameY(void) { return _imageInfo->currentFrameY; }
	inline void setFrameY(int frameY)
	{
		_imageInfo->currentFrameY = frameY;
		if (frameY > _imageInfo->maxFrameY)
		{
			_imageInfo->currentFrameY = _imageInfo->maxFrameY;
		}
	}

	//�̹��� 1������ ����, ����ũ��
	inline int getFrameWidth(void) { return _imageInfo->frameWidth; }
	inline int getFrameWidthHeight(void) { return _imageInfo->frameHeight; }
	//�̹��� �ִ� ������ x, y ����
	inline int getMaxFrameX(void) { return _imageInfo ->maxFrameX; }
	inline int getMaxFrameY(void) { return _imageInfo ->maxFrameY; }

public:
	Image();
	~Image() {}

private:	//�� ��״� ��� �̴ϼȶ���¡���� �ҷ����°ɰ�? ��Ӱ� ���谡 ����...!����� �׷��ٰԾƴϰ� �׷��ֱ����� ������ �һ���! �׷��� ����� �߻��Ѵ�. 
			//new�� �����Ҵ��� �Ǿ� �ڵ带 ������ ������ ȣ���� �߻��� ����̴ϼȶ���¡�� ȣ��ʰ� ���ÿ� ������ �ڵ带 �б��� �޸޹� �̴ϼȶ���¡ ���� �ʱ�ȭ ��Ű�� �ڵ带 �д´� 
			//	�׷��� �ʱ�ȭ�� ���� ���� ��Ű�ڴٴ� ��!!!!

	LPIMAGE_INFO _imageInfo; //�̹������� �ּҰ� - ����Ʈ�� �׻� ���ǹ����� ������ְ� ��������ض�...�̱������� ����
	CHAR*		 _fileName; //�̹����� �̸�
	BOOL		 _isTrans; //������ ���ٰ���?
	COLORREF	 _transColor;//�������� ���̵� ������ ���� rgb���� ���� �÷����۷��� 32��Ʈ ������  rgb(���� 1����Ʈ�� ������ �ְ� 0~255 ��ǥ�� ����) �� ���� �ɰ�������� ����Ʈ�����
	//-�ڵ�����ȭ�� ���� �ɰ��� ���� �̰� �� ���İ��ٶ� 0~255�� ǥ�������ε� �̰� unsigened char�ϱ� ����..�������! ������Ÿ���� ���ϴ°͵� �߿�!

	BLENDFUNCTION	_blendFunc;
	LPIMAGE_INFO	_blendImage;



};

