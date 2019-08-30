#pragma once
#include "Game_Item.h"

//스테이지에 따른 제한시간(스테이지별 게임난이도에 따라 달라질 수 있음)
#define SEC_LIMIT_TIME1 20
#define SEC_LIMIT_TIME2 30
#define SEC_LIMIT_TIME3 30

//게임 타임
class GAMETIME
{
private:
	size_t timeLimit;//게임 제한 시간
	LONG CurrentTime;//지난 시간
	DOUBLE timeBarWidth;//타임바 너비
public:
	LONG Count_FPS;//
	BOOL Pause;
	LONG ClearTime;
	OBJECT TimeBar[3];
	GAMETIME()
	{
		Pause = FALSE;
		ClearTime = 0;
		timeLimit = 0;
		CurrentTime = 0;
		Count_FPS = 0;
		timeBarWidth = List_IMG.bmp_Gameinfo[2].bmWidth;
	}
	//타임바 위치 및 크기 지정하기
	void SetTimeBar()
	{
		for (size_t i = 0; i < 3; i++)
		{
			TimeBar[i].SetImage(&List_IMG.hbmp_Gameinfo[i + 1], &List_IMG.bmp_Gameinfo[i + 1], COLOR_BACKGROUND_TRAN, 1, 0, 1, 1);
			TimeBar[i].SetSize({ List_IMG.bmp_Gameinfo[i + 1].bmWidth, List_IMG.bmp_Gameinfo[i + 1].bmHeight });
		}
		TimeBar[0].SetCenterPos({ ClientRect.right / 2, Stage_Line[COUNT_LINE - 1].PosY + TimeBar[0].Size.cy / 2 });
		TimeBar[1].SetCenterPos({ TimeBar[0].CenterPoint });
		timeBarWidth = List_IMG.bmp_Gameinfo[2].bmWidth;
		TimeBar[2].SetCenterPos({ TimeBar[0].Region.left, TimeBar[0].CenterPoint.y });
	}


	//제한 시간 정하기
	void Set_LimitTime(size_t Time_sec)
	{
		timeLimit = Time_sec;
		Count_FPS = 0;
		CurrentTime = 0;
		TimeBar[1].SetSize({ List_IMG.bmp_Gameinfo[2].bmWidth, List_IMG.bmp_Gameinfo[2].bmHeight });
		timeBarWidth = List_IMG.bmp_Gameinfo[2].bmWidth;
	}
	//남은 시간 구하기
	LONG Get_RemainingTime()
	{
		return (LONG)timeLimit - CurrentTime;
	}
	//제한 시간 종료 확인하기
	BOOL Check_TimeOut()
	{
		if (!Get_RemainingTime())
			return TRUE;
		return FALSE;
	}

	//타임바 그리기
	void Draw_TimeBar(HDC hdc)
	{
		if (!Pause)
		{
			//시간 구하기
			if ((LONG)timeLimit * FPS > Count_FPS)
			{
				Count_FPS++;
				if (Count_FPS % FPS == 0)
					CurrentTime++;
			}
			//타임바 줄어들기
			if (timeBarWidth > 0.0)
				timeBarWidth -= (DOUBLE)List_IMG.bmp_Gameinfo[2].bmWidth / (FPS * (LONG)timeLimit);
			if (timeBarWidth < 0.0)
				timeBarWidth = 0.0;
		}
		else
		{
			//시간 구하기
			if ((LONG)timeLimit * FPS > Count_FPS)
			{
				Count_FPS += 10;
				if (Count_FPS % FPS == 0)
				{
					if(Get_RemainingTime())
						CurrentTime++;
				}
			}
			//타임바 줄어들기
			if (timeBarWidth > 0.0)
				timeBarWidth -= ((DOUBLE)List_IMG.bmp_Gameinfo[2].bmWidth / (FPS * (LONG)timeLimit)) * 10;
			if (timeBarWidth < 0.0)
				timeBarWidth = 0.0;
		}

		TimeBar[1].Size.cx = (LONG)timeBarWidth;
		TimeBar[1].Region.right = TimeBar[1].Region.left + (LONG)timeBarWidth;

		for (size_t i = 0; i < 3; i++)
			TimeBar[i].Draw(hdc);
	}
	~GAMETIME() {}
};
