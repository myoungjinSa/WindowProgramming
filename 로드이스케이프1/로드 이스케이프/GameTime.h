#pragma once
#include "Game_Item.h"

//���������� ���� ���ѽð�(���������� ���ӳ��̵��� ���� �޶��� �� ����)
#define SEC_LIMIT_TIME1 20
#define SEC_LIMIT_TIME2 30
#define SEC_LIMIT_TIME3 30

//���� Ÿ��
class GAMETIME
{
private:
	size_t timeLimit;//���� ���� �ð�
	LONG CurrentTime;//���� �ð�
	DOUBLE timeBarWidth;//Ÿ�ӹ� �ʺ�
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
	//Ÿ�ӹ� ��ġ �� ũ�� �����ϱ�
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


	//���� �ð� ���ϱ�
	void Set_LimitTime(size_t Time_sec)
	{
		timeLimit = Time_sec;
		Count_FPS = 0;
		CurrentTime = 0;
		TimeBar[1].SetSize({ List_IMG.bmp_Gameinfo[2].bmWidth, List_IMG.bmp_Gameinfo[2].bmHeight });
		timeBarWidth = List_IMG.bmp_Gameinfo[2].bmWidth;
	}
	//���� �ð� ���ϱ�
	LONG Get_RemainingTime()
	{
		return (LONG)timeLimit - CurrentTime;
	}
	//���� �ð� ���� Ȯ���ϱ�
	BOOL Check_TimeOut()
	{
		if (!Get_RemainingTime())
			return TRUE;
		return FALSE;
	}

	//Ÿ�ӹ� �׸���
	void Draw_TimeBar(HDC hdc)
	{
		if (!Pause)
		{
			//�ð� ���ϱ�
			if ((LONG)timeLimit * FPS > Count_FPS)
			{
				Count_FPS++;
				if (Count_FPS % FPS == 0)
					CurrentTime++;
			}
			//Ÿ�ӹ� �پ���
			if (timeBarWidth > 0.0)
				timeBarWidth -= (DOUBLE)List_IMG.bmp_Gameinfo[2].bmWidth / (FPS * (LONG)timeLimit);
			if (timeBarWidth < 0.0)
				timeBarWidth = 0.0;
		}
		else
		{
			//�ð� ���ϱ�
			if ((LONG)timeLimit * FPS > Count_FPS)
			{
				Count_FPS += 10;
				if (Count_FPS % FPS == 0)
				{
					if(Get_RemainingTime())
						CurrentTime++;
				}
			}
			//Ÿ�ӹ� �پ���
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
