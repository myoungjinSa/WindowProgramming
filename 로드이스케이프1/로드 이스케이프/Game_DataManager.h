#pragma once
#include "GameTime.h"
#include "GameSound.h"

#define MAX_LIFE 3
#define COLOR_BUTTON RGB(0, 0, 0)
#define TIME_SCORE_INTERVAL 2

#define SEC_STAGECLAER 4
#define SEC_GAMEOVER 4

//�������� ����
/*
Client������ �� ���κ��� �з�
�� ���κ��� ������Ʈ���� ����

Line[0]  .....Game information1.....
Line[1]  ...........Forest...........
Line[2]  ..........Gorl in..........
Line[3]  ..Wood....Wood....Wood.....
Line[4]  tle...Tuttle...Tuttle...Tut
Line[5]  ..Wood....Wood....Wood.....
Line[6]  ......Wood....Wood....Wood.
Line[7]  Tuttle....Tuttle....Tuttle.
Line[8]  ........SideWalkWay........
Line[9]  ..Car......Car......Car....
Line[10] ........Car.........Car....
Line[11] Car...Car...Car...Car...Car
Line[12] ..Car..............Car.....
Line[13] ......Car......Car......Car
Line[14] ........SideWalkWay........
Line[15] .....Game information2.....

Line[0]: (���� ��ܿ� ��� ����, ���� ��ܿ� ����)
Line[15]: ���� ���� �ð��� ����Ͽ� TimeBar�� �پ���.

Line[2]: ���� ����
.        (���� 1: ���ΰ� ������Ʈ�� ���� ������ �����ϸ� ���� �����ð�(��) * 100�� ���� ���� �� ���� �����찡 ���� ���������� ��ȯ)
.        (���� 2: ��������[2]���� ���������� Line[1]��ü�� �ƴ϶� Line[1]���� ��Ǯ������Ʈ(4��)�� ���������� �ȴ�.)
.        (���� 3: ��������[3]���� ��ɲ��� ��Ǯ������Ʈ �� ������ ��ġ���� ��Ÿ�� �������� ���� ȭ���� ��� �������.)
.        (���� 4: ��������[3]���� ��ɲ��� ȭ���� �� ������ ȭ���� ������ ��ġ�� �̵��Ѵ�.
.                 ��ɲ��� ������ġ�κ��� ��������ġ������ �Ÿ�(sqrt(pow(dx,2)+pow(dy,2))�� ���ϰ�
.                 ��ɲ��� ������ġ�κ��� ��������ġ������ x�� ���� ����, y�� ���� ���͸� ���ؼ�(dx / �Ÿ�, dy / �Ÿ�)
.                 ȭ���� x������ x����⺤�� * ȭ��ӵ���ŭ, y������ y����⺤�� * ȭ��ӵ���ŭ �̵��Ѵ�.
.                 ex) Arrow.Move((dx / �Ÿ�) * SPEED, (dy / �Ÿ�) * SPEED);
.        (���� 5: ��������[3]���� �߻�� ȭ���� Line[7]�� �Ѿ�� �ʴ´�.

Line[3] ~ Line[7]: �볪���� �ź��̰� �յ���� �����ϸ鼭 ��->��(Ȥ�� ��<-��)�� ��ũ�Ѹ�
.                  (���� 1: �볪���� �ź��̰� �귯���� ������ ���� �ݴ�)
.                  (���� 2: �ź��̴� �Ѹ����� �ƴ϶� �ּ� 2���� �̻� ¦��� ������)
.                  (���� 3: �� ���κ� �̵� �ӵ��� �ٸ���)
.                  (���� 4: ���ΰ� ������Ʈ�� �̵��� �� �볪���� �ź��̵� �ٱ� �𼭸��� �������� �Ǹ� �������� �̵��ϰ� �Ѵ�.(�簢�� �ۿ� �������� �簢�� �������� �̵�))
.                  (���� 5: ���ΰ� ������Ʈ�� �볪���� �ź��� ������Ʈ ���� �ȿ� ���� �� �� ���κ� �̵� �ӵ��� ���߾� ���ΰ��� ���� �̵��ϰ� �ȴ�.)
.                  (���� 6: ���ΰ� ������Ʈ�� ���� �������� �̵��� �� �ش� ���ο����� ���ΰ�������Ʈ ��ġ��
�볪���� �ź��� ������Ʈ�� ������ ���ΰ��� ���� ������ �ȴ�.)
.                  (���� 7: ���ΰ� ������Ʈ�� �볪���� �ź��� ������Ʈ ���� �ȿ� �ְ�,
�볪���� �ź��� ������Ʈ�� ClientRect.right�� ClientRect.left�� ����� ���ΰ��� ���� ������ �ȴ�.)

Line[9] ~ Line[13]: �ڵ����� �յ���� �����ϸ鼭 ��ũ�Ѹ�
.                   (���� 1: Ȧ����° ������ ��<-�·� ��ũ�Ѹ�, ¦����° ������ ��->�·� ��ũ�Ѹ�)
.                   (���� 2: ���ΰ� ������Ʈ�� �ڵ��� ������Ʈ�� �浹�� �Ͼ�� ��������� -1 �����ϰ�, ���ΰ��� ��ġ�� ó�� ������ġ(Line[14])�� �̵���Ų��.

Line[7], Line[14]: ���ΰ� ������Ʈ ������ ����
.                   (���� 1: ���ΰ� ������Ʈ�� Line[2] ~ Line[6]���� �׾��� ��쿡�� Line[7]���� ������,
.                            ���ΰ� ������Ʈ�� Line[8] ~ Line[13]���� �׾��� ��쿡�� Line[14]���� �������� �ȴ�.)

�� ��� ���ο��� ���ΰ� ������Ʈ�� ��, ��, ��, �� ������ �� �ִ�.
���ΰ� ������Ʈ�� Line[1](���� ����)�� �������� �� Line[0]�� ������ ��� ������Ʈ�� �ʱ�ȭ �ǰ� ���Ҵ�ȴ�.(�������� ���� �� ���� ���� ���������� ��ȯ)
*/
void SetStageLevel(HWND hWnd, size_t Level);

class GAMEINFO
{
private:
	TCHAR infoText[256];
	LONG ScoreCount;//������ �������� ����<-���� �����쿡 ��µǴ� ������
	RECT StartButton, HelpButton, PreviousButton;//��ư ����
	BOOL ButtonUp_S, ButtonUp_H;//��ư ���� Ȱ��ȭ �÷���
	LONG RGB_s[3];
	LONG index_RGB;

	//infoTextPosX
	DOUBLE infoTextPosX;
	//�˸��ؽ�Ʈ �������� �ð�
	LONG info_sec_Count;
	LONG DestX;
public:
	//������ ����(������ ȹ��: 100��, ����: ��������/2)
	LONG StackScore;

	//���� ���� �÷���
	BOOL GameStart;
	BOOL GameHelp;
	BOOL GameEnd;
	
	//�������� ��ȯ �÷���
	BOOL StageClear;
	BOOL StageFail;

	GAMETIME GameTime;
	GAMESOUND GameSound;

	LONG Life;//���

	LONG Image_index;
	LONG level;
	GAMEINFO()
	{
		memset(infoText, 0, sizeof(TCHAR) * 255);
		infoTextPosX = 0.0;
		info_sec_Count = 0;
		ScoreCount = 0;
		StackScore = 0;
		Life = MAX_LIFE;
		GameStart = FALSE;
		GameHelp = FALSE;
		GameEnd = FALSE;

		StageClear = FALSE;
		StageFail = FALSE;

		ButtonUp_S = FALSE;
		ButtonUp_H = FALSE;
		HelpButton = { 0, 0, 0, 0 };
		StartButton = { 0, 0, 0, 0 };
		PreviousButton = { 0, 0, 0, 0 };

		index_RGB = 1;
		RGB_s[0] = 255;
		RGB_s[1] = 0;
		RGB_s[2] = 0;

		Image_index = 0;
		level = 0;
	}

	//�˸��ؽ�Ʈ �ʱ�ȭ
	void init_infoText()
	{
		memset(infoText, 0, sizeof(TCHAR) * 255);
		DestX = 0;
		infoTextPosX = 0.0;
		info_sec_Count = 0;
	}

	//��Ʈ��ȭ�� ��ȯ �� ���� ������ ���� 1�� �ʱ�ȭ
	void init_All_GameData()
	{
		Flog.Property.SetFrameStart(0);
		Flog.Property.SetRBPos({ ClientRect.right / 2 + Flog.Property.Size.cx / 2, Stage_Line[COUNT_LINE - 1].PosY - 5 });
		Flog.initMoveProperty();
		Flog.BeforeMovePoint = { ClientRect.right / 2 + Flog.Property.Size.cx / 2, Stage_Line[COUNT_LINE - 1].PosY - 5 };
		Flog.Death = FALSE;
		memset(infoText, 0, sizeof(TCHAR) * 255);
		ScoreCount = 0;
		StackScore = 0;
		Image_index = 0;
		level = 1;
		Life = MAX_LIFE;
		GameStart = FALSE;
		GameHelp = FALSE;
		GameEnd = FALSE;

		StageClear = FALSE;
		StageFail = FALSE;

		ButtonUp_S = FALSE;
		ButtonUp_H = FALSE;
		HelpButton = { 0, 0, 0, 0 };
		StartButton = { 0, 0, 0, 0 };
		PreviousButton = { 0, 0, 0, 0 };

		GameSound.Reset_AllSound();
		//GameSound.PlaySOUND(SOUND_BACKGROUND_MUSIC1);
	}
	//��ư Ŭ�� Ȯ��
	void CheckByClick()
	{
		if (!GameStart)
		{
			if (!GameHelp && PtInRect(&StartButton, MousePoint))
				GameStart = TRUE;
			else if (!GameHelp && PtInRect(&HelpButton, MousePoint))
				GameHelp = TRUE;
			else if (GameHelp && PtInRect(&PreviousButton, MousePoint))
				GameHelp = FALSE;
		}
	}

	//���콺�����Ϳ� ��ư���� �浹üũ <- ��ư ���� Ȱ��ȭ
	void Check_CollisionButton()
	{
		if (!GameStart && !GameHelp)
		{
			if (PtInRect(&StartButton, MousePoint))
				ButtonUp_S = TRUE;
			else
				ButtonUp_S = FALSE;
			if (PtInRect(&HelpButton, MousePoint))
				ButtonUp_H = TRUE;
			else
				ButtonUp_H = FALSE;
		}
	}

	//��Ʈ�� ȭ�� ���� ���
	void Draw_introinfo(HDC hdc)
	{
		HFONT hFont, oldFont;
		SIZE Size_Button;

		hFont = CreateFont(Unit.cy, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("�޸յձ�������"));
		oldFont = (HFONT)SelectObject(hdc, hFont);

		//GAME START ��ư ���
		wsprintf(infoText, TEXT("GAME START"));
		GetTextExtentPoint(hdc, infoText, (int)_tcslen(infoText), &Size_Button);
		if (StartButton.left == 0 && StartButton.top == 0 && StartButton.right == 0 && StartButton.bottom == 0)
		{
			StartButton.left = ClientRect.right / 2 - Size_Button.cx / 2;
			StartButton.right = StartButton.left + Size_Button.cx;
			StartButton.top = Stage_Line[COUNT_LINE - 3].PosY - Size_Button.cy / 2;
			StartButton.bottom = StartButton.top + Size_Button.cy;
		}
		//��ư ���� ����
		if (ButtonUp_S)
		{
			LONG indexNext = (index_RGB - 1 < 0) ? 2 : index_RGB - 1;
			if (RGB_s[index_RGB] < RGB_s[indexNext]) RGB_s[index_RGB] += 15;
			else if (RGB_s[index_RGB] > RGB_s[indexNext]) RGB_s[index_RGB] -= 15;
			else if (RGB_s[index_RGB] == RGB_s[indexNext])
				index_RGB = (index_RGB - 1 < 0) ? 2 : index_RGB - 1;
			SetTextColor(hdc, RGB(RGB_s[0], RGB_s[1], RGB_s[2]));
		}
		else
			SetTextColor(hdc, COLOR_BUTTON);
		SetBkMode(hdc, TRANSPARENT);
		TextOut(hdc, StartButton.left, StartButton.top, infoText, (int)_tcslen(infoText));

		//GAME HELP ��ư ���
		wsprintf(infoText, TEXT("GAME HELP"));
		GetTextExtentPoint(hdc, infoText, (int)_tcslen(infoText), &Size_Button);
		if (HelpButton.left == 0 && HelpButton.top == 0 && HelpButton.right == 0 && HelpButton.bottom == 0)
		{
			HelpButton.left = ClientRect.right / 2 - Size_Button.cx / 2;
			HelpButton.right = HelpButton.left + Size_Button.cx;
			HelpButton.top = Stage_Line[COUNT_LINE - 2].PosY - Size_Button.cy / 2;
			HelpButton.bottom = HelpButton.top + Size_Button.cy;
		}
		//��ư ���� ����
		if (ButtonUp_H)
		{
			LONG indexNext = (index_RGB - 1 < 0) ? 2 : index_RGB - 1;
			if (RGB_s[index_RGB] < RGB_s[indexNext]) RGB_s[index_RGB] += 15;
			else if (RGB_s[index_RGB] > RGB_s[indexNext]) RGB_s[index_RGB] -= 15;
			else if (RGB_s[index_RGB] == RGB_s[indexNext])
				index_RGB = (index_RGB - 1 < 0) ? 2 : index_RGB - 1;
			SetTextColor(hdc, RGB(RGB_s[0], RGB_s[1], RGB_s[2]));
		}
		else
			SetTextColor(hdc, COLOR_BUTTON);
		SetBkMode(hdc, TRANSPARENT);
		TextOut(hdc, HelpButton.left, HelpButton.top, infoText, (int)_tcslen(infoText));

		SelectObject(hdc, oldFont);
		DeleteObject(hFont);
	}

	//���� ȭ�� ���� ���
	void Draw_Helpinfo(HDC hdc)
	{
		//Help �̹��� ���
		//////////////////
		//              //
		//////////////////
		


		//�ڷΰ���(Ȥ�� ����ȭ��) ��ư ���
		HFONT hFont, oldFont;
		SIZE FontSize;

		hFont = CreateFont(Unit.cy - 10, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("�޸յձ�������"));
		oldFont = (HFONT)SelectObject(hdc, hFont);
		wsprintf(infoText, TEXT("�ڷΰ���"));
		GetTextExtentPoint(hdc, infoText, (int)_tcslen(infoText), &FontSize);
		if (PreviousButton.left == 0 && PreviousButton.top == 0 && PreviousButton.right == 0 && PreviousButton.bottom == 0)
		{
			PreviousButton.left = ClientRect.right - (FontSize.cx + Unit.cx);
			PreviousButton.right = PreviousButton.left + FontSize.cx;
			PreviousButton.top = Stage_Line[COUNT_LINE - 1].PosY + 5;
			PreviousButton.bottom = PreviousButton.top + FontSize.cy;
		}
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(221, 160, 30));
		TextOut(hdc, PreviousButton.left, PreviousButton.top, infoText, (int)_tcslen(infoText));

		SelectObject(hdc, oldFont);
		DeleteObject(hFont);
	}
	//�������� ���
	void Draw_Gameinfo(HDC hdc,LONG count_heart_Index)
	{
		/////////////////////
		//��� �̹��� ���)//
		static HDC BackGrounddc;
		static HBITMAP hBit, oldBit;

		BackGrounddc = CreateCompatibleDC(hdc);

		SelectObject(BackGrounddc, List_IMG.hbmp_HeartAnimaition[count_heart_Index]);									//������ ���� �̹���
		for (int i = 0; i <Life; i++) {
			TransparentBlt(hdc, ClientRect.left + List_IMG.bmp_HeartAnimaition[count_heart_Index].bmWidth*i, ClientRect.top + 10, List_IMG.bmp_HeartAnimaition[count_heart_Index].bmWidth, List_IMG.bmp_HeartAnimaition[count_heart_Index].bmHeight, BackGrounddc, 0, 0, List_IMG.bmp_HeartAnimaition[count_heart_Index].bmWidth, List_IMG.bmp_HeartAnimaition[count_heart_Index].bmHeight, RGB(255, 255, 255));
		}
		DeleteDC(BackGrounddc);
		//		/////////////////////
		/////////////////////

		HFONT hFont, oldFont;
		SIZE FontSize;

		hFont = CreateFont(Unit.cy - 20, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("�޸յձ�������"));
		oldFont = (HFONT)SelectObject(hdc, hFont);

		//���� ���
		ScoreCount += (LONG)((DOUBLE)(StackScore - ScoreCount) / FPS * TIME_SCORE_INTERVAL);
		if (ScoreCount > StackScore)
			ScoreCount = StackScore;
		wsprintf(infoText, TEXT("Score: %d"), ScoreCount);
		GetTextExtentPoint(hdc, infoText, (int)_tcslen(infoText), &FontSize);
		SetTextColor(hdc, RGB(0, 0, 0));
		SetBkMode(hdc, TRANSPARENT);
		TextOut(hdc, ClientRect.right - FontSize.cx, Stage_Line[0].PosY + 5, infoText, (int)_tcslen(infoText));

		SelectObject(hdc, oldFont);
		DeleteObject(hFont);

		//Ÿ�ӹ� ���
		GameTime.Draw_TimeBar(hdc);

		hFont = CreateFont(Unit.cy - 20, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("�޸յձ�������"));
		oldFont = (HFONT)SelectObject(hdc, hFont);

		//���� �ð� ���
		LONG RemainingTime = GameTime.Get_RemainingTime();
		DOUBLE WarningColor = (RemainingTime <= 10) ? (214 / 11) * (11 - RemainingTime) : 0;
		wsprintf(infoText, TEXT("%d"), RemainingTime);
		GetTextExtentPoint(hdc, infoText, (int)_tcslen(infoText), &FontSize);
		SetTextColor(hdc, RGB(139, 214 - (LONG)WarningColor, 0));
		SetBkMode(hdc, TRANSPARENT);
		TextOut(hdc, GameTime.TimeBar[2].CenterPoint.x - FontSize.cx / 2, GameTime.TimeBar[1].CenterPoint.y - FontSize.cy / 2, infoText, (int)_tcslen(infoText));

		SelectObject(hdc, oldFont);
		DeleteObject(hFont);

		//�������� Ŭ�����
		if (StageClear)
		{
			info_sec_Count++;

			hFont = CreateFont(Unit.cy, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("�޸յձ�������"));
			oldFont = (HFONT)SelectObject(hdc, hFont);

			if(info_sec_Count < ((FPS*SEC_STAGECLAER) / 4 - 7) * 2)
				wsprintf(infoText, TEXT("Stage %d Clear!"), level - 1);
			else
				wsprintf(infoText, TEXT("Stage %d Start!"), level);
			GetTextExtentPoint(hdc, infoText, (int)_tcslen(infoText), &FontSize);

			if (info_sec_Count == 1 || info_sec_Count == ((FPS*SEC_STAGECLAER) / 4 - 7) * 2)
				DestX = ClientRect.right / 2 - FontSize.cx / 2;
			if (info_sec_Count == (FPS*SEC_STAGECLAER - 7) / 4 || info_sec_Count == ((FPS*SEC_STAGECLAER) / 4 - 5) * 3)
				DestX = ClientRect.right + 10;

			if (info_sec_Count == 1 || info_sec_Count == ((FPS*SEC_STAGECLAER) / 4 - 7) * 2)
				infoTextPosX = -FontSize.cx;
			//�۲� ��ġ(����)
			infoTextPosX += (DestX - infoTextPosX) * 0.2;

			if (info_sec_Count < ((FPS*SEC_STAGECLAER) / 4 - 7) * 2)
			{
				//�۲� �׶��̼�
				LONG indexNext = (index_RGB - 1 < 0) ? 2 : index_RGB - 1;
				if (RGB_s[index_RGB] < RGB_s[indexNext]) RGB_s[index_RGB] += 15;
				else if (RGB_s[index_RGB] > RGB_s[indexNext]) RGB_s[index_RGB] -= 15;
				else if (RGB_s[index_RGB] == RGB_s[indexNext])
					index_RGB = (index_RGB - 1 < 0) ? 2 : index_RGB - 1;
				SetTextColor(hdc, RGB(RGB_s[0], RGB_s[1], RGB_s[2]));
			}
			else
				SetTextColor(hdc, RGB(139, 214, 0));
			SetBkMode(hdc, TRANSPARENT);

			TextOut(hdc, (LONG)infoTextPosX, Stage_Line[8].PosY + 5, infoText, (int)_tcslen(infoText));

			SelectObject(hdc, oldFont);
			DeleteObject(hFont);
		}
		//���ӿ��� ��
		else if (StageFail)
		{
			info_sec_Count++;

			hFont = CreateFont(Unit.cy, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("�޸յձ�������"));
			oldFont = (HFONT)SelectObject(hdc, hFont);
			wsprintf(infoText, TEXT("GAMEOVER"));
			GetTextExtentPoint(hdc, infoText, (int)_tcslen(infoText), &FontSize);

			if (info_sec_Count == 1)
			{
				DestX = Stage_Line[8].PosY + 5 - FontSize.cy / 2;
				infoTextPosX = -FontSize.cy;
			}
			//�۲� ��ġ(����)
			infoTextPosX += (DestX - fabs(infoTextPosX)) * 0.05;

			SetTextColor(hdc, RGB(139, 0, 0));
			SetBkMode(hdc, TRANSPARENT);
			TextOut(hdc, ClientRect.right / 2 - FontSize.cx / 2, (LONG)infoTextPosX, infoText, (int)_tcslen(infoText));

			SelectObject(hdc, oldFont);
			DeleteObject(hFont);
		}
	}
	~GAMEINFO() {}
};
GAMEINFO GameDataMANEGER;

#define EVENTE_NEXTSTAGE 6
void CALLBACK NextStage(HWND hWnd, UINT uMessage, UINT idEvent, DWORD dwTime)
{
	GameDataMANEGER.StageClear = FALSE;
	Flog.Property.SetRBPos({ ClientRect.right / 2 + Flog.Property.Size.cx / 2, Stage_Line[COUNT_LINE - 1].PosY - 5 });
	Flog.BeforeMovePoint = { ClientRect.right / 2 + Flog.Property.Size.cx / 2, Stage_Line[COUNT_LINE - 1].PosY - 5 };
	GameDataMANEGER.GameTime.Pause = FALSE;
	SetStageLevel(hWnd, GameDataMANEGER.level);
	KillTimer(hWnd, idEvent);
}

#define EVENTE_GAMEOVER 7
void CALLBACK GameOver(HWND hWnd, UINT uMessage, UINT idEvent, DWORD dwTime)
{
	GameDataMANEGER.init_All_GameData();
	SetStageLevel(hWnd, 1);
	GameDataMANEGER.GameTime.Pause = FALSE;
	KillTimer(hWnd, idEvent);
}

// MAX_Count = 13 - 3 + 1 = 11 (�� ������Ʈ�� �ִ� ���ΰ��� + 1)
void Additem(LONG Count)
{
	List_Item.DeleteAllitem();
	LONG Randindex = 0;
	for (LONG i = 0; i < Count; i++)
	{
		Randindex = 3 + rand() % 11;
		if (!List_Item.CheckSamePosY(Stage_Line[Randindex].PosY + 5))
		{
			POINT LTPos;
			LONG SpeedX = 0;
			BOOL DropFlag = FALSE;
			LTPos.y = Stage_Line[Randindex].PosY + 5;
			if (3 <= Randindex && Randindex <= 7)
			{
				LONG RandObjectindex = (Stage_Line[Randindex].ObjectCount > 0) ? rand() % Stage_Line[Randindex].ObjectCount : 0;
				RECT ObjectRegion = Stage_Line[Randindex].GetObjectRegion(RandObjectindex);
				SpeedX = Stage_Line[Randindex].SpeedX;
				LTPos.x = ObjectRegion.left + rand() % (ObjectRegion.right - ObjectRegion.left - Unit.cx - 5);
				DropFlag = TRUE;
			}
			else
				LTPos.x = 5 + rand() % (ClientRect.right - Unit.cx - 5);

			List_Item.AddItem(rand() % COUNT_GAMEITEM_IMG, LTPos, DropFlag, SpeedX);
		}
		else
			i--;
	}
}

void SetStageLevel(HWND hWnd, size_t Level)
{
	//�� ���δ� ��׶��� �� ������Ʈ ����, ����, �ӵ� �ο�
	if (Level == 1)
	{
		/////////////////////////////////////////////////////////////////////////////////////
		//                         Game information(LIFE & Score)                          //
		/////////////////////////////////////////////////////////////////////////////////////

		/////////////////////////////////////////////////////////////////////////////////////
		//                               Forest(Gorl in)                                   //
		Hunter.SetLTPos({ rand() % (ClientRect.right - Hunter.GetSize().cx), Stage_Line[2].PosY });
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
		/////////////////////////////////////////////////////////////////////////////////////

		/////////////////////////////////////////////////////////////////////////////////////
		//                            Beach(Before Gorl in)                                //
		/////////////////////////////////////////////////////////////////////////////////////

		/////////////////////////////////////��ó�����//////////////////////////////////////
		ObjectsColor = COLOR_WOOD;
		Stage_Line[3].SpeedX = 3;
		Stage_Line[3].Add_Objects(NULL, 12, { (LONG)(Unit.cx * 3 - 10), Unit.cy - 10 }, 1, 3);

		ObjectsColor = COLOR_TUTTLE;
		Stage_Line[4].SpeedX = -2;
		Stage_Line[4].Add_Objects(NULL, 20, { (LONG)(Unit.cx * 2 - 10), Unit.cy - 10 }, 2, 4);

		ObjectsColor = COLOR_WOOD;
		Stage_Line[5].SpeedX = 4;
		Stage_Line[5].Add_Objects(NULL, 12, { (LONG)(Unit.cx * 2 - 10), Unit.cy - 10 }, 1, 2);

		ObjectsColor = COLOR_WOOD;
		Stage_Line[6].SpeedX = -4;
		Stage_Line[6].Add_Objects(NULL, 11, { (LONG)(Unit.cx * 4 - 10), Unit.cy - 10 }, 1, 3);

		ObjectsColor = COLOR_TUTTLE;
		Stage_Line[7].SpeedX = 1;
		Stage_Line[7].Add_Objects(NULL, 20, { (LONG)(Unit.cx * 3 - 10), Unit.cy - 10 }, 3, 3);
		/////////////////////////////////////��ó�����//////////////////////////////////////

		/////////////////////////////////////////////////////////////////////////////////////
		//                              Walk Way(ReSpan Area)                              //
		/////////////////////////////////////////////////////////////////////////////////////

		/////////////////////////////////////�鵵�Ρ�////////////////////////////////////////
		ObjectsColor = COLOR_CAR_BLUE;
		Stage_Line[9].SpeedX = -3;
		Stage_Line[9].Add_Objects(NULL, 1, { (LONG)(Unit.cx * 2 - 10), Unit.cy - 10 }, 1, 2);

		ObjectsColor = COLOR_CAR_YELLOW;
		Stage_Line[10].SpeedX = 2;
		Stage_Line[10].Add_Objects(NULL, 6, { (LONG)(Unit.cx * 2 - 10), Unit.cy - 10 }, 1, 1);

		ObjectsColor = COLOR_CAR_RED;
		Stage_Line[11].SpeedX = -6;
		Stage_Line[11].Add_Objects(NULL, 3, { (LONG)(Unit.cx * 2 - 10), Unit.cy - 10 }, 1, 1);

		ObjectsColor = COLOR_CAR_GRAY;
		Stage_Line[12].SpeedX = 2;
		Stage_Line[12].Add_Objects(NULL, 8, { (LONG)(Unit.cx * 2 - 10), Unit.cy - 10 }, 1, 2);

		ObjectsColor = COLOR_CAR_BLUE;
		Stage_Line[13].SpeedX = -3;
		Stage_Line[13].Add_Objects(NULL, 1, { (LONG)(Unit.cx * 2 - 10), Unit.cy - 10 }, 1, 1);
		/////////////////////////////////////�赵�Ρ�////////////////////////////////////////

		/////////////////////////////////////////////////////////////////////////////////////
		//                              Walk Way(ReSpan Area)                              //
		/////////////////////////////////////////////////////////////////////////////////////

		/////////////////////////////////////////////////////////////////////////////////////
		//                            Game information(Time Bar)                           //
		GameDataMANEGER.GameTime.Set_LimitTime(SEC_LIMIT_TIME1);
		/////////////////////////////////////////////////////////////////////////////////////


		//��Game Item��//
		KillTimer(hWnd, EVENT_BONDAGE);
		Additem(3);
		//��Game Item��//
	}
	else if (Level == 2) {
		/////////////////////////////////////////////////////////////////////////////////////
		//                         Game information(LIFE & Score)                          //
		/////////////////////////////////////////////////////////////////////////////////////

		/////////////////////////////////////////////////////////////////////////////////////
		//                               Forest(Gorl in)                                   //
		Hunter.SetLTPos({ rand() % (ClientRect.right - Hunter.GetSize().cx), Stage_Line[2].PosY });
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
		SetTimer(hWnd, EVENT_APPEAR, 1000 * SEC_APPEAR_INTERVAL, (TIMERPROC)Appear);
		/////////////////////////////////////////////////////////////////////////////////////

		/////////////////////////////////////////////////////////////////////////////////////
		//                            Beach(Before Gorl in)                                //
		/////////////////////////////////////////////////////////////////////////////////////

		/////////////////////////////////////��ó�����//////////////////////////////////////
		ObjectsColor = COLOR_WOOD;
		Stage_Line[3].SpeedX = 4;
		Stage_Line[3].Add_Objects(NULL, 12, { (LONG)(Unit.cx * 3 - 10), Unit.cy - 10 }, 1, 2);

		ObjectsColor = COLOR_TUTTLE;
		Stage_Line[4].SpeedX = -5;
		Stage_Line[4].Add_Objects(NULL, 20, { (LONG)(Unit.cx * 2 - 10), Unit.cy - 10 }, 2, 3);

		ObjectsColor = COLOR_WOOD;
		Stage_Line[5].SpeedX = 6;
		Stage_Line[5].Add_Objects(NULL, 12, { (LONG)(Unit.cx * 2 - 10), Unit.cy - 10 }, 1, 3);

		ObjectsColor = COLOR_WOOD;
		Stage_Line[6].SpeedX = -6;
		Stage_Line[6].Add_Objects(NULL, 11, { (LONG)(Unit.cx * 3 - 10), Unit.cy - 10 }, 1, 1);

		ObjectsColor = COLOR_TUTTLE;
		Stage_Line[7].SpeedX = 2;
		Stage_Line[7].Add_Objects(NULL, 20, { (LONG)(Unit.cx * 3 - 10), Unit.cy - 10 }, 3, 2);
		/////////////////////////////////////��ó�����//////////////////////////////////////

		/////////////////////////////////////////////////////////////////////////////////////
		//                              Walk Way(ReSpan Area)                              //
		/////////////////////////////////////////////////////////////////////////////////////

		/////////////////////////////////////�鵵�Ρ�////////////////////////////////////////
		ObjectsColor = COLOR_CAR_BLUE;
		Stage_Line[9].SpeedX = -6;
		Stage_Line[9].Add_Objects(NULL, 1, { (LONG)(Unit.cx * 2 - 10), Unit.cy - 10 }, 1, 1);

		ObjectsColor = COLOR_CAR_YELLOW;
		Stage_Line[10].SpeedX = 3;
		Stage_Line[10].Add_Objects(NULL, 6, { (LONG)(Unit.cx * 2 - 10), Unit.cy - 10 }, 1, 2);

		ObjectsColor = COLOR_CAR_RED;
		Stage_Line[11].SpeedX = -8;
		Stage_Line[11].Add_Objects(NULL, 3, { (LONG)(Unit.cx * 2 - 10), Unit.cy - 10 }, 1, 1);

		ObjectsColor = COLOR_CAR_GRAY;
		Stage_Line[12].SpeedX = 5;
		Stage_Line[12].Add_Objects(NULL, 8, { (LONG)(Unit.cx * 2 - 10), Unit.cy - 10 }, 1, 2);

		ObjectsColor = COLOR_CAR_BLUE;
		Stage_Line[13].SpeedX = -4;
		Stage_Line[13].Add_Objects(NULL, 1, { (LONG)(Unit.cx * 2 - 10), Unit.cy - 10 }, 1, 2);
		/////////////////////////////////////�赵�Ρ�////////////////////////////////////////

		/////////////////////////////////////////////////////////////////////////////////////
		//                              Walk Way(ReSpan Area)                              //
		/////////////////////////////////////////////////////////////////////////////////////

		/////////////////////////////////////////////////////////////////////////////////////
		//                            Game information(Time Bar)                           //
		GameDataMANEGER.GameTime.Set_LimitTime(SEC_LIMIT_TIME2);
		/////////////////////////////////////////////////////////////////////////////////////

		//��Game Item��//
		KillTimer(hWnd, EVENT_BONDAGE);
		Additem(5);
		//��Game Item��//
	}
	else if (Level == 3) {
		/////////////////////////////////////////////////////////////////////////////////////
		//                         Game information(LIFE & Score)                          //
		/////////////////////////////////////////////////////////////////////////////////////

		/////////////////////////////////////////////////////////////////////////////////////
		//                               Forest(Gorl in)                                   //
		Hunter.SetLTPos({ rand() % (ClientRect.right - Hunter.GetSize().cx), Stage_Line[2].PosY });
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
		SetTimer(hWnd, EVENT_APPEAR, 1000 * SEC_APPEAR_INTERVAL, (TIMERPROC)Appear);
		/////////////////////////////////////////////////////////////////////////////////////

		/////////////////////////////////////////////////////////////////////////////////////
		//                            Beach(Before Gorl in)                                //
		/////////////////////////////////////////////////////////////////////////////////////

		/////////////////////////////////////��ó�����//////////////////////////////////////
		ObjectsColor = COLOR_WOOD;
		Stage_Line[3].SpeedX = 9;
		Stage_Line[3].Add_Objects(NULL, 12, { (LONG)(Unit.cx * 2 - 10), Unit.cy - 10 }, 1, 2);

		ObjectsColor = COLOR_TUTTLE;
		Stage_Line[4].SpeedX = -5;
		Stage_Line[4].Add_Objects(NULL, 20, { (LONG)(Unit.cx * 2 - 10), Unit.cy - 10 }, 2, 2);

		ObjectsColor = COLOR_WOOD;
		Stage_Line[5].SpeedX = 7;
		Stage_Line[5].Add_Objects(NULL, 12, { (LONG)(Unit.cx * 2 - 10), Unit.cy - 10 }, 1, 2);

		ObjectsColor = COLOR_WOOD;
		Stage_Line[6].SpeedX = -10;
		Stage_Line[6].Add_Objects(NULL, 11, { (LONG)(Unit.cx * 2 - 10), Unit.cy - 10 }, 1, 2);

		ObjectsColor = COLOR_TUTTLE;
		Stage_Line[7].SpeedX = 7;
		Stage_Line[7].Add_Objects(NULL, 20, { (LONG)(Unit.cx * 3 - 10), Unit.cy - 10 }, 3, 1);
		/////////////////////////////////////��ó�����//////////////////////////////////////

		/////////////////////////////////////////////////////////////////////////////////////
		//                              Walk Way(ReSpan Area)                              //
		/////////////////////////////////////////////////////////////////////////////////////

		/////////////////////////////////////�鵵�Ρ�////////////////////////////////////////
		ObjectsColor = COLOR_CAR_BLUE;
		Stage_Line[9].SpeedX = -5;
		Stage_Line[9].Add_Objects(NULL, 1, { (LONG)(Unit.cx * 2 - 10), Unit.cy - 10 }, 1, 2);

		ObjectsColor = COLOR_CAR_YELLOW;
		Stage_Line[10].SpeedX = 10;
		Stage_Line[10].Add_Objects(NULL, 6, { (LONG)(Unit.cx * 2 - 10), Unit.cy - 10 }, 1, 1);

		ObjectsColor = COLOR_CAR_RED;
		Stage_Line[11].SpeedX = -2;
		Stage_Line[11].Add_Objects(NULL, 3, { (LONG)(Unit.cx * 2 - 10), Unit.cy - 10 }, 1, 3);

		ObjectsColor = COLOR_CAR_GRAY;
		Stage_Line[12].SpeedX = 10;
		Stage_Line[12].Add_Objects(NULL, 8, { (LONG)(Unit.cx * 2 - 10), Unit.cy - 10 }, 1, 1);

		ObjectsColor = COLOR_CAR_BLUE;
		Stage_Line[13].SpeedX = -8;
		Stage_Line[13].Add_Objects(NULL, 1, { (LONG)(Unit.cx * 2 - 10), Unit.cy - 10 }, 1, 1);
		/////////////////////////////////////�赵�Ρ�////////////////////////////////////////

		/////////////////////////////////////////////////////////////////////////////////////
		//                              Walk Way(ReSpan Area)                              //
		/////////////////////////////////////////////////////////////////////////////////////

		/////////////////////////////////////////////////////////////////////////////////////
		//                            Game information(Time Bar)                           //
		GameDataMANEGER.GameTime.Set_LimitTime(SEC_LIMIT_TIME3);
		/////////////////////////////////////////////////////////////////////////////////////

		//��Game Item��//
		KillTimer(hWnd, EVENT_BONDAGE);
		Additem(3);
		//��Game Item��//
	}
}