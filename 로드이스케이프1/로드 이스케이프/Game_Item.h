#pragma once
#include "GameCharacter.h"

/*
  아이템은 한 스테이지에서 랜덤한 위치에 랜덤한 갯수가 생성된다.
  랜덤 위치 높이는 3 ~ 12라인 중 랜덤한 PosY에 맞추어서 위치 지정(중복되는 위치 제외)
  만약 3 ~ 7라인에서 생성될 경우 움직이는 오브젝트위에 생성되어야 한다.
  (오브젝트 위에서 생성된 아이템 같은 경우 화면 밖에 넘어갈 경우 화면 밖에 있는 오브젝트 중 랜덤한 위치에 생성
  (개구리와 아이템이 충돌하면 점수 증가 및 충돌 아이템 제거)
*/

#define SCORE_ITEM 200

class ITEM
{
public:
	OBJECT Property;
	ITEM *NextData;
	LONG Type;
	LONG SpeedX;
	BOOL Drop;
	BOOL Inner_Window;
	ITEM()
	{
		NextData = nullptr;
		Type = -1;
		SpeedX = 0;
		Drop = FALSE;
		Inner_Window = TRUE;
	}
	~ITEM() {}
};

class LIST_ITEM
{
public:
	ITEM *ListTop;
	ITEM *CollisionItem;
	LIST_ITEM()
	{
		ListTop = nullptr;
		CollisionItem = nullptr;
	}
	//아이템 목록에 아이템 추가하기(종류, 위치)
	void AddItem(LONG ItemType, POINT LTPos, BOOL DropFlag, LONG SpeedX)
	{
		ITEM *NewItem = new ITEM;
		NewItem->Type = ItemType + 1;
		NewItem->Property.SetImage(&List_IMG.hbmp_Gameitem[ItemType], &List_IMG.bmp_Gameitem[ItemType], COLOR_BACKGROUND_TRAN, 1, 0, 1, SEC_FRAME_CYCLE);
		NewItem->Property.SetSize({ Unit.cx - 10, Unit.cy - 10 });
		NewItem->Property.SetLTPos(LTPos);
		NewItem->Drop = DropFlag;
		NewItem->SpeedX = SpeedX;
		if (ListTop == nullptr)
		{
			ListTop = NewItem;
		}
		else
		{
			NewItem->NextData = ListTop;
			ListTop = NewItem;
		}
	}
	BOOL CheckSamePosY(LONG PosY)
	{
		ITEM *CurrentItem = ListTop;
		while (CurrentItem != nullptr)
		{
			if (CurrentItem->Property.Region.top == PosY)
				return TRUE;
			CurrentItem = CurrentItem->NextData;
		}
		return FALSE;
	}
	//개구리와 충돌체크후 현재 아이템 목록에서 충돌한 아이템 제거하기(꺼내기)
	ITEM* CollsionWithPop(RECT Region)
	{
		ITEM *CurrentItem = ListTop, *PrevData = ListTop, *PopItem = nullptr;
		RECT intersRect = { 0,0,0,0 };
		while (CurrentItem != nullptr)
		{
			if (IntersectRect(&intersRect, &ListTop->Property.Region, &Region))
			{
				PopItem = ListTop;
				ListTop = ListTop->NextData;
				PopItem->NextData = nullptr;
				return PopItem;
			}
			else if(IntersectRect(&intersRect, &CurrentItem->Property.Region, &Region))
			{
				PrevData->NextData = CurrentItem->NextData;
				PopItem = CurrentItem;
				PopItem->NextData = nullptr;
				return PopItem;
			}
			PrevData = CurrentItem;
			CurrentItem = CurrentItem->NextData;
		}
		return nullptr;
	}
	//모든 아이템 삭제하기
	void DeleteAllitem()
	{
		ITEM* DeleteItem = ListTop;
		while (ListTop != nullptr)
		{
			DeleteItem = ListTop;
			ListTop = ListTop->NextData;
			delete DeleteItem;
		}
	}
	ITEM* SearchItemByPosY(LONG PosY)
	{
		ITEM *CurrentItem = ListTop;
		while (CurrentItem != nullptr)
		{
			if (CurrentItem->Property.Region.top == PosY)
				return CurrentItem;
			CurrentItem = CurrentItem->NextData;
		}
		return nullptr;
	}
	~LIST_ITEM()
	{
		DeleteAllitem();
	}
};

LIST_ITEM List_Item;

//속박 시간
#define EVENT_BONDAGE 9
#define SEC_BONDAGE 2

void CALLBACK RemoveSpiderWeb(HWND hWnd, UINT uMessage, UINT idEvent, DWORD dwTime)
{
	if (List_Item.CollisionItem != nullptr)
		delete List_Item.CollisionItem;
	List_Item.CollisionItem = nullptr;
	Flog.Bondage = FALSE;
	KillTimer(hWnd, idEvent);
}