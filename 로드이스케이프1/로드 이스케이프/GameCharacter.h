#pragma once
#include "Stage_Line.h"

/*
�������� 1�ʿ� Unit.cx, Unit.cy(41 Pixcel)��ŭ �̵��Ѵ�.(<- ���� �Ÿ� �� �ѹ��� �̵��ϴ� �Ÿ�)
�������� 1�ʿ� 1CycleFrame(�����غ�, ����, ���� �� ����)�� ����ȴ�.
�������� ����Ÿ��� ����� ���̰� �߻��� �� �ִ�.
���� �� �ٷ� �ٽ� ������ �� �ִ� ���� �ƴ϶� �ణ�� �����غ� �ð��� �ʿ��ϴ�.
(�������� �����ϴ� ����� �����ϸ� �ٷ� �����ؼ� �����ϴ� �͵� �ణ�� �ð��� �ʿ��ϴ�.)
*/

//������ ����
//���� �̻����� ��ġ
#define SEC_JUMPINTERVAL 0.2
#define SEC_MOVE 0.2

//������ ������ �̺�Ʈ
#define EVENTE_RESPAN 5
#define SEC_RESPAN 2

class FLOG
{
private:
	size_t JumpInterval_Sec_Count;//�����غ� �ð�����
	DOUBLE Count_DistanceX;//����Ÿ�_X (�������� ��ġ�κ��� �� �����Ӹ��� Unit.cx / (FPS * SEC_MOVE)��ŭ ������Ű�� ��ǥ����)
	DOUBLE Count_DistanceY;//����Ÿ�_Y (�������� ��ġ�κ��� �� �����Ӹ��� Unit.cy / (FPS * SEC_MOVE)��ŭ ������Ű�� ��ǥ����)
public:
	OBJECT FlogDeathAnimation;
	BOOL JumpIntervalFlag;//���� �غ�
	BOOL Death;
	BOOL Bondage;
	LONG CollsionType;//0: �浹���� ����, 1: ȭ��� �浹��, 2: �ڵ����� �浹��, 3: ���� ����

	POINT BeforeMovePoint;//���� ���� ��ġ

	OBJECT Property;//���� �Ӽ� & ��ɵ�

	LONG Direction;//0: ����, 1: left, 2: right, 3: up, 4: down


	void initMoveProperty()
	{
		Direction = 0;
		JumpInterval_Sec_Count = 0;
		Count_DistanceX = 0.0;
		Count_DistanceY = 0.0;
		Property.SetFrameStart(Property.StartFrame);
		JumpIntervalFlag = FALSE;
	}

	FLOG()
	{
		Death = FALSE;
		Bondage = FALSE;
		CollsionType = 0;
		BeforeMovePoint = { 0,0 };
		initMoveProperty();
	}
	//�����̱�
	void Move()
	{
		//���� ���°� �ƴϸ鼭 �����غ���°� �ƴ� ���
		if (Direction != 0 && JumpIntervalFlag == FALSE)
		{
			if (fabs(Count_DistanceX) > Unit.cx)//���� ������ġ�κ��� ����Ÿ���ŭ �̵����� ��
			{
				//����Ÿ� ������( Unit.cx / (FPS * SEC_MOVE)�� ������Ű�ٺ��� Unit.cx���� Ŀ�� ���� �����Ƿ�.)
				if (Direction == 2)//��
					Count_DistanceX = Unit.cx;
				else if (Direction == 1)//��
					Count_DistanceX = -Unit.cx;

				//�����غ� �÷��� �ߵ�
				JumpIntervalFlag = TRUE;
			}
			else//����Ÿ� ����
			{
				if (Direction == 2)//��
					Count_DistanceX += Unit.cx / (FPS * SEC_MOVE);
				else if (Direction == 1)//��
					Count_DistanceX -= Unit.cx / (FPS * SEC_MOVE);
			}

			if (fabs(Count_DistanceY) > Unit.cy)//���� ������ġ�κ��� ����Ÿ���ŭ �̵����� ��
			{
				//����Ÿ� ������( Unit.cy / (FPS * SEC_MOVE)�� ������Ű�ٺ��� Unit.cy���� Ŀ�� ���� �����Ƿ�.)
				if (Direction == 3)//��
					Count_DistanceY = -Unit.cy;
				else if (Direction == 4)//��
					Count_DistanceY = Unit.cy;

				//�����غ� �÷��� �ߵ�
				JumpIntervalFlag = TRUE;
			}
			else//����Ÿ� ����
			{
				if (Direction == 3)//��
					Count_DistanceY -= Unit.cy / (FPS * SEC_MOVE);
				else if (Direction == 4)//��
					Count_DistanceY += Unit.cy / (FPS * SEC_MOVE);
			}

			//��ǥ���� (�̵��̶��� �ٸ� ����.)
			Property.SetCenterPos({ BeforeMovePoint.x + (LONG)Count_DistanceX, BeforeMovePoint.y + (LONG)Count_DistanceY });
		}
		else//�����غ� �÷��װ� �ߵ����� ��
		{
			//������ ������ ���� ��µǰ� �ִ� �ִϸ��̼� ����� ó�� ���� �κ����� �ű��(�����غ�ð����� ��� �ݺ��ؼ� ��µǴ� ���� �����ϱ� ����.)
			Property.FrameCount = Property.StartFrame;
			if (JumpInterval_Sec_Count > FPS * SEC_JUMPINTERVAL)//�����غ� �ð��� ������ ��
				initMoveProperty();
			else
			{
				if (JumpIntervalFlag) JumpInterval_Sec_Count++;//�����غ�ð� ���� ����
			}
		}
	}
	//�׸���
	void Draw(HDC hdc)
	{
		/*���� �ִ� �����̸� ���� �غ� �̹��� ���
		(������ζ�� Doodle �ִϸ��̼��� ����ؾ� �ϳ�, ����μ� Doodle �ִϸ��̼ǿ� ���� ��������Ʈ�� ����.)
		Doodle Animation: ��ǥ�̵��� ���� ������ �� ���ϴ� ��ǵ�
		ex) ���� ���鼭 �������̳� ��ü ��ü�� Ŀ���ų� �پ��� ���, ���̳�, �ٸ��� ���ݾ� �����̴�, �Ѹ���� ������(?)�Ÿ��� ���*/
		if (!Death)
		{
			if (Direction == 0) Property.FrameCount = Property.StartFrame;
		}
		else//�׾��� �� ���� �̹��� ���
			Property.FrameCount = INDEX_DeathFrame_Flog;

		//�״� �̹��� ����� ������ ����
		if (Death)
		{
			FlogDeathAnimation.Draw(hdc);
		}
		else
			Property.Draw(hdc);
	}
	~FLOG() {};
};
FLOG Flog;

void CALLBACK Respan_Flog(HWND hWnd, UINT uMessage, UINT idEvent, DWORD dwTime)
{
	Flog.Death = FALSE;
	Flog.Bondage = FALSE;
	Flog.Property.SetFrameStart(0);
	if (Flog.CollsionType == 1 || Flog.CollsionType == 3)//ȭ��� �浹���� �� Ȥ�� ���� ������ ��
	{
		Flog.Property.SetLTPos({ ClientRect.right / 2 - Flog.Property.Size.cx / 2, Stage_Line[8].PosY + 5 });
		Flog.initMoveProperty();
		Flog.BeforeMovePoint = { ClientRect.right / 2 - Flog.Property.Size.cx / 2, Stage_Line[8].PosY + 5 };
	}
	else if (Flog.CollsionType == 2)//�ڵ����� �浹���� ��
	{
		Flog.Property.SetRBPos({ ClientRect.right / 2 + Flog.Property.Size.cx / 2, Stage_Line[COUNT_LINE - 1].PosY - 5 });
		Flog.initMoveProperty();
		Flog.BeforeMovePoint = { ClientRect.right / 2 + Flog.Property.Size.cx / 2, Stage_Line[COUNT_LINE - 1].PosY - 5 };
	}
	KillTimer(hWnd, idEvent);
}

//���� �ð�
#define SEC_AIMING 3
//��ɲ��� �ٽ� ��Ÿ���� �ð�
#define SEC_APPEAR_INTERVAL 4
//ȭ�� �ӵ�
#define SPEED_ARROW 9
//ȭ���� �����մ� �ð�
#define SEC_FIXED_ARROW 2
//��ɲ� ����
class HUNTER
{
private:
	OBJECT Property;
	DOUBLE Shot_Angle;
public:
	OBJECT Arrow;
	BOOL Shoot;
	BOOL Appear;
	BOOL Arrow_Collision;
	BOOL Arrow_Move;
	BOOL Hide;
	BOOL Aiming;

	void initFlag()
	{
		Shoot = FALSE;
		Appear = FALSE;
		Hide = TRUE;
		Arrow_Collision = FALSE;
		Arrow_Move = FALSE;
		Aiming = FALSE;
	}
	HUNTER()
	{
		Shot_Angle = 0.0;
		initFlag();
	}
	//������Ʈ�� bmp ����ϱ�(call by reference)
	void SetImage(LONG ObjectType)
	{
		Property.SetImage(&List_IMG.hbmp_Instance[ObjectType - 1], &List_IMG.bmp_Instance[ObjectType - 1], COLOR_BACKGROUND_TRAN, 1, 0, 1, SEC_FRAME_CYCLE);
		Arrow.SetImage(&List_IMG.hbmp_Instance[ObjectType], &List_IMG.bmp_Instance[ObjectType], COLOR_BACKGROUND_TRAN, 1, 0, 1, SEC_FRAME_CYCLE);
	}

	//ũ�� �����ϱ� (Focus Center)
	void SetSize(SIZE Size)
	{
		Property.SetSize(Size);
		Arrow.SetSize({ List_IMG.bmp_Instance[13].bmWidth, List_IMG.bmp_Instance[13].bmHeight });
	}

	//��ġ �����ϱ� (Focus LeftTop)
	void SetLTPos(POINT LTPoint)
	{
		Property.SetLTPos(LTPoint);
		Arrow.SetLTPos(LTPoint);
	}

	//�̵��ϱ�
	void Move(LONG dx, LONG dy)
	{
		Property.Move(dx, dy);
		Arrow.Move(dx, dy);
	}
	//ȭ�� �̵��ϱ�
	void Move_Arrow(LONG dx, LONG dy)
	{
		Arrow.Move(dx, dy);
	}

	//���� ���ϱ�
	POINT GetCenterPoint() { return Property.CenterPoint; }
	//ȸ���� ���ϱ�
	DOUBLE GetAngle() { return Shot_Angle; }
	//ũ�� ���ϱ�
	SIZE GetSize() { return Property.Size; }
	//ȭ�� ���� ���ϱ�
	RECT GetArrowRect() { return Arrow.Region; }

	//��ǥ���� ���� ���ϱ�(��1��и� x���� ����)
	void SetDirection_Arrow(POINT TargetPoint)
	{
		Shot_Angle = getDegree(TORAD(Shot_Angle), TargetPoint, Property.CenterPoint);
		Property.Angle = Shot_Angle;
		Arrow.Angle = Shot_Angle;
	}
	//��ǥ���� �浹üũ
	BOOL CollsionByTarget(RECT TargetRegion)
	{
		RECT intersRect;
		if (IntersectRect(&intersRect, &Arrow.Region, &TargetRegion))
			return TRUE;
		return FALSE;
	}

	//�׸���
	void Draw(HDC hdc)
	{
		if (!Hide)
			Property.Draw(hdc);
	}
	//ȭ�� �׸���
	void Draw_Arrow(HDC hdc)
	{
		if (Arrow_Move)
			Arrow.Draw(hdc);
	}
	~HUNTER() {}
};
HUNTER Hunter;

//��ɲ� �̺�Ʈ
#define EVENT_APPEAR 2
#define EVENT_AIMING 3
#define EVENT_FIXED_ARROW 4
//��ɲ� ����
void CALLBACK Appear(HWND hWnd, UINT uMessage, UINT idEvent, DWORD dwTime)
{
	Hunter.Hide = FALSE;
	Hunter.SetLTPos({ rand() % (ClientRect.right - Hunter.GetSize().cx), Stage_Line[2].PosY });
	KillTimer(hWnd, idEvent);
}
//ȭ�� ǥ������ �ʱ�
void CALLBACK Disappear_Arrow(HWND hWnd, UINT uMessage, UINT idEvent, DWORD dwTime)
{
	Hunter.Arrow_Move = FALSE;
	Hunter.Arrow_Collision = FALSE;
	Hunter.Arrow.SetLTPos(Hunter.GetCenterPoint());
	KillTimer(hWnd, idEvent);
}
//�����ϱ�
void CALLBACK Aiming(HWND hWnd, UINT uMessage, UINT idEvent, DWORD dwTime)
{
	Hunter.Shoot = TRUE;
	Hunter.Aiming = FALSE;
	Hunter.Arrow_Move = TRUE;
	//PlaySound(TEXT("bullseye.wav"), NULL, SND_ASYNC);//ȭ���� ���̳� �������� �¾����� �Ҹ�
	KillTimer(hWnd, idEvent);
}
