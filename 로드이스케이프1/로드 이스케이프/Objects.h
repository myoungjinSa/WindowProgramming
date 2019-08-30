#pragma once
#include "List_Bmp.h"

#define FPS 30//Frame Per Second

#define SEC_FRAME_CYCLE 1//1 FrameCycle Per 1 Sec

////////////�ӽ� ������Ʈ ����////////////
#define COLOR_FLOG RGB(181, 230, 29
#define COLOR_TUTTLE RGB(34, 177, 76)
#define COLOR_WOOD RGB(156, 97, 65)
#define COLOR_CAR_YELLOW RGB(255, 255, 128)
#define COLOR_CAR_RED RGB(255, 62, 62)
#define COLOR_CAR_BLUE RGB(55, 117, 244)
#define COLOR_CAR_GRAY RGB(55, 61, 83)
//////////////////////////////////////////
//�ӽú���(��������Ʈ�� �����Ƿ� �� ������Ʈ���� ������ �ο��ϱ� ����)
COLORREF ObjectsColor = NULL;

//RECT�� �⺻���� �ϴ� Object
class OBJECT
{
private:
	//Frame by bmp ����
	HBITMAP *hBitMap;
	BITMAP *BitMap;
	DOUBLE AnimateSec;//Frame ��� �ð�
	LONG FrameMax;//�Ҵ�� BitMap�� �� Frame ����
	LONG Count_IMG;//���� ��µǰ� �ִ� Frame

public:
	//������
	OBJECT *NextData;

	//���η� ���������Ͽ� ���ҵ� ������ŭ �̹��� ���� ���
	size_t DivisionColumn;

	//������Ʈ ȸ����(�̹��� ȸ����)
	DOUBLE Angle;

	DOUBLE FrameCount;//FrameCount += FrameMax / (FPS * AnimateSec); <-FPS�� ������� AnimateSec�� ���� Frame���� (��, FrameCount�� FPS�� 2���� ���� ��µ��� �ʰ� �ǳʶٴ� Frame�� �߻��Ѵ�.)
	LONG StartFrame;//bmp Start index <- ù��° �̹������Ͱ� �ƴ�, �̹��� �迭 �߿� ������ index���� MaxFrame������ ���̰� �ϱ� ���� �뵵.
	//bmp���� ���ܽ�ų ����(Ȥ�� Rectangle() ����)
	COLORREF BackGroundColor;

	//����
	POINT CenterPoint;
	//���� �� ũ��
	RECT Region;
	SIZE Size;
	OBJECT()
	{
		//��� ����
		NextData = nullptr;

		//Frame����
		hBitMap = nullptr;
		BitMap = nullptr;
		AnimateSec = 0.0;
		StartFrame = 0;
		FrameMax = 0;
		FrameCount = 0.0;
		Count_IMG = 0;
		BackGroundColor = NULL;
		DivisionColumn = 1;

		//��ġ �� ũ�� ����
		Angle = 0.0;
		CenterPoint.x = 0;
		CenterPoint.y = 0;
		Size.cx = 0;
		Size.cy = 0;
		Region.bottom = 0;
		Region.left = 0;
		Region.right = 0;
		Region.top = 0;
	}
	//ũ�� �����ϱ� (Focus Center)
	void SetSize(SIZE Size)
	{
		this->Size.cx = Size.cx;
		this->Size.cy = Size.cy;
		Region.left = CenterPoint.x - Size.cx / 2;
		Region.right = CenterPoint.x + Size.cx / 2;
		Region.top = CenterPoint.y - Size.cy / 2;
		Region.bottom = CenterPoint.y + Size.cy / 2;
	}
	//ũ�� �ø��ų� ���̱� (Focus Center)
	void AddSize(SIZE Size)
	{
		this->Size.cx += Size.cx;
		this->Size.cy += Size.cy;
		SetSize(Size);
	}
	//��ġ �����ϱ� (Focus LeftTop)
	void SetLTPos(POINT LTPoint)
	{
		CenterPoint.x = LTPoint.x + Size.cx / 2;
		CenterPoint.y = LTPoint.y + Size.cy / 2;
		Region.left = LTPoint.x;
		Region.right = Region.left + Size.cx;
		Region.top = LTPoint.y;
		Region.bottom = Region.top + Size.cy;
	}
	//��ġ �����ϱ� (Focus RightBottom)
	void SetRBPos(POINT RBPoint)
	{
		CenterPoint.x = RBPoint.x - Size.cx / 2;
		CenterPoint.y = RBPoint.y - Size.cy / 2;
		Region.right = RBPoint.x;
		Region.left = Region.right - Size.cx;
		Region.bottom = RBPoint.y;
		Region.top = Region.bottom - Size.cy;
	}
	//��ġ �����ϱ� (Focus Center)
	void SetCenterPos(POINT CenterPoint)
	{
		this->CenterPoint.x = CenterPoint.x;
		this->CenterPoint.y = CenterPoint.y;
		Region.left = CenterPoint.x - Size.cx / 2;
		Region.right = CenterPoint.x + Size.cx / 2;
		Region.top = CenterPoint.y - Size.cy / 2;
		Region.bottom = CenterPoint.y + Size.cy / 2;
	}
	//�̵��ϱ�
	void Move(LONG dx, LONG dy)
	{
		CenterPoint.x += dx;
		CenterPoint.y += dy;
		Region.left += dx;
		Region.top += dy;
		Region.right += dx;
		Region.bottom += dy;
	}

	//�̹��� ��Ͽ��� �̹��� ��� ��ġ �����ϱ�(<- �̹��� ����� �̹��� ������ ��Ȯ�� �˰� �־�� ��.)
	void SetFrameStart(LONG StartFrame)
	{
		this->StartFrame = StartFrame;
		FrameCount = StartFrame;
	}

	//������Ʈ�� bmp ����ϱ�(call by reference)
	void SetImage(HBITMAP *hBitMap, BITMAP *BitMap, COLORREF BackGroundColor, size_t DivisionColumn, LONG StartFrame, LONG FrameMax, DOUBLE AnimateSec)
	{
		this->hBitMap = hBitMap;
		this->BitMap = BitMap;
		this->BackGroundColor = BackGroundColor;
		this->StartFrame = StartFrame;
		this->DivisionColumn = DivisionColumn;
		FrameCount = StartFrame;
		this->FrameMax = FrameMax;
		this->AnimateSec = AnimateSec;
	}
	//�׸���
	void Draw(HDC hdc)
	{
		//��Ʈ���� �������
		if (hBitMap != nullptr && BitMap != nullptr)
		{
			HDC hBitmapDC;

			//FPS�� ������� AnimateSec���� image ī��Ʈ �����ϱ�
			FrameCount += (DOUBLE)FrameMax / (FPS * AnimateSec);
			if (FrameCount >= StartFrame + FrameMax)
				FrameCount = StartFrame;
			Count_IMG = (LONG)FrameCount;

			hBitmapDC = CreateCompatibleDC(hdc);
			SelectObject(hBitmapDC, hBitMap[Count_IMG]);

			//����ũ DC �� ��Ʈ�� ����
			COLORREF oldBkColor = SetBkColor(hBitmapDC, BackGroundColor);
			HDC maskBitmapDC = CreateCompatibleDC(NULL);
			HBITMAP maskHBitmap = CreateBitmap(BitMap[Count_IMG].bmWidth, BitMap[Count_IMG].bmHeight, 1, 1, NULL);
			SelectObject(maskBitmapDC, maskHBitmap);
			BitBlt(maskBitmapDC, 0, 0, BitMap[Count_IMG].bmWidth, BitMap[Count_IMG].bmHeight, hBitmapDC, 0, 0, NOTSRCCOPY);
			SetBkColor(hBitmapDC, oldBkColor);

			//ȸ����ȯ�� �ʿ��� ��/��, ��/��, ��/�� ����Ʈ
			POINT Point_TranceRotation[COUNT_POINT_TRANCE_ROTATION];
			
			for (size_t i = 0; i < DivisionColumn; i++)
			{
				RotateBit(Angle,//ȸ����
				{ Region.left + (Size.cx / (LONG)(DivisionColumn + 1)) * ((LONG)i + 1), CenterPoint.y },//�߽���ǥ
				{ Size.cx / (LONG)DivisionColumn, Size.cy },//ũ��
					Point_TranceRotation);//��ĺ�ȯ-ȸ��
				PlgBlt(hdc, Point_TranceRotation, hBitmapDC, 0, 0, BitMap[Count_IMG].bmWidth, BitMap[Count_IMG].bmHeight, maskHBitmap, NULL, NULL);
				//�Ӽ�->��Ŀ->����ٿ� msimg32.lib�� �������� �� �� �ִ� �Լ��̴�.
				/*TransparentBlt(hdc, Region.left + (Size.cx / (LONG)DivisionColumn) * (LONG)i, Region.top, (Size.cx / (LONG)DivisionColumn), Size.cy,
					hBitmapDC, 0, 0, BitMap[Count_IMG].bmWidth, BitMap[Count_IMG].bmHeight, BackGroundColor);*/
			}
			DeleteObject(maskHBitmap);
			DeleteDC(maskBitmapDC);
			DeleteDC(hBitmapDC);
		}
		//��Ʈ���� ���� ��� �簢���� �׷��ش�.
		else
		{
			HBRUSH hBrush, oldBrush;
			hBrush = CreateSolidBrush(BackGroundColor);
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			SelectObject(hdc, GetStockObject(WHITE_PEN));
			for (size_t i = 0; i < DivisionColumn; i++)
				Rectangle(hdc, Region.left + (Size.cx / (LONG)DivisionColumn) * (LONG)i, Region.top, Region.left + (Size.cx / (LONG)DivisionColumn) * (LONG)(i + 1), Region.bottom);
			SelectObject(hdc, oldBrush);
			DeleteObject(hBrush);
		}
	}
	~OBJECT() {}
};