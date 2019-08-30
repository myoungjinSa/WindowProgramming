#pragma once
#include "GameCharacter.h"

/*
  �������� �� ������������ ������ ��ġ�� ������ ������ �����ȴ�.
  ���� ��ġ ���̴� 3 ~ 12���� �� ������ PosY�� ���߾ ��ġ ����(�ߺ��Ǵ� ��ġ ����)
  ���� 3 ~ 7���ο��� ������ ��� �����̴� ������Ʈ���� �����Ǿ�� �Ѵ�.
  (������Ʈ ������ ������ ������ ���� ��� ȭ�� �ۿ� �Ѿ ��� ȭ�� �ۿ� �ִ� ������Ʈ �� ������ ��ġ�� ����
  (�������� �������� �浹�ϸ� ���� ���� �� �浹 ������ ����)
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
	//������ ��Ͽ� ������ �߰��ϱ�(����, ��ġ)
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
	//�������� �浹üũ�� ���� ������ ��Ͽ��� �浹�� ������ �����ϱ�(������)
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
	//��� ������ �����ϱ�
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

//�ӹ� �ð�
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