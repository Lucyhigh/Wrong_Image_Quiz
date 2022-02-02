#include "Stdafx.h"
#include "Image.h"

//���� ���带 ����ϱ� ���� ���̺귯�� �߰� #pragma comment ��ɾ��� ����ũ�� ����Ʈ �̹��� 32��Ʈ
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
	//���ʱ�ȭ ����-> �̹��� ������ ���� ��� �ִٸ� ��������� �ϰڴ�.����Ʈ�� �׻� ���ܸ� ����!!!
	if (_imageInfo != NULL) this->release();//�� ���� �ƴϴ�?
	//assert(_imageInfo !=NULL)�� �ᵵ������...�Ʈ�� ����뿡�� ������ �߻��ϸ� �ڵ带 ���� -��Ÿ�ӿ��� ����-���â�߸鼭...-������ ��Ÿ�ӿ��� ������ �߸� �ڵ带 ��¶�� �� �дµ� �̰ɷ� �׷��� ������
	//assert(_imageInfo !=nullptr)&&"��带 ���� �׷�¡"); 
	//�������ҋ� �������鼭 ���ڵ� �ƿ� ������..���϶� ������ �ƴ϶� �޽��� ������ �ݵ�� ��ƾ��ϴ� ���� �������. ������ �߻�ȭ������ ���°� ����)���̾����� ���� ���°͵� �Ƿ��� ����

	//�����Ͽ��� �����°�
	//static_assert -c++���� �߰���..������ ������ �̰� ����Ÿ�Ե��� ��ã�� ������ Ÿ���̶� ����� ��Ƴ�������

	//DC ��������
	HDC hdc = GetDC(_hWnd);

	/*
	resID = 0;
			hmemDC = NULL;//���۰��̶� �γ�
			hBit = NULL;
			hOBit = NULL;
			width = 0;
			height = 0;
			loadType = LOAD_RESOURCE;
	
	*/
	//�̹��� ���� ���� ������ �ʱ�ȭ
	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_EMPTY;
	_imageInfo->resID = 0;
	//1 ������
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);
	//2
	_imageInfo->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, width, height);
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	_imageInfo->width = width;
	_imageInfo->height = height;

	//���������� �ʱ�ȭ ��
	//�����̸�
	_fileName = NULL;
	_isTrans = FALSE;
	_transColor = RGB(0, 0, 0);

	//���ҽ� �����µ� �����ߴ�.
	if (_imageInfo->hBit == 0)
	{
		release();
		return E_FAIL;
	}

	//������ DC ����
	ReleaseDC(_hWnd, hdc);


	return S_OK;
}
//�����ε��� �͵��� ���Ѵ�.
//Ư¡�� �°� �����ε��� �Ѱ��� ���뱳���� �̹��� ���� -const DWORD resID �����̸� /OOL isTrans, COLORREF transColor �߰��ȰŽ����
HRESULT Image::init(const DWORD resID, int width, int height, BOOL isTrans, COLORREF transColor)
{
	if (_imageInfo != NULL) this->release();//�� ���� �ƴϴ�?

	//DC ��������
	HDC hdc = GetDC(_hWnd);

	//�̹��� ���� ���� ������ �ʱ�ȭ
	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_RESOURCE;//
	_imageInfo->resID = resID;//
	//1 ������
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);
	//2
	//3 �ε��Ʈ�� LoadBitmap
	//4 ���ҽ��� �������µ� �ٽ����� �Լ� MAKEINTRESOURCE
	_imageInfo->hBit = (HBITMAP)LoadBitmap(_hInstance,MAKEINTRESOURCE(_imageInfo->resID));
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	_imageInfo->width = width;
	_imageInfo->height = height;

	//�����̸�
	_fileName = NULL;
	_isTrans = isTrans;
	_transColor =transColor;

	//���ҽ� �����µ� �����ߴ�.
	if (_imageInfo->hBit == 0)
	{
		release();
		return E_FAIL;
	}

	//������ DC ����
	ReleaseDC(_hWnd, hdc);

	return S_OK;
}
//const char * filrName �� �ٸ� �����ε�
HRESULT Image::init(const char * fileName, int width, int height, BOOL isTrans, COLORREF transColor)
{
	if (_imageInfo != NULL) this->release();//�� ���� �ƴϴ�?

	//DC ��������
	HDC hdc = GetDC(_hWnd);

	//�̹��� ���� ���� ������ �ʱ�ȭ
	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_FILE;//
	_imageInfo->resID = 0;//
	//1 ������
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);
	//2
	//3 �ε��Ʈ�� LoadBitmap
	//4 ���ҽ��� �������µ� �ٽ����� �Լ� MAKEINTRESOURCE
	//5 LoadImage
	//6 LR_LOADFROMFILE
	_imageInfo->hBit = (HBITMAP)LoadImage(_hInstance, fileName,IMAGE_BITMAP,width,height,LR_LOADFROMFILE);
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	_imageInfo->width = width;
	_imageInfo->height = height;

	int len = strlen(fileName);

	_fileName = new CHAR[len + 1]; //len+1 �� 1�� ��Ÿ���� �� �־��ִ°���
	strcpy_s(_fileName,len+1,fileName);//�����ؼ� �־��ְڴ�~ strcpy_s �������� �� �����ض�******************************
	//�����̸�
	//�Ѵ� ����ư�� �� �ȵ�? ����Ÿ���̶�� �ȵ�; �����Ϸ� ���忡���� ���丮�� �����ָ� �츮�� �˰ٴ�? ���ϳ����� �������� �ƴ��� ������ �ؼ� �־��ٷ� ����Ұ���
	_isTrans = isTrans;
	_transColor = transColor;

	//���ҽ� �����µ� �����ߴ�.
	if (_imageInfo->hBit == 0)
	{
		release();
		return E_FAIL;
	}

	//������ DC ����
	ReleaseDC(_hWnd, hdc);

	return S_OK;
}

HRESULT Image::init(const char * fileName, float x, float y, int width, int height, BOOL isTrans, COLORREF transColor)
{
	if (_imageInfo != NULL) this->release();//�� ���� �ƴϴ�?

	//DC ��������
	HDC hdc = GetDC(_hWnd);

	//�̹��� ���� ���� ������ �ʱ�ȭ
	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_FILE;//
	_imageInfo->resID = 0;//
	//1 ������
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);
	//2
	//3 �ε��Ʈ�� LoadBitmap
	//4 ���ҽ��� �������µ� �ٽ����� �Լ� MAKEINTRESOURCE
	//5 LoadImage
	//6 LR_LOADFROMFILE
	_imageInfo->hBit = (HBITMAP)LoadImage(_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);

	_imageInfo->x = x;
	_imageInfo->y = y;
	_imageInfo->width = width;
	_imageInfo->height = height;

	int len = strlen(fileName);

	_fileName = new CHAR[len + 1]; //len+1 �� 1�� ��Ÿ���� �� �־��ִ°���
	strcpy_s(_fileName, len + 1, fileName);//�����ؼ� �־��ְڴ�~ strcpy_s �������� �� �����ض�******************************
	//�����̸�
	//�Ѵ� ����ư�� �� �ȵ�? ����Ÿ���̶�� �ȵ�; �����Ϸ� ���忡���� ���丮�� �����ָ� �츮�� �˰ٴ�? ���ϳ����� �������� �ƴ��� ������ �ؼ� �־��ٷ� ����Ұ���
	_isTrans = isTrans;
	_transColor = transColor;

	//���ҽ� �����µ� �����ߴ�.
	if (_imageInfo->hBit == 0)
	{
		release();
		return E_FAIL;
	}

	//������ DC ����
	ReleaseDC(_hWnd, hdc);

	return S_OK;
}

HRESULT Image::init(const char* fileName, int width, int height, int maxFrameX, int maxFrameY, BOOL isTrans, COLORREF transColor)
{
	if (_imageInfo != NULL) this->release();

	HDC hdc = GetDC(_hWnd);

	//�̹��� ���� ���� ���� �� �ʱ�ȭ
	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_FILE;
	_imageInfo->resID = 0;
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);
	_imageInfo->hBit = (HBITMAP)LoadImage(_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE); //LR_LOADFROMFILE : �ε� ���ҽ�
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

	//���� �̸�
	_isTrans = isTrans;
	_transColor = transColor;

	// ���ҽ� �����µ� �����ߴ�.
	if (_imageInfo->hBit == 0)
	{
		release();
		return E_FAIL;
	}
	//DC ����
	ReleaseDC(_hWnd, hdc);
	return S_OK;
}

HRESULT Image::init(const char * fileName, float x, float y, int width, int height, int maxFrameX, int maxFrameY, BOOL isTrans, COLORREF transColor)
{
	if (_imageInfo != NULL) this->release();//�� ���� �ƴϴ�?

	//DC ��������
	HDC hdc = GetDC(_hWnd);

	//�̹��� ���� ���� ������ �ʱ�ȭ
	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_FILE;//
	_imageInfo->resID = 0;//
	//1 ������
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);
	//2
	//3 �ε��Ʈ�� LoadBitmap
	//4 ���ҽ��� �������µ� �ٽ����� �Լ� MAKEINTRESOURCE
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

	_fileName = new CHAR[len + 1]; //len+1 �� 1�� ��Ÿ���� �� �־��ִ°���
	strcpy_s(_fileName, len + 1, fileName);//�����ؼ� �־��ְڴ�~ strcpy_s �������� �� �����ض�******************************
	//�����̸�
	//�Ѵ� ����ư�� �� �ȵ�? ����Ÿ���̶�� �ȵ�; �����Ϸ� ���忡���� ���丮�� �����ָ� �츮�� �˰ٴ�? ���ϳ����� �������� �ƴ��� ������ �ؼ� �־��ٷ� ����Ұ���
	_isTrans = isTrans;
	_transColor = transColor;

	//���ҽ� �����µ� �����ߴ�.
	if (_imageInfo->hBit == 0)
	{
		release();
		return E_FAIL;
	}

	//������ DC ����
	ReleaseDC(_hWnd, hdc);
	return S_OK;
}

HRESULT Image::initForAlphaBlend(void)
{
	HDC hdc = GetDC(_hWnd);
	
	_blendFunc.BlendFlags = 0;			//ȥ�� ���İ��� �����ʰڴ� �������� �� 0���ξ��� �� 0�� ������� �ʰڴ�
	_blendFunc.AlphaFormat = 0;			//�ҽ��� ��Ʈ�� ȥ�ս�ų����? �̹�����ü�� ó���Ұ��� +�ҽ� �ؼ� �����Ұ���
	_blendFunc.BlendOp = AC_SRC_OVER;	//��Ʈ�� �츮���°� 34��Ʈ�� �Ұ���?64��Ʈ��? �츰������� �Ѵ� �������� OVER


	//�̹��� ���� ���� ������ �ʱ�ȭ
	_blendImage = new IMAGE_INFO;
	_blendImage->loadType = LOAD_FILE;
	_blendImage->resID = 0; 
	_blendImage->hMemDC = CreateCompatibleDC(hdc);
	_blendImage->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, _imageInfo->width, _imageInfo->height);
	_blendImage->hOBit = (HBITMAP)SelectObject(_blendImage->hMemDC, _blendImage->hBit);
	_blendImage->width = WINSIZE_X;
	_blendImage->height = WINSIZE_Y;

	//������ DC ����
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
	if (_imageInfo)//�̹��������� �����ִ��� ���� ����
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
//����(0,0 ������ ����)
void Image::render(HDC hdc)
{
	if (_isTrans)//���� ������?
	{
		//GdiTransparentBlt() : ��Ʈ���� �ҷ��Ë� Ư�������� �����ϰ� �������ִ� �Լ� -�׳� �������ܾ��ϰ� �״�� �̹����ҷ����°ͺ��� ����
		GdiTransparentBlt					// ������ �����ɶ����� �޸𸮿� �����Ұ��� -
		(
			hdc,							//������ ����� DC(ȭ��DC-ȭ�鿡 ������)
			0,								//����� ������ǥ x,y
			0,
			_imageInfo->width,				//������ �̹��� ũ�� ���μ���
			_imageInfo->height,
			_imageInfo->hMemDC,				//����� ��� �޸�DC
			0, 0,							//���� ��������
			_imageInfo->width,				//���� ���� ���μ��� ũ��
			_imageInfo->height,
			_transColor						//�����Ҷ� ������ ����
		);
	}
	else //�ʰ��� �Ŵ� ���� �Ȼ��� ��¡ - Ư�����������Ұ� ��� ��Ӻ���
	{
		//BitBlt() : DC���� �������� ���� ��Ӻ������ִ� �Լ�
		//SRCCOPY : �ҽ������� ��󿵿��� �����Ѵ�
		BitBlt(hdc, 0, 0, _imageInfo->width, _imageInfo->height, _imageInfo->hMemDC, 0, 0, SRCCOPY);
	}
}

void Image::render(HDC hdc, int destX, int destY)//int destX, int destY �� ����߰ڳ�~
{
	if (_isTrans)//���� ������?
	{
		//GdiTransparentBlt() : ��Ʈ���� �ҷ��Ë� Ư�������� �����ϰ� �������ִ� �Լ� -�׳� �������ܾ��ϰ� �״�� �̹����ҷ����°ͺ��� ����
		GdiTransparentBlt					// ������ �����ɶ����� �޸𸮿� �����Ұ��� -
		(
			hdc,							//������ ����� DC(ȭ��DC-ȭ�鿡 ������)
			destX,							//����� ������ǥ x,y
			destY,
			_imageInfo->width,				//������ �̹��� ũ�� ���μ���
			_imageInfo->height,
			_imageInfo->hMemDC,				//����� ��� �޸�DC
			0, 0,							//���� ��������
			_imageInfo->width,				//���� ���� ���μ��� ũ��
			_imageInfo->height,
			_transColor						//�����Ҷ� ������ ����
		);
	}
	else //�ʰ��� �Ŵ� ���� �Ȼ��� ��¡ - Ư�����������Ұ� ��� ��Ӻ���
	{
		//BitBlt() : DC���� �������� ���� ��Ӻ������ִ� �Լ�
		//SRCCOPY : �ҽ������� ��󿵿��� �����Ѵ�
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
	//���ĺ��� ó�� ����ϴ���? ��� ������ �ʱ�ȭ���� �ϰ����
	if (!_blendImage) initForAlphaBlend();

	_blendFunc.SourceConstantAlpha = alpha;

	if (_isTrans)//���� ���Ŵ�
	{
		//1.����ؾ� �� DC�� �׷��� �ִ� ������ ���� �̹����� �׸���.
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

		//2.���� �̹����� ����� ���� �� ���� �̹����� �׸���.
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
		//3. ���� �̹����� ȭ�鿡 �׸���.
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
	else//�ٷ� �׷���
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

	if (_isTrans)//���� ���Ŵ�
	{
		//1.����ؾ� �� DC�� �׷��� �ִ� ������ ���� �̹����� �׸���.
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

		//2.���� �̹����� ����� ���� �� ���� �̹����� �׸���.
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
		//3. ���� �̹����� ȭ�鿡 �׸���.
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
	else//�ٷ� �׷���
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
	if (_isTrans)//���� ������?
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
	//�̹��� ����ó��
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

	if (_isTrans)//���� ������?
	{
		//GdiTransparentBlt() : ��Ʈ���� �ҷ��Ë� Ư�������� �����ϰ� �������ִ� �Լ� -�׳� �������ܾ��ϰ� �״�� �̹����ҷ����°ͺ��� ����
		GdiTransparentBlt					// ������ �����ɶ����� �޸𸮿� �����Ұ��� -
		(
			hdc,							//������ ����� DC(ȭ��DC-ȭ�鿡 ������)
			destX,							//����� ������ǥ x,y
			destY,
			_imageInfo->frameWidth,				//������ �̹��� ũ�� ���μ���
			_imageInfo->frameHeight,
			_imageInfo->hMemDC,				//����� ��� �޸�DC
			_imageInfo->currentFrameX * _imageInfo->frameWidth,							//���� ��������
			_imageInfo->currentFrameY * _imageInfo->frameHeight,							//���� ��������
			_imageInfo->frameWidth,				//���� ���� ���μ��� ũ��
			_imageInfo->frameHeight,
			_transColor						//�����Ҷ� ������ ����
		);
	}
	else //�ʰ��� �Ŵ� ���� �Ȼ��� ��¡ - Ư�����������Ұ� ��� ��Ӻ���
	{
		//BitBlt() : DC���� �������� ���� ��Ӻ������ִ� �Լ�
		//SRCCOPY : �ҽ������� ��󿵿��� �����Ѵ�
		BitBlt(hdc, destX, destY, _imageInfo->frameWidth, _imageInfo->frameHeight, _imageInfo->hMemDC, 
			_imageInfo->currentFrameX * _imageInfo->frameWidth, _imageInfo->currentFrameY * _imageInfo->frameHeight, SRCCOPY);
	}
}

