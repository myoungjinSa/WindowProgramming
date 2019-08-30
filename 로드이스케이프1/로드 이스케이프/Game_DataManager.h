#pragma once
#include "GameTime.h"
#include "GameSound.h"

#define MAX_LIFE 3
#define COLOR_BUTTON RGB(0, 0, 0)
#define TIME_SCORE_INTERVAL 2

#define SEC_STAGECLAER 4
#define SEC_GAMEOVER 4

//스테이지 레벨
/*
Client영역을 각 라인별로 분류
각 라인별로 오브젝트들을 관리

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

Line[0]: (좌측 상단에 목숨 갯수, 우측 상단에 점수)
Line[15]: 현재 남은 시간에 비례하여 TimeBar가 줄어든다.

Line[2]: 골인 지점
.        (조건 1: 주인공 오브젝트가 골인 지점에 도달하면 현재 남은시간(초) * 100씩 점수 누적 후 현재 윈도우가 다음 스테이지로 전환)
.        (조건 2: 스테이지[2]부터 골인지점이 Line[1]전체가 아니라 Line[1]내의 수풀오브젝트(4개)가 골인지점이 된다.)
.        (조건 3: 스테이지[3]부터 사냥꾼이 수풀오브젝트 중 랜덤한 위치에서 나타나 개구리를 향해 화살을 쏘고 사라진다.)
.        (조건 4: 스테이지[3]부터 사냥꾼이 화살을 쏠 때에는 화살이 개구리 위치로 이동한다.
.                 사냥꾼의 현재위치로부터 개구리위치까지의 거리(sqrt(pow(dx,2)+pow(dy,2))를 구하고
.                 사냥꾼의 현재위치로부터 개구리위치까지의 x축 방향 벡터, y축 방향 벡터를 구해서(dx / 거리, dy / 거리)
.                 화살을 x축으로 x축방향벡터 * 화살속도만큼, y축으로 y축방향벡터 * 화살속도만큼 이동한다.
.                 ex) Arrow.Move((dx / 거리) * SPEED, (dy / 거리) * SPEED);
.        (조건 5: 스테이지[3]에서 발사된 화살은 Line[7]을 넘어가지 않는다.

Line[3] ~ Line[7]: 통나무와 거북이가 균등간격을 유지하면서 좌->우(혹은 우<-좌)로 스크롤링
.                  (조건 1: 통나무와 거북이가 흘러가는 방향은 서로 반대)
.                  (조건 2: 거북이는 한마리가 아니라 최소 2마리 이상 짝지어서 움직임)
.                  (조건 3: 각 라인별 이동 속도가 다르다)
.                  (조건 4: 주인공 오브젝트가 이동할 때 통나무나 거북이등 바깥 모서리에 걸쳐지게 되면 안쪽으로 이동하게 한다.(사각형 밖에 걸쳐지면 사각형 안쪽으로 이동))
.                  (조건 5: 주인공 오브젝트가 통나무나 거북이 오브젝트 영역 안에 있을 때 각 라인별 이동 속도에 맞추어 주인공도 같이 이동하게 된다.)
.                  (조건 6: 주인공 오브젝트가 다음 라인으로 이동할 때 해당 라인에서의 주인공오브젝트 위치에
통나무나 거북이 오브젝트가 없으면 주인공이 물에 빠지게 된다.)
.                  (조건 7: 주인공 오브젝트가 통나무나 거북이 오브젝트 영역 안에 있고,
통나무나 거북이 오브젝트가 ClientRect.right나 ClientRect.left를 벗어나면 주인공이 물에 빠지게 된다.)

Line[9] ~ Line[13]: 자동차가 균등간격을 유지하면서 스크롤링
.                   (조건 1: 홀수번째 라인은 우<-좌로 스크롤링, 짝수번째 라인은 우->좌로 스크롤링)
.                   (조건 2: 주인공 오브젝트와 자동차 오브젝트의 충돌이 일어나면 목숨갯수를 -1 감소하고, 주인공의 위치를 처음 시작위치(Line[14])로 이동시킨다.

Line[7], Line[14]: 주인공 오브젝트 리스폰 지점
.                   (조건 1: 주인공 오브젝트가 Line[2] ~ Line[6]에서 죽었을 경우에는 Line[7]에서 리스폰,
.                            주인공 오브젝트가 Line[8] ~ Line[13]에서 죽었을 경우에는 Line[14]에서 리스폰이 된다.)

각 모든 라인에서 주인공 오브젝트가 상, 하, 좌, 우 움직일 수 있다.
주인공 오브젝트가 Line[1](골인 지점)에 도달했을 때 Line[0]을 제외한 모든 오브젝트가 초기화 되고 재할당된다.(스테이지 갱신 ≒ 다음 레벨 스테이지로 전환)
*/
void SetStageLevel(HWND hWnd, size_t Level);

class GAMEINFO
{
private:
	TCHAR infoText[256];
	LONG ScoreCount;//누적된 점수까지 증가<-실제 윈도우에 출력되는 데이터
	RECT StartButton, HelpButton, PreviousButton;//버튼 영역
	BOOL ButtonUp_S, ButtonUp_H;//버튼 색상 활성화 플래그
	LONG RGB_s[3];
	LONG index_RGB;

	//infoTextPosX
	DOUBLE infoTextPosX;
	//알림텍스트 보여지는 시간
	LONG info_sec_Count;
	LONG DestX;
public:
	//누적된 점수(아이템 획득: 100점, 죽음: 현재점수/2)
	LONG StackScore;

	//게임 상태 플래그
	BOOL GameStart;
	BOOL GameHelp;
	BOOL GameEnd;
	
	//스테이지 전환 플래그
	BOOL StageClear;
	BOOL StageFail;

	GAMETIME GameTime;
	GAMESOUND GameSound;

	LONG Life;//목숨

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

	//알림텍스트 초기화
	void init_infoText()
	{
		memset(infoText, 0, sizeof(TCHAR) * 255);
		DestX = 0;
		infoTextPosX = 0.0;
		info_sec_Count = 0;
	}

	//인트로화면 전환 및 게임 데이터 레벨 1로 초기화
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
	//버튼 클릭 확인
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

	//마우스포인터와 버튼과의 충돌체크 <- 버튼 색상 활성화
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

	//인트로 화면 정보 출력
	void Draw_introinfo(HDC hdc)
	{
		HFONT hFont, oldFont;
		SIZE Size_Button;

		hFont = CreateFont(Unit.cy, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("휴먼둥근헤드라인"));
		oldFont = (HFONT)SelectObject(hdc, hFont);

		//GAME START 버튼 출력
		wsprintf(infoText, TEXT("GAME START"));
		GetTextExtentPoint(hdc, infoText, (int)_tcslen(infoText), &Size_Button);
		if (StartButton.left == 0 && StartButton.top == 0 && StartButton.right == 0 && StartButton.bottom == 0)
		{
			StartButton.left = ClientRect.right / 2 - Size_Button.cx / 2;
			StartButton.right = StartButton.left + Size_Button.cx;
			StartButton.top = Stage_Line[COUNT_LINE - 3].PosY - Size_Button.cy / 2;
			StartButton.bottom = StartButton.top + Size_Button.cy;
		}
		//버튼 색상 지정
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

		//GAME HELP 버튼 출력
		wsprintf(infoText, TEXT("GAME HELP"));
		GetTextExtentPoint(hdc, infoText, (int)_tcslen(infoText), &Size_Button);
		if (HelpButton.left == 0 && HelpButton.top == 0 && HelpButton.right == 0 && HelpButton.bottom == 0)
		{
			HelpButton.left = ClientRect.right / 2 - Size_Button.cx / 2;
			HelpButton.right = HelpButton.left + Size_Button.cx;
			HelpButton.top = Stage_Line[COUNT_LINE - 2].PosY - Size_Button.cy / 2;
			HelpButton.bottom = HelpButton.top + Size_Button.cy;
		}
		//버튼 색상 지정
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

	//도움말 화면 정보 출력
	void Draw_Helpinfo(HDC hdc)
	{
		//Help 이미지 출력
		//////////////////
		//              //
		//////////////////
		


		//뒤로가기(혹은 시작화면) 버튼 출력
		HFONT hFont, oldFont;
		SIZE FontSize;

		hFont = CreateFont(Unit.cy - 10, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("휴먼둥근헤드라인"));
		oldFont = (HFONT)SelectObject(hdc, hFont);
		wsprintf(infoText, TEXT("뒤로가기"));
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
	//게임정보 출력
	void Draw_Gameinfo(HDC hdc,LONG count_heart_Index)
	{
		/////////////////////
		//목숨 이미지 출력)//
		static HDC BackGrounddc;
		static HBITMAP hBit, oldBit;

		BackGrounddc = CreateCompatibleDC(hdc);

		SelectObject(BackGrounddc, List_IMG.hbmp_HeartAnimaition[count_heart_Index]);									//개구리 생명 이미지
		for (int i = 0; i <Life; i++) {
			TransparentBlt(hdc, ClientRect.left + List_IMG.bmp_HeartAnimaition[count_heart_Index].bmWidth*i, ClientRect.top + 10, List_IMG.bmp_HeartAnimaition[count_heart_Index].bmWidth, List_IMG.bmp_HeartAnimaition[count_heart_Index].bmHeight, BackGrounddc, 0, 0, List_IMG.bmp_HeartAnimaition[count_heart_Index].bmWidth, List_IMG.bmp_HeartAnimaition[count_heart_Index].bmHeight, RGB(255, 255, 255));
		}
		DeleteDC(BackGrounddc);
		//		/////////////////////
		/////////////////////

		HFONT hFont, oldFont;
		SIZE FontSize;

		hFont = CreateFont(Unit.cy - 20, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("휴먼둥근헤드라인"));
		oldFont = (HFONT)SelectObject(hdc, hFont);

		//점수 출력
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

		//타임바 출력
		GameTime.Draw_TimeBar(hdc);

		hFont = CreateFont(Unit.cy - 20, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("휴먼둥근헤드라인"));
		oldFont = (HFONT)SelectObject(hdc, hFont);

		//남은 시간 출력
		LONG RemainingTime = GameTime.Get_RemainingTime();
		DOUBLE WarningColor = (RemainingTime <= 10) ? (214 / 11) * (11 - RemainingTime) : 0;
		wsprintf(infoText, TEXT("%d"), RemainingTime);
		GetTextExtentPoint(hdc, infoText, (int)_tcslen(infoText), &FontSize);
		SetTextColor(hdc, RGB(139, 214 - (LONG)WarningColor, 0));
		SetBkMode(hdc, TRANSPARENT);
		TextOut(hdc, GameTime.TimeBar[2].CenterPoint.x - FontSize.cx / 2, GameTime.TimeBar[1].CenterPoint.y - FontSize.cy / 2, infoText, (int)_tcslen(infoText));

		SelectObject(hdc, oldFont);
		DeleteObject(hFont);

		//스테이지 클리어시
		if (StageClear)
		{
			info_sec_Count++;

			hFont = CreateFont(Unit.cy, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("휴먼둥근헤드라인"));
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
			//글꼴 위치(관성)
			infoTextPosX += (DestX - infoTextPosX) * 0.2;

			if (info_sec_Count < ((FPS*SEC_STAGECLAER) / 4 - 7) * 2)
			{
				//글꼴 그라데이션
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
		//게임오버 시
		else if (StageFail)
		{
			info_sec_Count++;

			hFont = CreateFont(Unit.cy, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("휴먼둥근헤드라인"));
			oldFont = (HFONT)SelectObject(hdc, hFont);
			wsprintf(infoText, TEXT("GAMEOVER"));
			GetTextExtentPoint(hdc, infoText, (int)_tcslen(infoText), &FontSize);

			if (info_sec_Count == 1)
			{
				DestX = Stage_Line[8].PosY + 5 - FontSize.cy / 2;
				infoTextPosX = -FontSize.cy;
			}
			//글꼴 위치(관성)
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

// MAX_Count = 13 - 3 + 1 = 11 (≒ 오브젝트가 있는 라인갯수 + 1)
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
	//각 라인당 백그라운드 및 오브젝트 종류, 갯수, 속도 부여
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

		/////////////////////////////////////↓시냇가↓//////////////////////////////////////
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
		/////////////////////////////////////↑시냇가↑//////////////////////////////////////

		/////////////////////////////////////////////////////////////////////////////////////
		//                              Walk Way(ReSpan Area)                              //
		/////////////////////////////////////////////////////////////////////////////////////

		/////////////////////////////////////↓도로↓////////////////////////////////////////
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
		/////////////////////////////////////↑도로↑////////////////////////////////////////

		/////////////////////////////////////////////////////////////////////////////////////
		//                              Walk Way(ReSpan Area)                              //
		/////////////////////////////////////////////////////////////////////////////////////

		/////////////////////////////////////////////////////////////////////////////////////
		//                            Game information(Time Bar)                           //
		GameDataMANEGER.GameTime.Set_LimitTime(SEC_LIMIT_TIME1);
		/////////////////////////////////////////////////////////////////////////////////////


		//↓Game Item↓//
		KillTimer(hWnd, EVENT_BONDAGE);
		Additem(3);
		//↑Game Item↑//
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

		/////////////////////////////////////↓시냇가↓//////////////////////////////////////
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
		/////////////////////////////////////↑시냇가↑//////////////////////////////////////

		/////////////////////////////////////////////////////////////////////////////////////
		//                              Walk Way(ReSpan Area)                              //
		/////////////////////////////////////////////////////////////////////////////////////

		/////////////////////////////////////↓도로↓////////////////////////////////////////
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
		/////////////////////////////////////↑도로↑////////////////////////////////////////

		/////////////////////////////////////////////////////////////////////////////////////
		//                              Walk Way(ReSpan Area)                              //
		/////////////////////////////////////////////////////////////////////////////////////

		/////////////////////////////////////////////////////////////////////////////////////
		//                            Game information(Time Bar)                           //
		GameDataMANEGER.GameTime.Set_LimitTime(SEC_LIMIT_TIME2);
		/////////////////////////////////////////////////////////////////////////////////////

		//↓Game Item↓//
		KillTimer(hWnd, EVENT_BONDAGE);
		Additem(5);
		//↑Game Item↑//
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

		/////////////////////////////////////↓시냇가↓//////////////////////////////////////
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
		/////////////////////////////////////↑시냇가↑//////////////////////////////////////

		/////////////////////////////////////////////////////////////////////////////////////
		//                              Walk Way(ReSpan Area)                              //
		/////////////////////////////////////////////////////////////////////////////////////

		/////////////////////////////////////↓도로↓////////////////////////////////////////
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
		/////////////////////////////////////↑도로↑////////////////////////////////////////

		/////////////////////////////////////////////////////////////////////////////////////
		//                              Walk Way(ReSpan Area)                              //
		/////////////////////////////////////////////////////////////////////////////////////

		/////////////////////////////////////////////////////////////////////////////////////
		//                            Game information(Time Bar)                           //
		GameDataMANEGER.GameTime.Set_LimitTime(SEC_LIMIT_TIME3);
		/////////////////////////////////////////////////////////////////////////////////////

		//↓Game Item↓//
		KillTimer(hWnd, EVENT_BONDAGE);
		Additem(3);
		//↑Game Item↑//
	}
}