#pragma once
#include "List_Bmp.h"

#define FPS 30//Frame Per Second

#define SEC_FRAME_CYCLE 1//1 FrameCycle Per 1 Sec

////////////임시 오브젝트 색상////////////
#define COLOR_FLOG RGB(181, 230, 29
#define COLOR_TUTTLE RGB(34, 177, 76)
#define COLOR_WOOD RGB(156, 97, 65)
#define COLOR_CAR_YELLOW RGB(255, 255, 128)
#define COLOR_CAR_RED RGB(255, 62, 62)
#define COLOR_CAR_BLUE RGB(55, 117, 244)
#define COLOR_CAR_GRAY RGB(55, 61, 83)
//////////////////////////////////////////
//임시변수(스프라이트가 없으므로 각 오브젝트들의 색상을 부여하기 위함)
COLORREF ObjectsColor = NULL;

//RECT를 기본으로 하는 Object
class OBJECT
{
private:
	//Frame by bmp 관련
	HBITMAP *hBitMap;
	BITMAP *BitMap;
	DOUBLE AnimateSec;//Frame 재생 시간
	LONG FrameMax;//할당된 BitMap의 총 Frame 갯수
	LONG Count_IMG;//현재 출력되고 있는 Frame

public:
	//노드관련
	OBJECT *NextData;

	//세로로 영역분할하여 분할된 영역만큼 이미지 복사 출력
	size_t DivisionColumn;

	//오브젝트 회전각(이미지 회전각)
	DOUBLE Angle;

	DOUBLE FrameCount;//FrameCount += FrameMax / (FPS * AnimateSec); <-FPS에 상관없이 AnimateSec초 동안 Frame증가 (단, FrameCount가 FPS의 2배일 경우는 출력되지 않고 건너뛰는 Frame이 발생한다.)
	LONG StartFrame;//bmp Start index <- 첫번째 이미지부터가 아닌, 이미지 배열 중에 임의의 index부터 MaxFrame까지만 보이게 하기 위한 용도.
	//bmp에서 제외시킬 색상(혹은 Rectangle() 색상)
	COLORREF BackGroundColor;

	//중점
	POINT CenterPoint;
	//영역 및 크기
	RECT Region;
	SIZE Size;
	OBJECT()
	{
		//노드 관련
		NextData = nullptr;

		//Frame관련
		hBitMap = nullptr;
		BitMap = nullptr;
		AnimateSec = 0.0;
		StartFrame = 0;
		FrameMax = 0;
		FrameCount = 0.0;
		Count_IMG = 0;
		BackGroundColor = NULL;
		DivisionColumn = 1;

		//위치 및 크기 관련
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
	//크기 지정하기 (Focus Center)
	void SetSize(SIZE Size)
	{
		this->Size.cx = Size.cx;
		this->Size.cy = Size.cy;
		Region.left = CenterPoint.x - Size.cx / 2;
		Region.right = CenterPoint.x + Size.cx / 2;
		Region.top = CenterPoint.y - Size.cy / 2;
		Region.bottom = CenterPoint.y + Size.cy / 2;
	}
	//크기 늘리거나 줄이기 (Focus Center)
	void AddSize(SIZE Size)
	{
		this->Size.cx += Size.cx;
		this->Size.cy += Size.cy;
		SetSize(Size);
	}
	//위치 지정하기 (Focus LeftTop)
	void SetLTPos(POINT LTPoint)
	{
		CenterPoint.x = LTPoint.x + Size.cx / 2;
		CenterPoint.y = LTPoint.y + Size.cy / 2;
		Region.left = LTPoint.x;
		Region.right = Region.left + Size.cx;
		Region.top = LTPoint.y;
		Region.bottom = Region.top + Size.cy;
	}
	//위치 지정하기 (Focus RightBottom)
	void SetRBPos(POINT RBPoint)
	{
		CenterPoint.x = RBPoint.x - Size.cx / 2;
		CenterPoint.y = RBPoint.y - Size.cy / 2;
		Region.right = RBPoint.x;
		Region.left = Region.right - Size.cx;
		Region.bottom = RBPoint.y;
		Region.top = Region.bottom - Size.cy;
	}
	//위치 지정하기 (Focus Center)
	void SetCenterPos(POINT CenterPoint)
	{
		this->CenterPoint.x = CenterPoint.x;
		this->CenterPoint.y = CenterPoint.y;
		Region.left = CenterPoint.x - Size.cx / 2;
		Region.right = CenterPoint.x + Size.cx / 2;
		Region.top = CenterPoint.y - Size.cy / 2;
		Region.bottom = CenterPoint.y + Size.cy / 2;
	}
	//이동하기
	void Move(LONG dx, LONG dy)
	{
		CenterPoint.x += dx;
		CenterPoint.y += dy;
		Region.left += dx;
		Region.top += dy;
		Region.right += dx;
		Region.bottom += dy;
	}

	//이미지 목록에서 이미지 출력 위치 지정하기(<- 이미지 목록의 이미지 갯수를 정확히 알고 있어야 함.)
	void SetFrameStart(LONG StartFrame)
	{
		this->StartFrame = StartFrame;
		FrameCount = StartFrame;
	}

	//오브젝트에 bmp 등록하기(call by reference)
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
	//그리기
	void Draw(HDC hdc)
	{
		//비트맵이 있을경우
		if (hBitMap != nullptr && BitMap != nullptr)
		{
			HDC hBitmapDC;

			//FPS에 상관없이 AnimateSec동안 image 카운트 변경하기
			FrameCount += (DOUBLE)FrameMax / (FPS * AnimateSec);
			if (FrameCount >= StartFrame + FrameMax)
				FrameCount = StartFrame;
			Count_IMG = (LONG)FrameCount;

			hBitmapDC = CreateCompatibleDC(hdc);
			SelectObject(hBitmapDC, hBitMap[Count_IMG]);

			//마스크 DC 및 비트맵 생성
			COLORREF oldBkColor = SetBkColor(hBitmapDC, BackGroundColor);
			HDC maskBitmapDC = CreateCompatibleDC(NULL);
			HBITMAP maskHBitmap = CreateBitmap(BitMap[Count_IMG].bmWidth, BitMap[Count_IMG].bmHeight, 1, 1, NULL);
			SelectObject(maskBitmapDC, maskHBitmap);
			BitBlt(maskBitmapDC, 0, 0, BitMap[Count_IMG].bmWidth, BitMap[Count_IMG].bmHeight, hBitmapDC, 0, 0, NOTSRCCOPY);
			SetBkColor(hBitmapDC, oldBkColor);

			//회전변환에 필요한 좌/상, 우/상, 좌/하 포인트
			POINT Point_TranceRotation[COUNT_POINT_TRANCE_ROTATION];
			
			for (size_t i = 0; i < DivisionColumn; i++)
			{
				RotateBit(Angle,//회전각
				{ Region.left + (Size.cx / (LONG)(DivisionColumn + 1)) * ((LONG)i + 1), CenterPoint.y },//중심좌표
				{ Size.cx / (LONG)DivisionColumn, Size.cy },//크기
					Point_TranceRotation);//행렬변환-회전
				PlgBlt(hdc, Point_TranceRotation, hBitmapDC, 0, 0, BitMap[Count_IMG].bmWidth, BitMap[Count_IMG].bmHeight, maskHBitmap, NULL, NULL);
				//속성->링커->명령줄에 msimg32.lib를 등록해줘야 쓸 수 있는 함수이다.
				/*TransparentBlt(hdc, Region.left + (Size.cx / (LONG)DivisionColumn) * (LONG)i, Region.top, (Size.cx / (LONG)DivisionColumn), Size.cy,
					hBitmapDC, 0, 0, BitMap[Count_IMG].bmWidth, BitMap[Count_IMG].bmHeight, BackGroundColor);*/
			}
			DeleteObject(maskHBitmap);
			DeleteDC(maskBitmapDC);
			DeleteDC(hBitmapDC);
		}
		//비트맵이 없을 경우 사각형을 그려준다.
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