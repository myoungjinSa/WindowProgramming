#pragma once
#include "Objects.h"

//���� ����
#define COUNT_LINE 16

//�� ���� �� ĭ�� ũ��� ���ο� ���ΰ� �����ϸ� �� ũ�⸦ 1Unit �̶� ��
SIZE Unit;
//Client����
RECT ClientRect;
//���콺 ��ǥ
POINT MousePoint;
//����Ű �Է�ó����
BOOL KeyBuffer[256];

/*
Client������ �� ���κ��� �з�
�� ���κ��� ������Ʈ���� ����

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

/*
��������
���� 1: �� ���δ� ���� ũ��(�ʺ�)�� ���߾� ������Ʈ�� ������ ���� ������Ʈ���� �յ���� ��ġ�ȴ�.
���� 2: �� ���δ� ������Ʈ�� ����, ������ �����ؾ� �Ѵ�.(�������Ḯ��Ʈ ����)
���� 3: �յ���� ��ġ�� ������ ������Ʈ���� �ؽ�ũ�Ѹ������ �����ؾ� �Ѵ�.
���� 4: ���ΰ��� �� ������ ������Ʈ�� �浹üũ�� �����ؾ� �Ѵ�.
*/
class STAGE_LINE
{
private:
	//���� ������ ������Ʈ ����
	LONG ObjectsType;

	//������Ʈ ���
	OBJECT *ObjectsHead;
	//��ũ�Ѹ��� ������Ʈ ���
	OBJECT *ObjectsHead_Clon;

	/*
	�յ�� ��ġ
	���� 1: �� ������Ʈ�׷��� ũ�� �� ������Ʈ�׷��� ������ ���� �� ������Ʈ�׷��� �յ���� �����ȴ�.
	*/
	void Equalintervals()
	{
		if (ObjectsHead != nullptr)
		{
			LONG TEMP = 0;
			DOUBLE SpaceSize = 0.0;

			TEMP = (LONG)(ObjectsHead->Size.cx * ObjectCount);
			//����ũ�� 
			SpaceSize = (DOUBLE)(Width - TEMP) / (ObjectCount + 1);

			OBJECT *CurrentObject = ObjectsHead, *CurrentObject_Clon = ObjectsHead_Clon;;
			LONG index = 1;
			//���� ������Ʈ ��ġ �����ϱ�(����ũ�� ���ϱ�)
			while (CurrentObject != nullptr)
			{
				CurrentObject->SetLTPos({ (LONG)(SpaceSize * index + CurrentObject->Size.cx * (index - 1)), PosY + 5 });

				//��ũ�Ѹ��� ������Ʈ ��ġ �����ϱ�(���� ������Ʈ ��ġ �� ������ �ʺ�)
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
	//���� ������ ������Ʈ ����
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
	//���ο� �� �ִ� ������Ʈ �����
	void DeleteAll()
	{
		//������Ʈ's �����
		OBJECT *DeleteData = nullptr;
		while (ObjectsHead != nullptr)
		{
			DeleteData = ObjectsHead;
			ObjectsHead = ObjectsHead->NextData;
			delete DeleteData;
		}

		//��ũ�Ѹ��� ������Ʈ's �����
		while (ObjectsHead_Clon != nullptr)
		{
			DeleteData = ObjectsHead_Clon;
			ObjectsHead_Clon = ObjectsHead_Clon->NextData;
			delete DeleteData;
		}
	}
	//���� ���ο� ������Ʈ �߰��ϱ�(����̹��� ����, ������Ʈ ����, ������Ʈ ũ��, ������Ʈ �̹��� ��� ���� ���� ����, ������Ʈ ����)
	void Add_Objects(LONG BackGroundType, LONG ObjectsType, SIZE ObjectSize, size_t SizeDivision_Column, size_t ObjectCount)
	{
		//������ �ִ� ������Ʈ �����
		DeleteAll();
		 
		//���� ������ ������Ʈ ���� �� ���� ����
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

				//������Ʈ ũ�� �ο��ϱ�
				CurrentObject->SetSize(ObjectSize);
				CurrentObject_Clon->SetSize(ObjectSize);

				//Division_Test
				CurrentObject->DivisionColumn = SizeDivision_Column;
				CurrentObject_Clon->DivisionColumn = SizeDivision_Column;
				//Color_Test
				CurrentObject->BackGroundColor = ObjectsColor;
				CurrentObject_Clon->BackGroundColor = ObjectsColor;

				//������Ʈ ������ ���� ������Ʈ�� �̹��� �ο��ϱ�
				if (ObjectsType != 0)
				{
					//���� ������Ʈ(�ź���)�� ���
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
					//�� ���� ���� ������Ʈ�� ���
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

				//������Ʈ ũ�� �ο��ϱ�
				CurrentObject->SetSize(ObjectSize);
				CurrentObject_Clon->SetSize(ObjectSize);

				//Division_Test
				CurrentObject->DivisionColumn = SizeDivision_Column;
				CurrentObject_Clon->DivisionColumn = SizeDivision_Column;
				//Color_Test
				CurrentObject->BackGroundColor = ObjectsColor;
				CurrentObject_Clon->BackGroundColor = ObjectsColor;

				//������Ʈ ������ ���� ������Ʈ�� �̹��� �ο��ϱ�
				if (ObjectsType != 0)
				{
					//�ź����� ���
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
					//�� ���� ���� ������Ʈ�� ���
					else
					{
						CurrentObject->SetImage(&List_IMG.hbmp_Instance[ObjectsType - 1], &List_IMG.bmp_Instance[ObjectsType - 1], COLOR_BACKGROUND_TRAN, SizeDivision_Column, NULL, 1, SEC_FRAME_CYCLE);
						CurrentObject_Clon->SetImage(&List_IMG.hbmp_Instance[ObjectsType - 1], &List_IMG.bmp_Instance[ObjectsType - 1], COLOR_BACKGROUND_TRAN, SizeDivision_Column, NULL, 1, SEC_FRAME_CYCLE);
					}
				}
			}
		}

		//������ ������Ʈ ��ġ �ο��ϱ�(������Ʈ ũ��� ������ ���� �յ� ���� ��ġ)
		Equalintervals();
	}
	//������Ʈ �����̱�
	void Move_Objects()
	{
		OBJECT *CurrentObject = ObjectsHead, *CurrentObject_Clon = ObjectsHead_Clon;;
		while (CurrentObject != nullptr)
		{
			CurrentObject->Move(SpeedX, 0);
			CurrentObject_Clon->Move(SpeedX, 0);

			//���� ���� ����� �� ��ũ�Ѹ�(���� ���� ���� �ȿ� �ִ� (������Ʈ ��ġ �� ���� ����ũ��)��ŭ ��ġ�̵�)
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
	//������Ʈ���� �浹üũ
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
	//�������� �浹üũ�� ������Ʈ ��ȯ
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
	//���� ���ο� �ִ� ������Ʈ �׸���
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