#pragma once
#include "Game_DataManager.h"


#define WIDTH_WND 500
#define HEIGHT_WND 700

/*
게임 Win조건
- 주인공 오브젝트가 Stage[3]의 Line[1]에 도달하면 게임 Win
게임 Lose조건
- Line[0]의 목숨갯수가 0이 됬을 때 Lose
*/


//TimeProcedure(FPS(30) calls per 1 second)
void CALLBACK TimerProcedure(HWND hWnd, UINT uMessage, UINT idEvent, DWORD dwTime);
void CALLBACK IntroProcedure(HWND hWnd, UINT uMessage, UINT idEvent, DWORD dwTime);
//메세지 처리하는 윈도우프로시져
LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT iMessage, WPARAM wParameter, LPARAM lParameter);
LPCTSTR lpszClass = _T("Windows Class Name");//윈도우클래스의 이름
HINSTANCE g_hinst;//OS가 보내는 객체 핸들

DOUBLE count_Heart_Index;			//전역변수로 선언된 하트 배열 인덱스
//WinMain
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPreinstance, PSTR szCmdLine, int nCmdShow)
{
	HWND hWindow = NULL;//윈도우 핸들
	MSG Message;//os가 보내는 메세지 저장용
	WNDCLASSEX WindowClass;//윈도우 클래스
	g_hinst = hinstance;

	//윈도우 기본형태 지정
	WindowClass.cbSize = sizeof(WindowClass);
	WindowClass.style = CS_HREDRAW | CS_VREDRAW;
	WindowClass.lpfnWndProc = (WNDPROC)WindowProcedure;
	WindowClass.cbClsExtra = 0;
	WindowClass.cbWndExtra = 0;
	WindowClass.hInstance = hinstance;
	WindowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WindowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WindowClass.lpszMenuName = NULL;
	WindowClass.lpszClassName = lpszClass;
	WindowClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&WindowClass);//레지스터에 윈도우 등록

								  //윈도우 속성값 부여(윈도우 만들기)
	hWindow = CreateWindow(lpszClass, _T("Window Programming 어렵지 않아요!"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		WIDTH_WND, HEIGHT_WND, NULL, NULL, hinstance, NULL);

	ShowWindow(hWindow, nCmdShow);//윈도우 창 띄우기
	UpdateWindow(hWindow);

	//메세지루프
	while (GetMessage(&Message, NULL, 0, 0))
	{
		if (IsWindow(hWindow))
		{
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
	}

	return (int)Message.wParam;
}

#define ENVET_HELP 8
LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT iMessage, WPARAM wParameter, LPARAM lParameter)
{
	static HDC hdc, subdc, BackgroundDC;//DC의 핸들
	static HBITMAP hBitMap_Subdc, oldBitMap_Subdc;//더블버퍼링용 비트맵 핸들
	static PAINTSTRUCT lpPaint;//출력될 영역에 대한 정보를 저장한 구조체 공간에 대한 주소
	static HPEN hPen, oldPen;
	static TCHAR TextBuf[256];

	switch (iMessage)
	{
	case WM_CREATE://c++클래스의 생성자와 비슷한 느낌.
		srand((unsigned)time(NULL));
		GetClientRect(hWnd, &ClientRect);
		Unit.cx = ClientRect.bottom / COUNT_LINE;
		Unit.cy = ClientRect.bottom / COUNT_LINE;
		List_IMG.SetIMG(g_hinst);
		GameDataMANEGER.level = 1;		//레벨 1로 설정
		for (int i = 0; i < COUNT_LINE; i++)
		{
			Stage_Line[i].PosY = Unit.cy * i;
			Stage_Line[i].Width = ClientRect.right;
		}

		//헌터 설정
		Hunter.SetImage(13);
		Hunter.SetSize({ (LONG)(Unit.cx * 1.5) - 10, (LONG)(Unit.cy * 1.5) - 10 });

		//레벨 1(≒ 스테이지 1)
		SetStageLevel(hWnd, GameDataMANEGER.level);
		//개구리 설정
		Flog.Property.SetSize({ Unit.cx - 10, Unit.cy-10 });
		Flog.FlogDeathAnimation.SetImage(List_IMG.hbmp_AnimateFlog, List_IMG.bmp_AnimateFlog, COLOR_BACKGROUND_TRAN, 1, 12, 3, SEC_RESPAN);
		Flog.FlogDeathAnimation.SetSize(Flog.Property.Size);
		Flog.Property.SetImage(List_IMG.hbmp_AnimateFlog, List_IMG.bmp_AnimateFlog, COLOR_BACKGROUND_TRAN, 1, 0, COUNT_AnimateCycle_Flog, SEC_MOVE);
		Flog.Property.SetLTPos({ ClientRect.right / 2 - Flog.Property.Size.cx / 2, Stage_Line[COUNT_LINE - 2].PosY + 5 });
		//Flog.Property.SetLTPos({ ClientRect.right / 2 - Flog.Property.Size.cx / 2, Stage_Line[2].PosY + 5 });

		GameDataMANEGER.GameTime.SetTimeBar();
		//BGM 재생
		GameDataMANEGER.GameSound.PlaySOUND(0);

		SetTimer(hWnd, 1, 1000 / FPS, (TIMERPROC)TimerProcedure);
		SetTimer(hWnd, ENVET_HELP, 700, (TIMERPROC)IntroProcedure);//HELP 애니메이션
		break;
	case WM_LBUTTONDOWN:
		if (!GameDataMANEGER.GameStart)
		{
			MousePoint.x = LOWORD(lParameter);
			MousePoint.y = HIWORD(lParameter);
			GameDataMANEGER.CheckByClick();
		}
		break;
	case WM_MOUSEMOVE:
		if (!GameDataMANEGER.GameStart)
		{
			MousePoint.x = LOWORD(lParameter);
			MousePoint.y = HIWORD(lParameter);
			GameDataMANEGER.Check_CollisionButton();
		}
		break;
	case WM_KEYDOWN:
		KeyBuffer[wParameter] = TRUE;
		break;
	case WM_KEYUP:
		KeyBuffer[wParameter] = FALSE;
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &lpPaint);
		subdc = CreateCompatibleDC(hdc);
		hBitMap_Subdc = CreateCompatibleBitmap(hdc, ClientRect.right, ClientRect.bottom);
		oldBitMap_Subdc = (HBITMAP)SelectObject(subdc, hBitMap_Subdc);

		if (GameDataMANEGER.GameStart)//게임 화면
		{
			BackgroundDC = CreateCompatibleDC(subdc);

			//배경 상단
			SelectObject(BackgroundDC, List_IMG.hbmp_BackGround[0]);
			StretchBlt(subdc, ClientRect.left, ClientRect.top, ClientRect.right, Unit.cy + 1, BackgroundDC, 0, 0, List_IMG.bmp_BackGround[0].bmWidth, List_IMG.bmp_BackGround[0].bmHeight, SRCCOPY);

			//사냥꾼출력
			Hunter.Draw(subdc);

			//배경 하단
			SelectObject(BackgroundDC, List_IMG.hbmp_BackGround[1]);
			StretchBlt(subdc, ClientRect.left, ClientRect.top + Unit.cy, ClientRect.right, (ClientRect.bottom - Unit.cy + 5) - (ClientRect.bottom - (Stage_Line[COUNT_LINE - 1].PosY + Unit.cy)), BackgroundDC, 0, 0, List_IMG.bmp_BackGround[1].bmWidth, List_IMG.bmp_BackGround[1].bmHeight, SRCCOPY);

			DeleteDC(BackgroundDC);

			//여기에 그리기를 함.
			ITEM *DrawItem = nullptr;
			for (size_t i = 0; i < COUNT_LINE; i++)
			{
				if (8 <= i && i <= 13)
				{
					if (DrawItem = List_Item.SearchItemByPosY(Stage_Line[i].PosY + 5))
						DrawItem->Property.Draw(subdc);
				}
				Stage_Line[i].Draw_Objects(subdc);
				if (3 <= i && i <= 7)
				{
					if (DrawItem = List_Item.SearchItemByPosY(Stage_Line[i].PosY + 5))
						DrawItem->Property.Draw(subdc);
				}
				if (i == 8)
				{
					if (List_Item.CollisionItem != nullptr)
						List_Item.CollisionItem->Property.Draw(subdc);
					Flog.Draw(subdc);
				}
			}

			//화살 출력
			Hunter.Draw_Arrow(subdc);

			//게임 정보 출력(타임바, 목숨, 점수 등)
			GameDataMANEGER.Draw_Gameinfo(subdc, (LONG)count_Heart_Index);
		}
		else if (GameDataMANEGER.GameHelp)//게임 조작설명 화면
		{
			//배경 그리기
			static HBRUSH hBrush, oldBrush;
			hBrush = CreateSolidBrush(RGB(254, 254, 255));
			oldBrush = (HBRUSH)SelectObject(subdc, hBrush);
			FillRect(subdc, &ClientRect, hBrush);
			
			SelectObject(subdc, oldBrush);

			BackgroundDC = CreateCompatibleDC(subdc);

			//개구리 관련 이미지
			
			SelectObject(BackgroundDC, List_IMG.hbmp_AnimateFlog[0]);
			StretchBlt(subdc, ClientRect.left + Unit.cx, ClientRect.top + Unit.cy, List_IMG.bmp_AnimateFlog[0].bmWidth, List_IMG.bmp_AnimateFlog[0].bmHeight, BackgroundDC, 0, 0, List_IMG.bmp_AnimateFlog[0].bmWidth, List_IMG.bmp_AnimateFlog[0].bmHeight, SRCCOPY);

			//조작키 관련 이미지
			SelectObject(BackgroundDC, List_IMG.hbmp_Helpinfo_IMG[0]);
			BitBlt(subdc, ClientRect.left + Unit.cx, ClientRect.top + (Unit.cy*2), List_IMG.bmp_Helpinfo_IMG[0].bmWidth, List_IMG.bmp_Helpinfo_IMG[0].bmHeight, BackgroundDC, 0, 0, SRCCOPY);
			//아이템 관련 이미지
				
			SelectObject(BackgroundDC, List_IMG.hbmp_Gameitem[0]);
			BitBlt(subdc, ClientRect.left + (Unit.cx * 4), ClientRect.top + Unit.cy, List_IMG.bmp_Gameitem[0].bmWidth, List_IMG.bmp_Gameitem[0].bmHeight, BackgroundDC, 0, 0, SRCCOPY);
					
			SelectObject(BackgroundDC, List_IMG.hbmp_Gameitem[1]);
			BitBlt(subdc, ClientRect.left + (Unit.cx * 5), ClientRect.top + Unit.cy, List_IMG.bmp_Gameitem[1].bmWidth, List_IMG.bmp_Gameitem[1].bmHeight, BackgroundDC, 0, 0, SRCCOPY);

			SelectObject(BackgroundDC, List_IMG.hbmp_Gameitem[2]);
			BitBlt(subdc, ClientRect.left + (Unit.cx * 6), ClientRect.top + Unit.cy, List_IMG.bmp_Gameitem[2].bmWidth, List_IMG.bmp_Gameitem[2].bmHeight, BackgroundDC, 0, 0, SRCCOPY);
			
			//거미줄 관련 이미지

			SelectObject(BackgroundDC, List_IMG.hbmp_Gameitem[3]);									
			StretchBlt(subdc, ClientRect.left + (Unit.cx * 4), ClientRect.top + (Unit.cy * 3),Unit.cx,Unit.cy, BackgroundDC, 0, 0, List_IMG.bmp_Gameitem[3].bmWidth, List_IMG.bmp_Gameitem[3].bmHeight, SRCCOPY);

			//자동차 관련 이미지
			SelectObject(BackgroundDC, List_IMG.hbmp_Instance[0]);
			StretchBlt(subdc, ClientRect.left + Unit.cx , ClientRect.top + (Unit.cy * 6), List_IMG.bmp_Instance[3].bmWidth, List_IMG.bmp_Instance[3].bmHeight,BackgroundDC, 0, 0, List_IMG.bmp_Instance[3].bmWidth, List_IMG.bmp_Instance[3].bmHeight, SRCCOPY);

			//통나무 관련 이미지  (자동차에 오브젝트에 맞게 크기 수정)
			SelectObject(BackgroundDC, List_IMG.hbmp_Instance[10]);
			StretchBlt(subdc, ClientRect.left + (Unit.cx*4), ClientRect.top + (Unit.cy * 6), List_IMG.bmp_Instance[3].bmWidth, List_IMG.bmp_Instance[3].bmHeight, BackgroundDC, 0, 0, List_IMG.bmp_Instance[10].bmWidth, List_IMG.bmp_Instance[10].bmHeight, SRCCOPY);

			//거북이 관련 이미지
			SelectObject(BackgroundDC, List_IMG.hbmp_AnimateTuttle[0]);
			StretchBlt(subdc, ClientRect.left + (Unit.cx * 6), ClientRect.top + (Unit.cy * 6), List_IMG.bmp_Instance[3].bmWidth, List_IMG.bmp_Instance[3].bmHeight, BackgroundDC, 0, 0, List_IMG.bmp_AnimateTuttle[0].bmWidth, List_IMG.bmp_AnimateTuttle[0].bmHeight, SRCCOPY);

			//사냥꾼 관련 이미지
			SelectObject(BackgroundDC, List_IMG.hbmp_Instance[12]);
			StretchBlt(subdc, ClientRect.left + Unit.cx, ClientRect.top + (Unit.cy * 9), List_IMG.bmp_Instance[12].bmWidth, List_IMG.bmp_Instance[12].bmHeight, BackgroundDC, 0,0, List_IMG.bmp_Instance[12].bmWidth, List_IMG.bmp_Instance[12].bmHeight, SRCCOPY);

			//화살 관련 이미지
			SelectObject(BackgroundDC, List_IMG.hbmp_Instance[13]);
			StretchBlt(subdc, ClientRect.left + Unit.cx, ClientRect.top + (Unit.cy * 11), List_IMG.bmp_Instance[13].bmWidth*2, List_IMG.bmp_Instance[13].bmHeight*2, BackgroundDC, 0, 0, List_IMG.bmp_Instance[13].bmWidth, List_IMG.bmp_Instance[13].bmHeight, SRCCOPY);

			//타임바 관련 이미지
			SelectObject(BackgroundDC, List_IMG.hbmp_Gameinfo[1]);
			StretchBlt(subdc, ClientRect.left + Unit.cx, ClientRect.top + (Unit.cy * 13), List_IMG.bmp_Gameinfo[1].bmWidth, List_IMG.bmp_Gameinfo[1].bmHeight, BackgroundDC, 0, 0, List_IMG.bmp_Gameinfo[1].bmWidth, List_IMG.bmp_Gameinfo[1].bmHeight, SRCCOPY);

			SelectObject(BackgroundDC, List_IMG.hbmp_Gameinfo[2]);
			TransparentBlt(subdc, ClientRect.left + Unit.cx+(Unit.cx/2)+3, ClientRect.top + (Unit.cy * 13)+(Unit.cy/2)-3, List_IMG.bmp_Gameinfo[2].bmWidth, List_IMG.bmp_Gameinfo[2].bmHeight, BackgroundDC, 0, 0, List_IMG.bmp_Gameinfo[2].bmWidth, List_IMG.bmp_Gameinfo[2].bmHeight, RGB(255,255,255));


			HFONT hFont, oldFont;
			SIZE FontSize;

			hFont = CreateFont(15, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("휴먼둥근헤드라인"));
			oldFont = (HFONT)SelectObject(subdc, hFont);
			//아이템 관련 텍스트
			wsprintf(TextBuf, TEXT("아이템을 먹으면 보너스 점수 상승"));									
			GetTextExtentPoint(hdc, TextBuf, (int)_tcslen(TextBuf), &FontSize);
			
			SetBkMode(subdc, TRANSPARENT);
			SetTextColor(subdc, RGB(221, 0, 0));
			TextOut(subdc, ClientRect.left + (Unit.cx * 4), ClientRect.top + (Unit.cy*2), TextBuf, (int)_tcslen(TextBuf));
			
			//거미줄 관련 텍스트
			wsprintf(TextBuf, TEXT("거미줄에 걸리면 2초동안 속박"));										
			GetTextExtentPoint(hdc, TextBuf, (int)_tcslen(TextBuf), &FontSize);

			SetBkMode(subdc, TRANSPARENT);
			SetTextColor(subdc, RGB(221, 160, 30));

			TextOut(subdc, ClientRect.left + (Unit.cx * 4), ClientRect.top + (Unit.cy * 5), TextBuf, (int)_tcslen(TextBuf));

			//자동차 관련 텍스트
			wsprintf(TextBuf, TEXT("자동차에 부딫히면 시작점으로 리스폰"));
			GetTextExtentPoint(hdc, TextBuf, (int)_tcslen(TextBuf), &FontSize);

			SetBkMode(subdc, TRANSPARENT);
			SetTextColor(subdc, RGB(2,225, 30));

			TextOut(subdc, ClientRect.left + Unit.cx, ClientRect.top + (Unit.cy * 7), TextBuf, (int)_tcslen(TextBuf));


			//통나무 관련 텍스트
			wsprintf(TextBuf, TEXT("통나무와 거북이에 올라서 강을 건널수 있음"));
			GetTextExtentPoint(hdc, TextBuf, (int)_tcslen(TextBuf), &FontSize);

			SetBkMode(subdc, TRANSPARENT);
			SetTextColor(subdc, RGB(21, 0, 230));

			TextOut(subdc, ClientRect.left + Unit.cx, ClientRect.top + (Unit.cy * 8), TextBuf, (int)_tcslen(TextBuf));


			//사냥꾼 관련 텍스트
			wsprintf(TextBuf, TEXT("사냥꾼은 레벨 2부터 출현"));
			GetTextExtentPoint(hdc, TextBuf, (int)_tcslen(TextBuf), &FontSize);

			SetBkMode(subdc, TRANSPARENT);
			SetTextColor(subdc, RGB(221, 10, 230));

			TextOut(subdc, ClientRect.left + (Unit.cx*3), ClientRect.top + (Unit.cy * 9), TextBuf, (int)_tcslen(TextBuf));
			
			//화살 관련 텍스트

			wsprintf(TextBuf, TEXT("화살에 맞으면 생명 1감소 후 시작점으로 이동"));
			GetTextExtentPoint(hdc, TextBuf, (int)_tcslen(TextBuf), &FontSize);

			SetBkMode(subdc, TRANSPARENT);
			SetTextColor(subdc, RGB(100, 100, 230));

			TextOut(subdc, ClientRect.left + Unit.cx, ClientRect.top + (Unit.cy * 12), TextBuf, (int)_tcslen(TextBuf));

			//타임바 관련 텍스트
			wsprintf(TextBuf, TEXT("각 레벨에 정해진 시간부여   제한시간이 다 지나면 게임 오버"));
			GetTextExtentPoint(hdc, TextBuf, (int)_tcslen(TextBuf), &FontSize);

			SetBkMode(subdc, TRANSPARENT);
			SetTextColor(subdc, RGB(0, 1, 5));

			TextOut(subdc, ClientRect.left + Unit.cx, ClientRect.top + (Unit.cy * 14)+10, TextBuf, (int)_tcslen(TextBuf));

			SelectObject(subdc, oldFont);
			DeleteObject(hFont);

			
			//SelectObject(BackgroundDC, List_IMG.hbmp_);

			DeleteDC(BackgroundDC);

			//게임조작 설명 출력
			GameDataMANEGER.Draw_Helpinfo(subdc);
		}
		else//인트로화면
		{
			//배경 그리기
			BackgroundDC = CreateCompatibleDC(subdc);
			SelectObject(BackgroundDC, List_IMG.hbmp_Gameinfo[0]);
			StretchBlt(subdc, ClientRect.left, ClientRect.top, ClientRect.right, ClientRect.bottom, BackgroundDC, 0, 0, List_IMG.bmp_Gameinfo[0].bmWidth, List_IMG.bmp_Gameinfo[0].bmHeight, SRCCOPY);
			DeleteDC(BackgroundDC);

			//인트로 정보 출력(Start 버튼, Help 버튼)
			GameDataMANEGER.Draw_introinfo(subdc);
		}
		BitBlt(hdc, 0, 0, ClientRect.right, ClientRect.bottom, subdc, 0, 0, SRCCOPY);
		SelectObject(subdc, oldBitMap_Subdc);
		DeleteObject(hBitMap_Subdc);
		DeleteDC(subdc);
		EndPaint(hWnd, &lpPaint);
		break;
	case WM_DESTROY:
		GameDataMANEGER.GameSound.~GAMESOUND();
		KillTimer(hWnd, 1);
		KillTimer(hWnd, ENVET_HELP);
		KillTimer(hWnd, EVENT_APPEAR);
		KillTimer(hWnd, EVENT_AIMING);
		KillTimer(hWnd, EVENT_FIXED_ARROW);
		KillTimer(hWnd, EVENTE_RESPAN);
		KillTimer(hWnd, EVENTE_NEXTSTAGE);
		KillTimer(hWnd, EVENTE_GAMEOVER);
		KillTimer(hWnd, EVENT_BONDAGE);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, iMessage, wParameter, lParameter);//case에서 처리하지 못한 메세지를 처리하는 함수
}

void CALLBACK IntroProcedure(HWND hWnd, UINT uMessage, UINT idEvent, DWORD dwTime) {

	GameDataMANEGER.Image_index++;
	if (GameDataMANEGER.Image_index % 9 == 0) {
		GameDataMANEGER.Image_index = 0;
	}
	InvalidateRect(hWnd, NULL,FALSE);
}

void CALLBACK TimerProcedure(HWND hWnd, UINT uMessage, UINT idEvent, DWORD dwTime)
{
	if (GameDataMANEGER.GameStart && !GameDataMANEGER.StageClear && !GameDataMANEGER.StageFail)
	{
		//생명 비트맵 인덱스 증가
		count_Heart_Index += 0.1;
		if (count_Heart_Index>5.0) {
			count_Heart_Index = 0;
		}

		//헌터 리스폰 간격부여
		if (GameDataMANEGER.level > 1 && Hunter.Appear == FALSE)
		{
			SetTimer(hWnd, EVENT_APPEAR, 1000 * SEC_APPEAR_INTERVAL, (TIMERPROC)Appear);
			Hunter.Appear = TRUE;
		}

		POINT HunterCenterPoint = Hunter.GetCenterPoint();
		LONG DistY = (Stage_Line[1].PosY - Unit.cy / 2) - HunterCenterPoint.y;
		LONG DistX = Flog.Property.CenterPoint.x - HunterCenterPoint.x;
		//헌터 리스폰 위치 지정
		if (Hunter.Appear && !Hunter.Shoot && !Hunter.Hide)
		{
			Hunter.Move((LONG)(DistX*0.01), (LONG)(DistY * 0.1));
			Hunter.SetDirection_Arrow(Flog.Property.CenterPoint);
		}
		//헌터 숨기
		else if (Hunter.Shoot)
		{
			DistY = Stage_Line[2].PosY - HunterCenterPoint.y;
			Hunter.Move(0, (LONG)(DistY * 0.1));
		}

		//조준플래그 발동
		if (abs(DistY) <= 10 && !Hunter.Aiming && !Hunter.Shoot && !Hunter.Arrow_Move)
		{
			Hunter.Aiming = TRUE;
			SetTimer(hWnd, EVENT_AIMING, 1000 * SEC_AIMING, (TIMERPROC)Aiming);
		}
		//완전숨은 상태플래그 발동
		else if (abs(DistY) <= 10 && Hunter.Appear && Hunter.Shoot)
		{
			Hunter.Appear = FALSE;
			Hunter.Shoot = FALSE;
			Hunter.Hide = TRUE;
		}

		//화살 이동
		if (Hunter.Arrow_Move && !Hunter.Arrow_Collision)
		{
			LONG dx = (LONG)(SPEED_ARROW * cos(TORAD(Hunter.GetAngle())));
			LONG dy = (LONG)(SPEED_ARROW * sin(TORAD(Hunter.GetAngle())));
			Hunter.Move_Arrow(dx, dy);
		}

		//화살과의 충돌체크
		RECT Line8 = { ClientRect.left, Stage_Line[8].PosY + Unit.cy / 2, ClientRect.right, Stage_Line[9].PosY };
		if (Hunter.Arrow_Collision == FALSE && (Hunter.CollsionByTarget(Line8) || Hunter.CollsionByTarget(Flog.Property.Region)))
		{
			Hunter.Arrow_Collision = TRUE;
			if (Flog.Death == FALSE && Hunter.CollsionByTarget(Flog.Property.Region))
			{
				Flog.Death = TRUE;
				Flog.CollsionType = 1;
				GameDataMANEGER.Life--;
				GameDataMANEGER.StackScore /= 2;
				Flog.FlogDeathAnimation.SetCenterPos(Flog.Property.CenterPoint);
				PlaySound(TEXT("Resource\\Sound\\개구리4.WAV"), NULL, SND_ASYNC);//죽었을 때 소리
				if(GameDataMANEGER.Life) SetTimer(hWnd, EVENTE_RESPAN, 1000 * SEC_RESPAN, (TIMERPROC)Respan_Flog);
			}
			
			SetTimer(hWnd, EVENT_FIXED_ARROW, 1000 * SEC_FIXED_ARROW, (TIMERPROC)Disappear_Arrow);
		}
		else if (Hunter.GetArrowRect().right < ClientRect.left || Hunter.GetArrowRect().left > ClientRect.right)
			Hunter.Arrow_Move = FALSE;

		if (!Flog.Death)
		{
			//개구리 이동방향 설정
			if (!Flog.Bondage && Flog.Direction == 0)
			{
				if (KeyBuffer['W'])//↑
				{
					Flog.BeforeMovePoint = Flog.Property.CenterPoint;
					Flog.Direction = 3;
					Flog.Property.SetFrameStart(COUNT_AnimateCycle_Flog * 0);
					PlaySound(TEXT("Resource\\Sound\\FROG.WAV"), NULL, SND_ASYNC);
				}
				else if (KeyBuffer['A'])//←
				{
					Flog.BeforeMovePoint = Flog.Property.CenterPoint;
					Flog.Direction = 1;
					Flog.Property.SetFrameStart(COUNT_AnimateCycle_Flog * 1);
					PlaySound(TEXT("Resource\\Sound\\FROG.WAV"), NULL, SND_ASYNC);
				}
				else if (KeyBuffer['S'])//↓
				{
					Flog.BeforeMovePoint = Flog.Property.CenterPoint;
					Flog.Direction = 4;
					Flog.Property.SetFrameStart(COUNT_AnimateCycle_Flog * 2);
					PlaySound(TEXT("Resource\\Sound\\FROG.WAV"), NULL, SND_ASYNC);
				}
				else if (KeyBuffer['D'])//→
				{
					Flog.BeforeMovePoint = Flog.Property.CenterPoint;
					Flog.Direction = 2;
					Flog.Property.SetFrameStart(COUNT_AnimateCycle_Flog * 3);
					PlaySound(TEXT("Resource\\Sound\\FROG.WAV"), NULL, SND_ASYNC);
				}
			}

			//아이템과 개구리와 충돌체크
			if (List_Item.CollisionItem == nullptr)
				List_Item.CollisionItem = List_Item.CollsionWithPop(Flog.Property.Region);
			if(List_Item.CollisionItem != nullptr)
			{
				if (List_Item.CollisionItem->Type == 4)//거미줄
				{
					if (List_Item.CollisionItem->Inner_Window)
					{
						SetTimer(hWnd, EVENT_BONDAGE, 1000 * SEC_BONDAGE, (TIMERPROC)RemoveSpiderWeb);
						List_Item.CollisionItem->Inner_Window = FALSE;
					}
					List_Item.CollisionItem->Property.Move(List_Item.CollisionItem->SpeedX, 0);
				}
				else
				{
					GameDataMANEGER.StackScore += SCORE_ITEM;
					delete List_Item.CollisionItem;
					List_Item.CollisionItem = nullptr;
				}
			}

			if (!Flog.Bondage)
			{
				if (!Flog.JumpIntervalFlag || List_Item.CollisionItem == nullptr)
					Flog.Move();//개구리 이동
				else
				{
					Flog.Bondage = TRUE;
					Flog.initMoveProperty();
				}
			}
		}

		for (size_t i = 0; i < COUNT_LINE; i++)
		{
			//각 라인별 오브젝트 이동
			Stage_Line[i].Move_Objects();

			//화살이 Fix
			if (Hunter.Arrow_Collision && 3 <= i && i <= 7 && Stage_Line[i].Collision_Objects(Hunter.GetArrowRect()))
				Hunter.Move_Arrow(Stage_Line[i].SpeedX, 0);
			else if (Hunter.Arrow_Collision && Stage_Line[3].PosY >= Hunter.GetArrowRect().bottom && Stage_Line[7].PosY <= Hunter.GetArrowRect().bottom && !Stage_Line[i].Collision_Objects(Hunter.GetArrowRect()))
				Hunter.Arrow_Move = FALSE;

			//오브젝트 위에 놓여져 있는 아이템 이동
			ITEM *DropItem = nullptr;
			OBJECT *CollisionObejct = nullptr;
			if (3 <= i && i <= 7)
			{
				if (DropItem = List_Item.SearchItemByPosY(Stage_Line[i].PosY + 5))
				{
					if (!DropItem->Drop)
					{
						if (CollisionObejct = Stage_Line[i].GetObjectsByCollision(DropItem->Property.Region))
						{
							POINT NewLTPos;
							NewLTPos.y = DropItem->Property.Region.top;
							NewLTPos.x = CollisionObejct->Region.left + rand() % (CollisionObejct->Size.cx - DropItem->Property.Size.cx);
							DropItem->SpeedX = Stage_Line[i].SpeedX;
							DropItem->Property.SetLTPos(NewLTPos);
							DropItem->Drop = TRUE;
						}
					}
					if(DropItem->Drop && Stage_Line[i].Collision_Objects(DropItem->Property.Region))
						DropItem->Property.Move(Stage_Line[i].SpeedX, 0);
				}
			}

			if (9 <= i && i <= 13 && Flog.Death == FALSE && Stage_Line[i].Collision_Objects(Flog.Property.Region))//Line[9] ~ Line[13]까지 각각의 오브젝트와 개구리와의 상호작용(충돌체크 후 리스폰 결정)
			{
				Flog.Death = TRUE;
				Flog.CollsionType = 2;
				GameDataMANEGER.Life--;
				GameDataMANEGER.StackScore /= 2;
				Flog.FlogDeathAnimation.SetCenterPos(Flog.Property.CenterPoint);
				PlaySound(TEXT("Resource\\Sound\\개구리4.WAV"), NULL, SND_ASYNC);//차와 충돌했을때 소리
				if (GameDataMANEGER.Life) SetTimer(hWnd, EVENTE_RESPAN, 1000 * SEC_RESPAN, (TIMERPROC)Respan_Flog);
			}
			//Line[3] ~ Line[7]까지 각각의 오브젝트와 개구리와의 상호작용 (충돌체크 후 리스폰 결정)
			else if (3 <= i && i <= 7 && Flog.Death == FALSE && Stage_Line[i].PosY <= Flog.Property.Region.top && Flog.Property.Region.bottom <= Stage_Line[i + 1].PosY && !(Stage_Line[i].Collision_Objects(Flog.Property.Region)))
			{
				Flog.Death = TRUE;
				Flog.CollsionType = 3;
				GameDataMANEGER.Life--;
				GameDataMANEGER.StackScore /= 2;
				Flog.FlogDeathAnimation.SetCenterPos(Flog.Property.CenterPoint);
				PlaySound(TEXT("Resource\\Sound\\water_splash2.wav"), NULL, SND_ASYNC);//물에 풍덩하는 소리
				if (GameDataMANEGER.Life) SetTimer(hWnd, EVENTE_RESPAN, 1000 * SEC_RESPAN, (TIMERPROC)Respan_Flog);
			}
			//Line[3] ~ Line[7]까지 각각의 오브젝트와 개구리와의 상호작용 (충돌체크 후 같이 이동 및 이동제한)
			else if (3 <= i && i <= 7 && Stage_Line[i].PosY <= Flog.Property.Region.top && Flog.Property.Region.bottom <= Stage_Line[i + 1].PosY && (Stage_Line[i].Collision_Objects(Flog.Property.Region)))
			{
				Flog.Property.Move(Stage_Line[i].SpeedX, 0);
				OBJECT *CollsionObject = Stage_Line[i].GetObjectsByCollision(Flog.Property.Region);
				if (CollsionObject != nullptr)
				{
					if (Flog.Property.Region.left < CollsionObject->Region.left)
					{
						Flog.Property.SetLTPos({ CollsionObject->Region.left, CollsionObject->Region.top });
						Flog.initMoveProperty();
						Flog.BeforeMovePoint = Flog.Property.CenterPoint;
					}
					else if (Flog.Property.Region.right > CollsionObject->Region.right)
					{
						Flog.Property.SetRBPos({ CollsionObject->Region.right, CollsionObject->Region.bottom });
						Flog.initMoveProperty();
						Flog.BeforeMovePoint = Flog.Property.CenterPoint;
					}
				}
			}
			else if (!GameDataMANEGER.StageClear && i == 1&&Stage_Line[i].PosY <= Flog.Property.Region.top&& Flog.Property.Region.bottom <= Stage_Line[i + 1].PosY )
			{
				GameDataMANEGER.level++;
				if (GameDataMANEGER.level > 3)
				{
					//게임 클리어
					Hunter.Appear = FALSE;
					Hunter.Hide = TRUE;
					Hunter.Shoot = FALSE;
					Hunter.Aiming = FALSE;
					Hunter.Arrow_Move = FALSE;
					Hunter.Arrow_Collision = FALSE;
					KillTimer(hWnd, EVENT_APPEAR);
					KillTimer(hWnd, EVENT_AIMING);
					KillTimer(hWnd, EVENT_FIXED_ARROW);
					KillTimer(hWnd, EVENT_APPEAR);
					GameDataMANEGER.init_All_GameData();
					SetStageLevel(hWnd, 1);
				}
				else
				{
					if (!GameDataMANEGER.GameTime.Check_TimeOut())
					{
						//스테이지 전환
						GameDataMANEGER.StageClear = TRUE;
						GameDataMANEGER.init_infoText();
						GameDataMANEGER.GameTime.Pause = TRUE;
						GameDataMANEGER.GameTime.ClearTime = GameDataMANEGER.GameTime.Get_RemainingTime();
						GameDataMANEGER.GameTime.Count_FPS = 0;
						GameDataMANEGER.StackScore = GameDataMANEGER.GameTime.ClearTime * 100;
						Flog.Property.SetFrameStart(0);
						Flog.initMoveProperty();
						Hunter.Appear = FALSE;
						Hunter.Hide = TRUE;
						Hunter.Shoot = FALSE;
						Hunter.Aiming = FALSE;
						Hunter.Arrow_Move = FALSE;
						Hunter.Arrow_Collision = FALSE;
						KillTimer(hWnd, EVENT_APPEAR);
						KillTimer(hWnd, EVENT_AIMING);
						KillTimer(hWnd, EVENT_FIXED_ARROW);
						KillTimer(hWnd, EVENT_APPEAR);
						SetTimer(hWnd, EVENTE_GAMEOVER, 1000 * SEC_STAGECLAER, (TIMERPROC)NextStage);
					}
				}
			}
		}


		//개구리 목숨이 0개가 됬을 때 혹은 타임오버가 됬을 때
		if (!GameDataMANEGER.StageFail && (!GameDataMANEGER.Life || GameDataMANEGER.GameTime.Check_TimeOut()))
		{
			//게임 실패
			GameDataMANEGER.StageFail = TRUE;
			GameDataMANEGER.init_infoText();
			GameDataMANEGER.GameTime.Pause = TRUE;
			GameDataMANEGER.GameTime.Count_FPS = 0;
			Hunter.Appear = FALSE;
			Hunter.Hide = TRUE;
			Hunter.Shoot = FALSE;
			Hunter.Aiming = FALSE;
			Hunter.Arrow_Move = FALSE;
			Hunter.Arrow_Collision = FALSE;
			KillTimer(hWnd, EVENT_APPEAR);
			KillTimer(hWnd, EVENT_AIMING);
			KillTimer(hWnd, EVENT_FIXED_ARROW);
			KillTimer(hWnd, EVENT_APPEAR);
			SetTimer(hWnd, EVENTE_GAMEOVER, 1000 * SEC_GAMEOVER, (TIMERPROC)GameOver);
		}

		//오브젝트 위에 놓여져 있는 아이템 이동범위 설정
		ITEM *CurrentItem = List_Item.ListTop;
		while (CurrentItem != nullptr)
		{
			if (CurrentItem->Inner_Window)
			{
				if (CurrentItem->Property.Region.left >= ClientRect.right
					|| CurrentItem->Property.Region.right <= ClientRect.left)
				{
					if (CurrentItem->Property.Region.left >= ClientRect.right)
					{
						POINT LTPos;
						LTPos.x = ClientRect.left - CurrentItem->Property.Size.cx;
						LONG Lineindex = 3 + rand() % 5;
						while(Stage_Line[Lineindex].SpeedX < 0)
							Lineindex = 3 + rand() % 5;
						LTPos.y = Stage_Line[Lineindex].PosY + 5;
						CurrentItem->Property.SetLTPos(LTPos);
					}
					else if (CurrentItem->Property.Region.right <= ClientRect.left)
					{
						POINT RBPos;
						RBPos.x = ClientRect.right + CurrentItem->Property.Size.cx;
						LONG Lineindex = 3 + rand() % 5;
						while (Stage_Line[Lineindex].SpeedX > 0)
							Lineindex = 3 + rand() % 5;
						RBPos.y = Stage_Line[Lineindex + 1].PosY - 5;
						CurrentItem->Property.SetRBPos(RBPos);
					}
					CurrentItem->Inner_Window = FALSE;
					CurrentItem->Drop = FALSE;
				}
			}
			else
			{
				if (ClientRect.left < CurrentItem->Property.CenterPoint.x
					&& CurrentItem->Property.CenterPoint.x < ClientRect.right)
					CurrentItem->Inner_Window = TRUE;
			}
			CurrentItem = CurrentItem->NextData;
		}

		//개구리와 화면 끝 부분과의 충돌체크
		if (Flog.Property.Region.left < ClientRect.left + 5)//←
		{
			Flog.Property.SetLTPos({ ClientRect.left + 5, Flog.Property.Region.top });
			Flog.initMoveProperty();
		}
		if (Flog.Property.Region.right > ClientRect.right - 5)//→
		{
			Flog.Property.SetRBPos({ ClientRect.right - 5, Flog.Property.Region.bottom });
			Flog.initMoveProperty();
		}
		if (Flog.Property.Region.top < Stage_Line[1].PosY + 5)//↑
		{
			Flog.Property.SetLTPos({ Flog.Property.Region.left, Stage_Line[1].PosY + 5 });
			Flog.initMoveProperty();
		}
		if (Flog.Property.Region.bottom > Stage_Line[COUNT_LINE - 1].PosY - 5)//↓
		{
			Flog.Property.SetRBPos({ Flog.Property.Region.right, Stage_Line[COUNT_LINE - 1].PosY - 5 });
			Flog.initMoveProperty();
		}
	}
	InvalidateRect(hWnd, NULL, FALSE);
}