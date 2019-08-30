#pragma once
#include "Game_DataManager.h"


#define WIDTH_WND 500
#define HEIGHT_WND 700

/*
���� Win����
- ���ΰ� ������Ʈ�� Stage[3]�� Line[1]�� �����ϸ� ���� Win
���� Lose����
- Line[0]�� ��������� 0�� ���� �� Lose
*/


//TimeProcedure(FPS(30) calls per 1 second)
void CALLBACK TimerProcedure(HWND hWnd, UINT uMessage, UINT idEvent, DWORD dwTime);
void CALLBACK IntroProcedure(HWND hWnd, UINT uMessage, UINT idEvent, DWORD dwTime);
//�޼��� ó���ϴ� ���������ν���
LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT iMessage, WPARAM wParameter, LPARAM lParameter);
LPCTSTR lpszClass = _T("Windows Class Name");//������Ŭ������ �̸�
HINSTANCE g_hinst;//OS�� ������ ��ü �ڵ�

DOUBLE count_Heart_Index;			//���������� ����� ��Ʈ �迭 �ε���
//WinMain
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPreinstance, PSTR szCmdLine, int nCmdShow)
{
	HWND hWindow = NULL;//������ �ڵ�
	MSG Message;//os�� ������ �޼��� �����
	WNDCLASSEX WindowClass;//������ Ŭ����
	g_hinst = hinstance;

	//������ �⺻���� ����
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

	RegisterClassEx(&WindowClass);//�������Ϳ� ������ ���

								  //������ �Ӽ��� �ο�(������ �����)
	hWindow = CreateWindow(lpszClass, _T("Window Programming ����� �ʾƿ�!"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		WIDTH_WND, HEIGHT_WND, NULL, NULL, hinstance, NULL);

	ShowWindow(hWindow, nCmdShow);//������ â ����
	UpdateWindow(hWindow);

	//�޼�������
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
	static HDC hdc, subdc, BackgroundDC;//DC�� �ڵ�
	static HBITMAP hBitMap_Subdc, oldBitMap_Subdc;//������۸��� ��Ʈ�� �ڵ�
	static PAINTSTRUCT lpPaint;//��µ� ������ ���� ������ ������ ����ü ������ ���� �ּ�
	static HPEN hPen, oldPen;
	static TCHAR TextBuf[256];

	switch (iMessage)
	{
	case WM_CREATE://c++Ŭ������ �����ڿ� ����� ����.
		srand((unsigned)time(NULL));
		GetClientRect(hWnd, &ClientRect);
		Unit.cx = ClientRect.bottom / COUNT_LINE;
		Unit.cy = ClientRect.bottom / COUNT_LINE;
		List_IMG.SetIMG(g_hinst);
		GameDataMANEGER.level = 1;		//���� 1�� ����
		for (int i = 0; i < COUNT_LINE; i++)
		{
			Stage_Line[i].PosY = Unit.cy * i;
			Stage_Line[i].Width = ClientRect.right;
		}

		//���� ����
		Hunter.SetImage(13);
		Hunter.SetSize({ (LONG)(Unit.cx * 1.5) - 10, (LONG)(Unit.cy * 1.5) - 10 });

		//���� 1(�� �������� 1)
		SetStageLevel(hWnd, GameDataMANEGER.level);
		//������ ����
		Flog.Property.SetSize({ Unit.cx - 10, Unit.cy-10 });
		Flog.FlogDeathAnimation.SetImage(List_IMG.hbmp_AnimateFlog, List_IMG.bmp_AnimateFlog, COLOR_BACKGROUND_TRAN, 1, 12, 3, SEC_RESPAN);
		Flog.FlogDeathAnimation.SetSize(Flog.Property.Size);
		Flog.Property.SetImage(List_IMG.hbmp_AnimateFlog, List_IMG.bmp_AnimateFlog, COLOR_BACKGROUND_TRAN, 1, 0, COUNT_AnimateCycle_Flog, SEC_MOVE);
		Flog.Property.SetLTPos({ ClientRect.right / 2 - Flog.Property.Size.cx / 2, Stage_Line[COUNT_LINE - 2].PosY + 5 });
		//Flog.Property.SetLTPos({ ClientRect.right / 2 - Flog.Property.Size.cx / 2, Stage_Line[2].PosY + 5 });

		GameDataMANEGER.GameTime.SetTimeBar();
		//BGM ���
		GameDataMANEGER.GameSound.PlaySOUND(0);

		SetTimer(hWnd, 1, 1000 / FPS, (TIMERPROC)TimerProcedure);
		SetTimer(hWnd, ENVET_HELP, 700, (TIMERPROC)IntroProcedure);//HELP �ִϸ��̼�
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

		if (GameDataMANEGER.GameStart)//���� ȭ��
		{
			BackgroundDC = CreateCompatibleDC(subdc);

			//��� ���
			SelectObject(BackgroundDC, List_IMG.hbmp_BackGround[0]);
			StretchBlt(subdc, ClientRect.left, ClientRect.top, ClientRect.right, Unit.cy + 1, BackgroundDC, 0, 0, List_IMG.bmp_BackGround[0].bmWidth, List_IMG.bmp_BackGround[0].bmHeight, SRCCOPY);

			//��ɲ����
			Hunter.Draw(subdc);

			//��� �ϴ�
			SelectObject(BackgroundDC, List_IMG.hbmp_BackGround[1]);
			StretchBlt(subdc, ClientRect.left, ClientRect.top + Unit.cy, ClientRect.right, (ClientRect.bottom - Unit.cy + 5) - (ClientRect.bottom - (Stage_Line[COUNT_LINE - 1].PosY + Unit.cy)), BackgroundDC, 0, 0, List_IMG.bmp_BackGround[1].bmWidth, List_IMG.bmp_BackGround[1].bmHeight, SRCCOPY);

			DeleteDC(BackgroundDC);

			//���⿡ �׸��⸦ ��.
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

			//ȭ�� ���
			Hunter.Draw_Arrow(subdc);

			//���� ���� ���(Ÿ�ӹ�, ���, ���� ��)
			GameDataMANEGER.Draw_Gameinfo(subdc, (LONG)count_Heart_Index);
		}
		else if (GameDataMANEGER.GameHelp)//���� ���ۼ��� ȭ��
		{
			//��� �׸���
			static HBRUSH hBrush, oldBrush;
			hBrush = CreateSolidBrush(RGB(254, 254, 255));
			oldBrush = (HBRUSH)SelectObject(subdc, hBrush);
			FillRect(subdc, &ClientRect, hBrush);
			
			SelectObject(subdc, oldBrush);

			BackgroundDC = CreateCompatibleDC(subdc);

			//������ ���� �̹���
			
			SelectObject(BackgroundDC, List_IMG.hbmp_AnimateFlog[0]);
			StretchBlt(subdc, ClientRect.left + Unit.cx, ClientRect.top + Unit.cy, List_IMG.bmp_AnimateFlog[0].bmWidth, List_IMG.bmp_AnimateFlog[0].bmHeight, BackgroundDC, 0, 0, List_IMG.bmp_AnimateFlog[0].bmWidth, List_IMG.bmp_AnimateFlog[0].bmHeight, SRCCOPY);

			//����Ű ���� �̹���
			SelectObject(BackgroundDC, List_IMG.hbmp_Helpinfo_IMG[0]);
			BitBlt(subdc, ClientRect.left + Unit.cx, ClientRect.top + (Unit.cy*2), List_IMG.bmp_Helpinfo_IMG[0].bmWidth, List_IMG.bmp_Helpinfo_IMG[0].bmHeight, BackgroundDC, 0, 0, SRCCOPY);
			//������ ���� �̹���
				
			SelectObject(BackgroundDC, List_IMG.hbmp_Gameitem[0]);
			BitBlt(subdc, ClientRect.left + (Unit.cx * 4), ClientRect.top + Unit.cy, List_IMG.bmp_Gameitem[0].bmWidth, List_IMG.bmp_Gameitem[0].bmHeight, BackgroundDC, 0, 0, SRCCOPY);
					
			SelectObject(BackgroundDC, List_IMG.hbmp_Gameitem[1]);
			BitBlt(subdc, ClientRect.left + (Unit.cx * 5), ClientRect.top + Unit.cy, List_IMG.bmp_Gameitem[1].bmWidth, List_IMG.bmp_Gameitem[1].bmHeight, BackgroundDC, 0, 0, SRCCOPY);

			SelectObject(BackgroundDC, List_IMG.hbmp_Gameitem[2]);
			BitBlt(subdc, ClientRect.left + (Unit.cx * 6), ClientRect.top + Unit.cy, List_IMG.bmp_Gameitem[2].bmWidth, List_IMG.bmp_Gameitem[2].bmHeight, BackgroundDC, 0, 0, SRCCOPY);
			
			//�Ź��� ���� �̹���

			SelectObject(BackgroundDC, List_IMG.hbmp_Gameitem[3]);									
			StretchBlt(subdc, ClientRect.left + (Unit.cx * 4), ClientRect.top + (Unit.cy * 3),Unit.cx,Unit.cy, BackgroundDC, 0, 0, List_IMG.bmp_Gameitem[3].bmWidth, List_IMG.bmp_Gameitem[3].bmHeight, SRCCOPY);

			//�ڵ��� ���� �̹���
			SelectObject(BackgroundDC, List_IMG.hbmp_Instance[0]);
			StretchBlt(subdc, ClientRect.left + Unit.cx , ClientRect.top + (Unit.cy * 6), List_IMG.bmp_Instance[3].bmWidth, List_IMG.bmp_Instance[3].bmHeight,BackgroundDC, 0, 0, List_IMG.bmp_Instance[3].bmWidth, List_IMG.bmp_Instance[3].bmHeight, SRCCOPY);

			//�볪�� ���� �̹���  (�ڵ����� ������Ʈ�� �°� ũ�� ����)
			SelectObject(BackgroundDC, List_IMG.hbmp_Instance[10]);
			StretchBlt(subdc, ClientRect.left + (Unit.cx*4), ClientRect.top + (Unit.cy * 6), List_IMG.bmp_Instance[3].bmWidth, List_IMG.bmp_Instance[3].bmHeight, BackgroundDC, 0, 0, List_IMG.bmp_Instance[10].bmWidth, List_IMG.bmp_Instance[10].bmHeight, SRCCOPY);

			//�ź��� ���� �̹���
			SelectObject(BackgroundDC, List_IMG.hbmp_AnimateTuttle[0]);
			StretchBlt(subdc, ClientRect.left + (Unit.cx * 6), ClientRect.top + (Unit.cy * 6), List_IMG.bmp_Instance[3].bmWidth, List_IMG.bmp_Instance[3].bmHeight, BackgroundDC, 0, 0, List_IMG.bmp_AnimateTuttle[0].bmWidth, List_IMG.bmp_AnimateTuttle[0].bmHeight, SRCCOPY);

			//��ɲ� ���� �̹���
			SelectObject(BackgroundDC, List_IMG.hbmp_Instance[12]);
			StretchBlt(subdc, ClientRect.left + Unit.cx, ClientRect.top + (Unit.cy * 9), List_IMG.bmp_Instance[12].bmWidth, List_IMG.bmp_Instance[12].bmHeight, BackgroundDC, 0,0, List_IMG.bmp_Instance[12].bmWidth, List_IMG.bmp_Instance[12].bmHeight, SRCCOPY);

			//ȭ�� ���� �̹���
			SelectObject(BackgroundDC, List_IMG.hbmp_Instance[13]);
			StretchBlt(subdc, ClientRect.left + Unit.cx, ClientRect.top + (Unit.cy * 11), List_IMG.bmp_Instance[13].bmWidth*2, List_IMG.bmp_Instance[13].bmHeight*2, BackgroundDC, 0, 0, List_IMG.bmp_Instance[13].bmWidth, List_IMG.bmp_Instance[13].bmHeight, SRCCOPY);

			//Ÿ�ӹ� ���� �̹���
			SelectObject(BackgroundDC, List_IMG.hbmp_Gameinfo[1]);
			StretchBlt(subdc, ClientRect.left + Unit.cx, ClientRect.top + (Unit.cy * 13), List_IMG.bmp_Gameinfo[1].bmWidth, List_IMG.bmp_Gameinfo[1].bmHeight, BackgroundDC, 0, 0, List_IMG.bmp_Gameinfo[1].bmWidth, List_IMG.bmp_Gameinfo[1].bmHeight, SRCCOPY);

			SelectObject(BackgroundDC, List_IMG.hbmp_Gameinfo[2]);
			TransparentBlt(subdc, ClientRect.left + Unit.cx+(Unit.cx/2)+3, ClientRect.top + (Unit.cy * 13)+(Unit.cy/2)-3, List_IMG.bmp_Gameinfo[2].bmWidth, List_IMG.bmp_Gameinfo[2].bmHeight, BackgroundDC, 0, 0, List_IMG.bmp_Gameinfo[2].bmWidth, List_IMG.bmp_Gameinfo[2].bmHeight, RGB(255,255,255));


			HFONT hFont, oldFont;
			SIZE FontSize;

			hFont = CreateFont(15, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("�޸յձ�������"));
			oldFont = (HFONT)SelectObject(subdc, hFont);
			//������ ���� �ؽ�Ʈ
			wsprintf(TextBuf, TEXT("�������� ������ ���ʽ� ���� ���"));									
			GetTextExtentPoint(hdc, TextBuf, (int)_tcslen(TextBuf), &FontSize);
			
			SetBkMode(subdc, TRANSPARENT);
			SetTextColor(subdc, RGB(221, 0, 0));
			TextOut(subdc, ClientRect.left + (Unit.cx * 4), ClientRect.top + (Unit.cy*2), TextBuf, (int)_tcslen(TextBuf));
			
			//�Ź��� ���� �ؽ�Ʈ
			wsprintf(TextBuf, TEXT("�Ź��ٿ� �ɸ��� 2�ʵ��� �ӹ�"));										
			GetTextExtentPoint(hdc, TextBuf, (int)_tcslen(TextBuf), &FontSize);

			SetBkMode(subdc, TRANSPARENT);
			SetTextColor(subdc, RGB(221, 160, 30));

			TextOut(subdc, ClientRect.left + (Unit.cx * 4), ClientRect.top + (Unit.cy * 5), TextBuf, (int)_tcslen(TextBuf));

			//�ڵ��� ���� �ؽ�Ʈ
			wsprintf(TextBuf, TEXT("�ڵ����� �΋H���� ���������� ������"));
			GetTextExtentPoint(hdc, TextBuf, (int)_tcslen(TextBuf), &FontSize);

			SetBkMode(subdc, TRANSPARENT);
			SetTextColor(subdc, RGB(2,225, 30));

			TextOut(subdc, ClientRect.left + Unit.cx, ClientRect.top + (Unit.cy * 7), TextBuf, (int)_tcslen(TextBuf));


			//�볪�� ���� �ؽ�Ʈ
			wsprintf(TextBuf, TEXT("�볪���� �ź��̿� �ö� ���� �ǳμ� ����"));
			GetTextExtentPoint(hdc, TextBuf, (int)_tcslen(TextBuf), &FontSize);

			SetBkMode(subdc, TRANSPARENT);
			SetTextColor(subdc, RGB(21, 0, 230));

			TextOut(subdc, ClientRect.left + Unit.cx, ClientRect.top + (Unit.cy * 8), TextBuf, (int)_tcslen(TextBuf));


			//��ɲ� ���� �ؽ�Ʈ
			wsprintf(TextBuf, TEXT("��ɲ��� ���� 2���� ����"));
			GetTextExtentPoint(hdc, TextBuf, (int)_tcslen(TextBuf), &FontSize);

			SetBkMode(subdc, TRANSPARENT);
			SetTextColor(subdc, RGB(221, 10, 230));

			TextOut(subdc, ClientRect.left + (Unit.cx*3), ClientRect.top + (Unit.cy * 9), TextBuf, (int)_tcslen(TextBuf));
			
			//ȭ�� ���� �ؽ�Ʈ

			wsprintf(TextBuf, TEXT("ȭ�쿡 ������ ���� 1���� �� ���������� �̵�"));
			GetTextExtentPoint(hdc, TextBuf, (int)_tcslen(TextBuf), &FontSize);

			SetBkMode(subdc, TRANSPARENT);
			SetTextColor(subdc, RGB(100, 100, 230));

			TextOut(subdc, ClientRect.left + Unit.cx, ClientRect.top + (Unit.cy * 12), TextBuf, (int)_tcslen(TextBuf));

			//Ÿ�ӹ� ���� �ؽ�Ʈ
			wsprintf(TextBuf, TEXT("�� ������ ������ �ð��ο�   ���ѽð��� �� ������ ���� ����"));
			GetTextExtentPoint(hdc, TextBuf, (int)_tcslen(TextBuf), &FontSize);

			SetBkMode(subdc, TRANSPARENT);
			SetTextColor(subdc, RGB(0, 1, 5));

			TextOut(subdc, ClientRect.left + Unit.cx, ClientRect.top + (Unit.cy * 14)+10, TextBuf, (int)_tcslen(TextBuf));

			SelectObject(subdc, oldFont);
			DeleteObject(hFont);

			
			//SelectObject(BackgroundDC, List_IMG.hbmp_);

			DeleteDC(BackgroundDC);

			//�������� ���� ���
			GameDataMANEGER.Draw_Helpinfo(subdc);
		}
		else//��Ʈ��ȭ��
		{
			//��� �׸���
			BackgroundDC = CreateCompatibleDC(subdc);
			SelectObject(BackgroundDC, List_IMG.hbmp_Gameinfo[0]);
			StretchBlt(subdc, ClientRect.left, ClientRect.top, ClientRect.right, ClientRect.bottom, BackgroundDC, 0, 0, List_IMG.bmp_Gameinfo[0].bmWidth, List_IMG.bmp_Gameinfo[0].bmHeight, SRCCOPY);
			DeleteDC(BackgroundDC);

			//��Ʈ�� ���� ���(Start ��ư, Help ��ư)
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
	return DefWindowProc(hWnd, iMessage, wParameter, lParameter);//case���� ó������ ���� �޼����� ó���ϴ� �Լ�
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
		//���� ��Ʈ�� �ε��� ����
		count_Heart_Index += 0.1;
		if (count_Heart_Index>5.0) {
			count_Heart_Index = 0;
		}

		//���� ������ ���ݺο�
		if (GameDataMANEGER.level > 1 && Hunter.Appear == FALSE)
		{
			SetTimer(hWnd, EVENT_APPEAR, 1000 * SEC_APPEAR_INTERVAL, (TIMERPROC)Appear);
			Hunter.Appear = TRUE;
		}

		POINT HunterCenterPoint = Hunter.GetCenterPoint();
		LONG DistY = (Stage_Line[1].PosY - Unit.cy / 2) - HunterCenterPoint.y;
		LONG DistX = Flog.Property.CenterPoint.x - HunterCenterPoint.x;
		//���� ������ ��ġ ����
		if (Hunter.Appear && !Hunter.Shoot && !Hunter.Hide)
		{
			Hunter.Move((LONG)(DistX*0.01), (LONG)(DistY * 0.1));
			Hunter.SetDirection_Arrow(Flog.Property.CenterPoint);
		}
		//���� ����
		else if (Hunter.Shoot)
		{
			DistY = Stage_Line[2].PosY - HunterCenterPoint.y;
			Hunter.Move(0, (LONG)(DistY * 0.1));
		}

		//�����÷��� �ߵ�
		if (abs(DistY) <= 10 && !Hunter.Aiming && !Hunter.Shoot && !Hunter.Arrow_Move)
		{
			Hunter.Aiming = TRUE;
			SetTimer(hWnd, EVENT_AIMING, 1000 * SEC_AIMING, (TIMERPROC)Aiming);
		}
		//�������� �����÷��� �ߵ�
		else if (abs(DistY) <= 10 && Hunter.Appear && Hunter.Shoot)
		{
			Hunter.Appear = FALSE;
			Hunter.Shoot = FALSE;
			Hunter.Hide = TRUE;
		}

		//ȭ�� �̵�
		if (Hunter.Arrow_Move && !Hunter.Arrow_Collision)
		{
			LONG dx = (LONG)(SPEED_ARROW * cos(TORAD(Hunter.GetAngle())));
			LONG dy = (LONG)(SPEED_ARROW * sin(TORAD(Hunter.GetAngle())));
			Hunter.Move_Arrow(dx, dy);
		}

		//ȭ����� �浹üũ
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
				PlaySound(TEXT("Resource\\Sound\\������4.WAV"), NULL, SND_ASYNC);//�׾��� �� �Ҹ�
				if(GameDataMANEGER.Life) SetTimer(hWnd, EVENTE_RESPAN, 1000 * SEC_RESPAN, (TIMERPROC)Respan_Flog);
			}
			
			SetTimer(hWnd, EVENT_FIXED_ARROW, 1000 * SEC_FIXED_ARROW, (TIMERPROC)Disappear_Arrow);
		}
		else if (Hunter.GetArrowRect().right < ClientRect.left || Hunter.GetArrowRect().left > ClientRect.right)
			Hunter.Arrow_Move = FALSE;

		if (!Flog.Death)
		{
			//������ �̵����� ����
			if (!Flog.Bondage && Flog.Direction == 0)
			{
				if (KeyBuffer['W'])//��
				{
					Flog.BeforeMovePoint = Flog.Property.CenterPoint;
					Flog.Direction = 3;
					Flog.Property.SetFrameStart(COUNT_AnimateCycle_Flog * 0);
					PlaySound(TEXT("Resource\\Sound\\FROG.WAV"), NULL, SND_ASYNC);
				}
				else if (KeyBuffer['A'])//��
				{
					Flog.BeforeMovePoint = Flog.Property.CenterPoint;
					Flog.Direction = 1;
					Flog.Property.SetFrameStart(COUNT_AnimateCycle_Flog * 1);
					PlaySound(TEXT("Resource\\Sound\\FROG.WAV"), NULL, SND_ASYNC);
				}
				else if (KeyBuffer['S'])//��
				{
					Flog.BeforeMovePoint = Flog.Property.CenterPoint;
					Flog.Direction = 4;
					Flog.Property.SetFrameStart(COUNT_AnimateCycle_Flog * 2);
					PlaySound(TEXT("Resource\\Sound\\FROG.WAV"), NULL, SND_ASYNC);
				}
				else if (KeyBuffer['D'])//��
				{
					Flog.BeforeMovePoint = Flog.Property.CenterPoint;
					Flog.Direction = 2;
					Flog.Property.SetFrameStart(COUNT_AnimateCycle_Flog * 3);
					PlaySound(TEXT("Resource\\Sound\\FROG.WAV"), NULL, SND_ASYNC);
				}
			}

			//�����۰� �������� �浹üũ
			if (List_Item.CollisionItem == nullptr)
				List_Item.CollisionItem = List_Item.CollsionWithPop(Flog.Property.Region);
			if(List_Item.CollisionItem != nullptr)
			{
				if (List_Item.CollisionItem->Type == 4)//�Ź���
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
					Flog.Move();//������ �̵�
				else
				{
					Flog.Bondage = TRUE;
					Flog.initMoveProperty();
				}
			}
		}

		for (size_t i = 0; i < COUNT_LINE; i++)
		{
			//�� ���κ� ������Ʈ �̵�
			Stage_Line[i].Move_Objects();

			//ȭ���� Fix
			if (Hunter.Arrow_Collision && 3 <= i && i <= 7 && Stage_Line[i].Collision_Objects(Hunter.GetArrowRect()))
				Hunter.Move_Arrow(Stage_Line[i].SpeedX, 0);
			else if (Hunter.Arrow_Collision && Stage_Line[3].PosY >= Hunter.GetArrowRect().bottom && Stage_Line[7].PosY <= Hunter.GetArrowRect().bottom && !Stage_Line[i].Collision_Objects(Hunter.GetArrowRect()))
				Hunter.Arrow_Move = FALSE;

			//������Ʈ ���� ������ �ִ� ������ �̵�
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

			if (9 <= i && i <= 13 && Flog.Death == FALSE && Stage_Line[i].Collision_Objects(Flog.Property.Region))//Line[9] ~ Line[13]���� ������ ������Ʈ�� ���������� ��ȣ�ۿ�(�浹üũ �� ������ ����)
			{
				Flog.Death = TRUE;
				Flog.CollsionType = 2;
				GameDataMANEGER.Life--;
				GameDataMANEGER.StackScore /= 2;
				Flog.FlogDeathAnimation.SetCenterPos(Flog.Property.CenterPoint);
				PlaySound(TEXT("Resource\\Sound\\������4.WAV"), NULL, SND_ASYNC);//���� �浹������ �Ҹ�
				if (GameDataMANEGER.Life) SetTimer(hWnd, EVENTE_RESPAN, 1000 * SEC_RESPAN, (TIMERPROC)Respan_Flog);
			}
			//Line[3] ~ Line[7]���� ������ ������Ʈ�� ���������� ��ȣ�ۿ� (�浹üũ �� ������ ����)
			else if (3 <= i && i <= 7 && Flog.Death == FALSE && Stage_Line[i].PosY <= Flog.Property.Region.top && Flog.Property.Region.bottom <= Stage_Line[i + 1].PosY && !(Stage_Line[i].Collision_Objects(Flog.Property.Region)))
			{
				Flog.Death = TRUE;
				Flog.CollsionType = 3;
				GameDataMANEGER.Life--;
				GameDataMANEGER.StackScore /= 2;
				Flog.FlogDeathAnimation.SetCenterPos(Flog.Property.CenterPoint);
				PlaySound(TEXT("Resource\\Sound\\water_splash2.wav"), NULL, SND_ASYNC);//���� ǳ���ϴ� �Ҹ�
				if (GameDataMANEGER.Life) SetTimer(hWnd, EVENTE_RESPAN, 1000 * SEC_RESPAN, (TIMERPROC)Respan_Flog);
			}
			//Line[3] ~ Line[7]���� ������ ������Ʈ�� ���������� ��ȣ�ۿ� (�浹üũ �� ���� �̵� �� �̵�����)
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
					//���� Ŭ����
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
						//�������� ��ȯ
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


		//������ ����� 0���� ���� �� Ȥ�� Ÿ�ӿ����� ���� ��
		if (!GameDataMANEGER.StageFail && (!GameDataMANEGER.Life || GameDataMANEGER.GameTime.Check_TimeOut()))
		{
			//���� ����
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

		//������Ʈ ���� ������ �ִ� ������ �̵����� ����
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

		//�������� ȭ�� �� �κа��� �浹üũ
		if (Flog.Property.Region.left < ClientRect.left + 5)//��
		{
			Flog.Property.SetLTPos({ ClientRect.left + 5, Flog.Property.Region.top });
			Flog.initMoveProperty();
		}
		if (Flog.Property.Region.right > ClientRect.right - 5)//��
		{
			Flog.Property.SetRBPos({ ClientRect.right - 5, Flog.Property.Region.bottom });
			Flog.initMoveProperty();
		}
		if (Flog.Property.Region.top < Stage_Line[1].PosY + 5)//��
		{
			Flog.Property.SetLTPos({ Flog.Property.Region.left, Stage_Line[1].PosY + 5 });
			Flog.initMoveProperty();
		}
		if (Flog.Property.Region.bottom > Stage_Line[COUNT_LINE - 1].PosY - 5)//��
		{
			Flog.Property.SetRBPos({ Flog.Property.Region.right, Stage_Line[COUNT_LINE - 1].PosY - 5 });
			Flog.initMoveProperty();
		}
	}
	InvalidateRect(hWnd, NULL, FALSE);
}