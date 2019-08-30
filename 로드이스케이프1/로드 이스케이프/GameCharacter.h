#pragma once
#include "Stage_Line.h"

/*
개구리가 1초에 Unit.cx, Unit.cy(41 Pixcel)만큼 이동한다.(<- 도약 거리 ≒ 한번에 이동하는 거리)
개구리가 1초에 1CycleFrame(도약준비, 도약, 도약 후 착지)이 재생된다.
개구리의 도약거리와 모션의 차이가 발생할 수 있다.
착지 후 바로 다시 도약할 수 있는 것이 아니라 약간의 도약준비 시간이 필요하다.
(개구리가 점프하는 모션을 관찰하면 바로 연속해서 점프하는 것도 약간의 시간이 필요하다.)
*/

//개구리 정의
//제일 이상적인 수치
#define SEC_JUMPINTERVAL 0.2
#define SEC_MOVE 0.2

//개구리 리스폰 이벤트
#define EVENTE_RESPAN 5
#define SEC_RESPAN 2

class FLOG
{
private:
	size_t JumpInterval_Sec_Count;//도약준비 시간간격
	DOUBLE Count_DistanceX;//도약거리_X (점프시작 위치로부터 매 프레임마다 Unit.cx / (FPS * SEC_MOVE)만큼 누적시키어 좌표지정)
	DOUBLE Count_DistanceY;//도약거리_Y (점프시작 위치로부터 매 프레임마다 Unit.cy / (FPS * SEC_MOVE)만큼 누적시키어 좌표지정)
public:
	OBJECT FlogDeathAnimation;
	BOOL JumpIntervalFlag;//도약 준비
	BOOL Death;
	BOOL Bondage;
	LONG CollsionType;//0: 충돌하지 않음, 1: 화살과 충돌함, 2: 자동차와 충돌함, 3: 물에 빠짐

	POINT BeforeMovePoint;//점프 시작 위치

	OBJECT Property;//각종 속성 & 기능들

	LONG Direction;//0: 정지, 1: left, 2: right, 3: up, 4: down


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
	//움직이기
	void Move()
	{
		//정지 상태가 아니면서 도약준비상태가 아닌 경우
		if (Direction != 0 && JumpIntervalFlag == FALSE)
		{
			if (fabs(Count_DistanceX) > Unit.cx)//점프 시작위치로부터 도약거리만큼 이동했을 때
			{
				//도약거리 재조정( Unit.cx / (FPS * SEC_MOVE)로 누적시키다보면 Unit.cx보다 커질 때가 있으므로.)
				if (Direction == 2)//→
					Count_DistanceX = Unit.cx;
				else if (Direction == 1)//←
					Count_DistanceX = -Unit.cx;

				//도약준비 플래그 발동
				JumpIntervalFlag = TRUE;
			}
			else//도약거리 누적
			{
				if (Direction == 2)//→
					Count_DistanceX += Unit.cx / (FPS * SEC_MOVE);
				else if (Direction == 1)//←
					Count_DistanceX -= Unit.cx / (FPS * SEC_MOVE);
			}

			if (fabs(Count_DistanceY) > Unit.cy)//점프 시작위치로부터 도약거리만큼 이동했을 때
			{
				//도약거리 재조정( Unit.cy / (FPS * SEC_MOVE)로 누적시키다보면 Unit.cy보다 커질 때가 있으므로.)
				if (Direction == 3)//↑
					Count_DistanceY = -Unit.cy;
				else if (Direction == 4)//↓
					Count_DistanceY = Unit.cy;

				//도약준비 플래그 발동
				JumpIntervalFlag = TRUE;
			}
			else//도약거리 누적
			{
				if (Direction == 3)//↑
					Count_DistanceY -= Unit.cy / (FPS * SEC_MOVE);
				else if (Direction == 4)//↓
					Count_DistanceY += Unit.cy / (FPS * SEC_MOVE);
			}

			//좌표지정 (이동이랑은 다른 개념.)
			Property.SetCenterPos({ BeforeMovePoint.x + (LONG)Count_DistanceX, BeforeMovePoint.y + (LONG)Count_DistanceY });
		}
		else//도약준비 플래그가 발동했을 때
		{
			//도약이 끝나고 현재 출력되고 있는 애니메이션 장면을 처음 시작 부분으로 옮기기(도약준비시간동안 계속 반복해서 출력되는 것을 방지하기 위함.)
			Property.FrameCount = Property.StartFrame;
			if (JumpInterval_Sec_Count > FPS * SEC_JUMPINTERVAL)//도약준비 시간이 끝났을 때
				initMoveProperty();
			else
			{
				if (JumpIntervalFlag) JumpInterval_Sec_Count++;//도약준비시간 간격 누적
			}
		}
	}
	//그리기
	void Draw(HDC hdc)
	{
		/*멈춰 있는 상태이면 도약 준비 이미지 출력
		(원래대로라면 Doodle 애니메이션을 출력해야 하나, 현재로선 Doodle 애니메이션에 관한 스프라이트가 없다.)
		Doodle Animation: 좌표이동이 없는 상태일 때 취하는 모션들
		ex) 숨을 쉬면서 가슴팍이나 전체 상체가 커지거나 줄어드는 모션, 팔이나, 다리가 조금씩 움직이는, 한마디로 꼼지락(?)거리는 모션*/
		if (!Death)
		{
			if (Direction == 0) Property.FrameCount = Property.StartFrame;
		}
		else//죽었을 때 죽은 이미지 출력
			Property.FrameCount = INDEX_DeathFrame_Flog;

		//죽는 이미지 만들면 수정할 예정
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
	if (Flog.CollsionType == 1 || Flog.CollsionType == 3)//화살과 충돌했을 시 혹은 물에 빠졌을 때
	{
		Flog.Property.SetLTPos({ ClientRect.right / 2 - Flog.Property.Size.cx / 2, Stage_Line[8].PosY + 5 });
		Flog.initMoveProperty();
		Flog.BeforeMovePoint = { ClientRect.right / 2 - Flog.Property.Size.cx / 2, Stage_Line[8].PosY + 5 };
	}
	else if (Flog.CollsionType == 2)//자동차와 충돌했을 시
	{
		Flog.Property.SetRBPos({ ClientRect.right / 2 + Flog.Property.Size.cx / 2, Stage_Line[COUNT_LINE - 1].PosY - 5 });
		Flog.initMoveProperty();
		Flog.BeforeMovePoint = { ClientRect.right / 2 + Flog.Property.Size.cx / 2, Stage_Line[COUNT_LINE - 1].PosY - 5 };
	}
	KillTimer(hWnd, idEvent);
}

//조준 시간
#define SEC_AIMING 3
//사냥꾼이 다시 나타나는 시간
#define SEC_APPEAR_INTERVAL 4
//화살 속도
#define SPEED_ARROW 9
//화살이 꽂혀잇는 시간
#define SEC_FIXED_ARROW 2
//사냥꾼 정의
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
	//오브젝트에 bmp 등록하기(call by reference)
	void SetImage(LONG ObjectType)
	{
		Property.SetImage(&List_IMG.hbmp_Instance[ObjectType - 1], &List_IMG.bmp_Instance[ObjectType - 1], COLOR_BACKGROUND_TRAN, 1, 0, 1, SEC_FRAME_CYCLE);
		Arrow.SetImage(&List_IMG.hbmp_Instance[ObjectType], &List_IMG.bmp_Instance[ObjectType], COLOR_BACKGROUND_TRAN, 1, 0, 1, SEC_FRAME_CYCLE);
	}

	//크기 지정하기 (Focus Center)
	void SetSize(SIZE Size)
	{
		Property.SetSize(Size);
		Arrow.SetSize({ List_IMG.bmp_Instance[13].bmWidth, List_IMG.bmp_Instance[13].bmHeight });
	}

	//위치 지정하기 (Focus LeftTop)
	void SetLTPos(POINT LTPoint)
	{
		Property.SetLTPos(LTPoint);
		Arrow.SetLTPos(LTPoint);
	}

	//이동하기
	void Move(LONG dx, LONG dy)
	{
		Property.Move(dx, dy);
		Arrow.Move(dx, dy);
	}
	//화살 이동하기
	void Move_Arrow(LONG dx, LONG dy)
	{
		Arrow.Move(dx, dy);
	}

	//중점 구하기
	POINT GetCenterPoint() { return Property.CenterPoint; }
	//회전각 구하기
	DOUBLE GetAngle() { return Shot_Angle; }
	//크기 구하기
	SIZE GetSize() { return Property.Size; }
	//화살 영역 구하기
	RECT GetArrowRect() { return Arrow.Region; }

	//목표와의 내각 구하기(제1사분면 x축이 기준)
	void SetDirection_Arrow(POINT TargetPoint)
	{
		Shot_Angle = getDegree(TORAD(Shot_Angle), TargetPoint, Property.CenterPoint);
		Property.Angle = Shot_Angle;
		Arrow.Angle = Shot_Angle;
	}
	//목표와의 충돌체크
	BOOL CollsionByTarget(RECT TargetRegion)
	{
		RECT intersRect;
		if (IntersectRect(&intersRect, &Arrow.Region, &TargetRegion))
			return TRUE;
		return FALSE;
	}

	//그리기
	void Draw(HDC hdc)
	{
		if (!Hide)
			Property.Draw(hdc);
	}
	//화살 그리기
	void Draw_Arrow(HDC hdc)
	{
		if (Arrow_Move)
			Arrow.Draw(hdc);
	}
	~HUNTER() {}
};
HUNTER Hunter;

//사냥꾼 이벤트
#define EVENT_APPEAR 2
#define EVENT_AIMING 3
#define EVENT_FIXED_ARROW 4
//사냥꾼 숨기
void CALLBACK Appear(HWND hWnd, UINT uMessage, UINT idEvent, DWORD dwTime)
{
	Hunter.Hide = FALSE;
	Hunter.SetLTPos({ rand() % (ClientRect.right - Hunter.GetSize().cx), Stage_Line[2].PosY });
	KillTimer(hWnd, idEvent);
}
//화살 표시하지 않기
void CALLBACK Disappear_Arrow(HWND hWnd, UINT uMessage, UINT idEvent, DWORD dwTime)
{
	Hunter.Arrow_Move = FALSE;
	Hunter.Arrow_Collision = FALSE;
	Hunter.Arrow.SetLTPos(Hunter.GetCenterPoint());
	KillTimer(hWnd, idEvent);
}
//조준하기
void CALLBACK Aiming(HWND hWnd, UINT uMessage, UINT idEvent, DWORD dwTime)
{
	Hunter.Shoot = TRUE;
	Hunter.Aiming = FALSE;
	Hunter.Arrow_Move = TRUE;
	//PlaySound(TEXT("bullseye.wav"), NULL, SND_ASYNC);//화살이 땅이나 개구리에 맞았을때 소리
	KillTimer(hWnd, idEvent);
}
