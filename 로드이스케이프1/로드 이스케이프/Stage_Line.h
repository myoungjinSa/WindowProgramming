#pragma once
#include "Objects.h"

//라인 갯수
#define COUNT_LINE 16

//맵 상의 한 칸의 크기는 가로와 세로가 동일하며 그 크기를 1Unit 이라 함
SIZE Unit;
//Client영역
RECT ClientRect;
//마우스 좌표
POINT MousePoint;
//다중키 입력처리용
BOOL KeyBuffer[256];

/*
Client영역을 각 라인별로 분류
각 라인별로 오브젝트들을 관리

Line[0]  .....Game information1.....
Line[1]  ...........Frost...........
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

/*
라인정의
조건 1: 각 라인당 라인 크기(너비)에 맞추어 오브젝트의 갯수에 따라 오브젝트들이 균등분할 배치된다.
조건 2: 각 라인당 오브젝트의 삽입, 삭제가 용이해야 한다.(인접연결리스트 구현)
조건 3: 균등분할 배치된 각각의 오브젝트들의 ※스크롤링기법이 가능해야 한다.
조건 4: 주인공과 각 라인의 오브젝트와 충돌체크가 가능해야 한다.
*/
class STAGE_LINE
{
private:
	//현재 라인의 오브젝트 종류
	LONG ObjectsType;

	//오브젝트 헤드
	OBJECT *ObjectsHead;
	//스크롤링용 오브젝트 헤드
	OBJECT *ObjectsHead_Clon;

	/*
	균등간격 배치
	조건 1: 각 오브젝트그룹의 크기 및 오브젝트그룹의 갯수에 따라 각 오브젝트그룹의 균등간격이 조정된다.
	*/
	void Equalintervals()
	{
		if (ObjectsHead != nullptr)
		{
			LONG TEMP = 0;
			DOUBLE SpaceSize = 0.0;

			TEMP = (LONG)(ObjectsHead->Size.cx * ObjectCount);
			//여백크기 
			SpaceSize = (DOUBLE)(Width - TEMP) / (ObjectCount + 1);

			OBJECT *CurrentObject = ObjectsHead, *CurrentObject_Clon = ObjectsHead_Clon;;
			LONG index = 1;
			//현재 오브젝트 위치 지정하기(여백크기 더하기)
			while (CurrentObject != nullptr)
			{
				CurrentObject->SetLTPos({ (LONG)(SpaceSize * index + CurrentObject->Size.cx * (index - 1)), PosY + 5 });

				//스크롤링용 오브젝트 위치 지정하기(현재 오브젝트 위치 ± 윈도우 너비)
				if (SpeedX > 0)
					CurrentObject_Clon->SetLTPos({ CurrentObject->Region.left - Width - CurrentObject_Clon->Size.cx, CurrentObject->Region.top });
				else
					CurrentObject_Clon->SetLTPos({ CurrentObject->Region.left + Width + CurrentObject->Size.cx, CurrentObject->Region.top });

				index++;
				CurrentObject = CurrentObject->NextData;
				CurrentObject_Clon = CurrentObject_Clon->NextData;
			}
		}
	}
public:
	//현재 라인의 오브젝트 갯수
	size_t ObjectCount;

	LONG PosY;
	LONG Width;
	LONG SpeedX;
	STAGE_LINE()
	{
		ObjectsHead = nullptr;
		ObjectsHead_Clon = nullptr;
		ObjectCount = 0;
		PosY = 0;
		Width = 0;
		SpeedX = 0;
	}
	//라인에 들어가 있는 오브젝트 지우기
	void DeleteAll()
	{
		//오브젝트's 지우기
		OBJECT *DeleteData = nullptr;
		while (ObjectsHead != nullptr)
		{
			DeleteData = ObjectsHead;
			ObjectsHead = ObjectsHead->NextData;
			delete DeleteData;
		}

		//스크롤링용 오브젝트's 지우기
		while (ObjectsHead_Clon != nullptr)
		{
			DeleteData = ObjectsHead_Clon;
			ObjectsHead_Clon = ObjectsHead_Clon->NextData;
			delete DeleteData;
		}
	}
	//현재 라인에 오브젝트 추가하기(배경이미지 종류, 오브젝트 종류, 오브젝트 크기, 오브젝트 이미지 출력 영역 분할 갯수, 오브젝트 갯수)
	void Add_Objects(LONG BackGroundType, LONG ObjectsType, SIZE ObjectSize, size_t SizeDivision_Column, size_t ObjectCount)
	{
		//기존에 있는 오브젝트 지우기
		DeleteAll();
		 
		//현재 라인의 오브젝트 종류 및 갯수 지정
		this->ObjectsType = ObjectsType;
		this->ObjectCount = ObjectCount;

		OBJECT *CurrentObject = nullptr, *CurrentObject_Clon = nullptr;
		for (size_t index = 0; index < ObjectCount; index++)
		{
			if (ObjectsHead == nullptr)
			{
				ObjectsHead = new OBJECT;
				CurrentObject = ObjectsHead;
				ObjectsHead_Clon = new OBJECT;
				CurrentObject_Clon = ObjectsHead_Clon;

				//오브젝트 크기 부여하기
				CurrentObject->SetSize(ObjectSize);
				CurrentObject_Clon->SetSize(ObjectSize);

				//Division_Test
				CurrentObject->DivisionColumn = SizeDivision_Column;
				CurrentObject_Clon->DivisionColumn = SizeDivision_Column;
				//Color_Test
				CurrentObject->BackGroundColor = ObjectsColor;
				CurrentObject_Clon->BackGroundColor = ObjectsColor;

				//오브젝트 종류에 따른 오브젝트에 이미지 부여하기
				if (ObjectsType != 0)
				{
					//동적 오브젝트(거북이)일 경우
					if (ObjectsType == 20)
					{
						if (SpeedX > 0)
						{
							CurrentObject->SetImage(List_IMG.hbmp_AnimateTuttle, List_IMG.bmp_AnimateTuttle, COLOR_BACKGROUND_TRAN, SizeDivision_Column, 0, COUNT_AnimateCycle_Tuttle, SEC_FRAME_CYCLE);
							CurrentObject_Clon->SetImage(List_IMG.hbmp_AnimateTuttle, List_IMG.bmp_AnimateTuttle, COLOR_BACKGROUND_TRAN, SizeDivision_Column, 0, COUNT_AnimateCycle_Tuttle, SEC_FRAME_CYCLE);
						}
						else
						{
							CurrentObject->SetImage(List_IMG.hbmp_AnimateTuttle, List_IMG.bmp_AnimateTuttle, COLOR_BACKGROUND_TRAN, SizeDivision_Column, COUNT_AnimateCycle_Tuttle, COUNT_AnimateCycle_Tuttle, SEC_FRAME_CYCLE);
							CurrentObject_Clon->SetImage(List_IMG.hbmp_AnimateTuttle, List_IMG.bmp_AnimateTuttle, COLOR_BACKGROUND_TRAN, SizeDivision_Column, COUNT_AnimateCycle_Tuttle, COUNT_AnimateCycle_Tuttle, SEC_FRAME_CYCLE);
						}
					}
					//그 외의 정적 오브젝트일 경우
					else
					{
						CurrentObject->SetImage(&List_IMG.hbmp_Instance[ObjectsType - 1], &List_IMG.bmp_Instance[ObjectsType - 1], COLOR_BACKGROUND_TRAN, SizeDivision_Column, NULL, 1, SEC_FRAME_CYCLE);
						CurrentObject_Clon->SetImage(&List_IMG.hbmp_Instance[ObjectsType - 1], &List_IMG.bmp_Instance[ObjectsType - 1], COLOR_BACKGROUND_TRAN, SizeDivision_Column, NULL, 1, SEC_FRAME_CYCLE);
					}
				}
			}
			else
			{
				CurrentObject->NextData = new OBJECT;
				CurrentObject = CurrentObject->NextData;
				CurrentObject_Clon->NextData = new OBJECT;
				CurrentObject_Clon = CurrentObject_Clon->NextData;

				//오브젝트 크기 부여하기
				CurrentObject->SetSize(ObjectSize);
				CurrentObject_Clon->SetSize(ObjectSize);

				//Division_Test
				CurrentObject->DivisionColumn = SizeDivision_Column;
				CurrentObject_Clon->DivisionColumn = SizeDivision_Column;
				//Color_Test
				CurrentObject->BackGroundColor = ObjectsColor;
				CurrentObject_Clon->BackGroundColor = ObjectsColor;

				//오브젝트 종류에 따른 오브젝트에 이미지 부여하기
				if (ObjectsType != 0)
				{
					//거북이일 경우
					if (ObjectsType == 20)
					{
						if (SpeedX > 0)
						{
							CurrentObject->SetImage(List_IMG.hbmp_AnimateTuttle, List_IMG.bmp_AnimateTuttle, COLOR_BACKGROUND_TRAN, SizeDivision_Column, 0, COUNT_AnimateCycle_Tuttle, SEC_FRAME_CYCLE);
							CurrentObject_Clon->SetImage(List_IMG.hbmp_AnimateTuttle, List_IMG.bmp_AnimateTuttle, COLOR_BACKGROUND_TRAN, SizeDivision_Column, 0, COUNT_AnimateCycle_Tuttle, SEC_FRAME_CYCLE);
						}
						else
						{
							CurrentObject->SetImage(List_IMG.hbmp_AnimateTuttle, List_IMG.bmp_AnimateTuttle, COLOR_BACKGROUND_TRAN, SizeDivision_Column, COUNT_AnimateCycle_Tuttle, COUNT_AnimateCycle_Tuttle, SEC_FRAME_CYCLE);
							CurrentObject_Clon->SetImage(List_IMG.hbmp_AnimateTuttle, List_IMG.bmp_AnimateTuttle, COLOR_BACKGROUND_TRAN, SizeDivision_Column, COUNT_AnimateCycle_Tuttle, COUNT_AnimateCycle_Tuttle, SEC_FRAME_CYCLE);
						}
					}
					//그 외의 정적 오브젝트일 경우
					else
					{
						CurrentObject->SetImage(&List_IMG.hbmp_Instance[ObjectsType - 1], &List_IMG.bmp_Instance[ObjectsType - 1], COLOR_BACKGROUND_TRAN, SizeDivision_Column, NULL, 1, SEC_FRAME_CYCLE);
						CurrentObject_Clon->SetImage(&List_IMG.hbmp_Instance[ObjectsType - 1], &List_IMG.bmp_Instance[ObjectsType - 1], COLOR_BACKGROUND_TRAN, SizeDivision_Column, NULL, 1, SEC_FRAME_CYCLE);
					}
				}
			}
		}

		//생성된 오브젝트 위치 부여하기(오브젝트 크기와 갯수에 따른 균등 분할 배치)
		Equalintervals();
	}
	//오브젝트 움직이기
	void Move_Objects()
	{
		OBJECT *CurrentObject = ObjectsHead, *CurrentObject_Clon = ObjectsHead_Clon;;
		while (CurrentObject != nullptr)
		{
			CurrentObject->Move(SpeedX, 0);
			CurrentObject_Clon->Move(SpeedX, 0);

			//라인 영역 벗어났을 시 스크롤링(현재 라인 영역 안에 있는 (오브젝트 위치 ± 라인 가로크기)만큼 위치이동)
			if (SpeedX > 0)
			{
				if (CurrentObject->Region.left >= Width)
					CurrentObject->SetCenterPos({ CurrentObject_Clon->Region.left - Width - (CurrentObject->Size.cx / 2), CurrentObject->CenterPoint.y });
				else if (CurrentObject_Clon->Region.left >= Width)
					CurrentObject_Clon->SetCenterPos({ CurrentObject->Region.left - Width - (CurrentObject_Clon->Size.cx / 2), CurrentObject_Clon->CenterPoint.y });
			}
			else
			{
				if(CurrentObject->Region.right <= 0)
					CurrentObject->SetCenterPos({ CurrentObject_Clon->Region.right + Width + (CurrentObject->Size.cx / 2), CurrentObject->CenterPoint.y });
				else if(CurrentObject_Clon->Region.right <= 0)
					CurrentObject_Clon->SetCenterPos({ CurrentObject->Region.right + Width + (CurrentObject_Clon->Size.cx / 2), CurrentObject_Clon->CenterPoint.y });
			}
			CurrentObject = CurrentObject->NextData;
			CurrentObject_Clon = CurrentObject_Clon->NextData;
		}
	}
	//오브젝트와의 충돌체크
	BOOL Collision_Objects(RECT &Region)
	{
		OBJECT *CurrentObject = ObjectsHead, *CurrentObject_Clon = ObjectsHead_Clon;;
		RECT intersRect = { 0,0,0,0 };
		while (CurrentObject != nullptr)
		{
			if (IntersectRect(&intersRect, &CurrentObject->Region, &Region) || IntersectRect(&intersRect, &CurrentObject_Clon->Region, &Region))
				return TRUE;
			CurrentObject = CurrentObject->NextData;
			CurrentObject_Clon = CurrentObject_Clon->NextData;
		}

		return FALSE;
	}
	//개구리와 충돌체크한 오브젝트 반환
	OBJECT* GetObjectsByCollision(RECT &Region)
	{
		OBJECT *CurrentObject = ObjectsHead, *CurrentObject_Clon = ObjectsHead_Clon;;
		RECT intersRect = { 0,0,0,0 };
		while (CurrentObject != nullptr)
		{
			if (IntersectRect(&intersRect, &CurrentObject->Region, &Region))
				return CurrentObject;
			else if(IntersectRect(&intersRect, &CurrentObject_Clon->Region, &Region))
				return CurrentObject_Clon;
			CurrentObject = CurrentObject->NextData;
			CurrentObject_Clon = CurrentObject_Clon->NextData;
		}

		return nullptr;
	}

	RECT GetObjectRegion(LONG Objectindex)
	{
		OBJECT *CurrentObject = ObjectsHead;
		for (LONG i = 0; i < Objectindex; i++)
			CurrentObject = CurrentObject->NextData;
		return CurrentObject->Region;
	}
	//현재 라인에 있는 오브젝트 그리기
	void Draw_Objects(HDC hdc)
	{
		OBJECT *CurrentObject = ObjectsHead, *CurrentObject_Clon = ObjectsHead_Clon;;
		while (CurrentObject != nullptr)
		{
			CurrentObject->Draw(hdc);
			CurrentObject_Clon->Draw(hdc);
			CurrentObject = CurrentObject->NextData;
			CurrentObject_Clon = CurrentObject_Clon->NextData;
		}
	}
	~STAGE_LINE()
	{
		DeleteAll();
	}
};

STAGE_LINE Stage_Line[COUNT_LINE];