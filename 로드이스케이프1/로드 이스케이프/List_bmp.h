#pragma once
#include "BaseLib.h"
#include "Math.h"
#include "resource.h"

//������ ���� ��ü �̹��� ����(��(0),��(3),��(6),��(9)����)
#define COUNT_AnimateFrame_Flog 15
#define INDEX_DeathFrame_Flog 12
//������ �� ��ǿ� �ʿ��� �̹��� ���� ex) ���������� �̵� (������ ���� �����̴� �̹��� 3��, ���� �غ�, ����, ���� �� ����)
#define COUNT_AnimateCycle_Flog 3
//�ź��� ���� ��ü �̹��� ����(��,�����)
#define COUNT_AnimateFrame_Tuttle 22
//�ź��� �� ��ǿ� �ʿ��� �̹��� ����
#define COUNT_AnimateCycle_Tuttle 11

//����ִϸ��̼ǿ� �ʿ��� �̹��� ����
#define COUNT_AnimateFrame_Heart 5


//Non Animation bmp ����(����, ����)
#define COUNT_INSTANCE_IMG 14

//���� ���� instance bmp ����(���, Ÿ�ӹ�, help, end ��)
#define COUNT_GAMEINFO_IMG 4

//���� ������ bmp ����(���, �ٳ���, ü�� ��)
#define COUNT_GAMEITEM_IMG 4

//������ ��� ����
#define COLOR_BACKGROUND_TRAN RGB(255, 255, 255)

/*
BackgroundType
0: �ܵ�, 1: �ε�, 2: ����, 3: �ó���, 4: �𷡻���(Ȥ�� ��), 5: �غ�(Ȥ�� �ó���, �ó����̶� ���� ���� �ִ� �̹���), 6: ��, 7: ���� �𷡻����� ���, 8: �ϴ�
IntanceType
Type 0 -> Empty
Type 20 -> 0: �ź���_1_right, 1: �ź���_2_right, 2: �ź���_3_right,...(right_sprite ���� ��ġ: 0, left_sprite ���� ��ġ: 12)
Type 1 -> ��(�Ķ�)L
Type 2 -> ��(�Ķ�)R
Type 3 -> ��(����)L
Type 4 -> ��(����)R
Type 5 -> ��(���)L
Type 6 -> ��(���)R
Type 7 -> ��(ȸ��1)L
Type 8 -> ��(ȸ��1)R
Type 9 -> ��(ȸ��2)L
Type 10 -> ��(ȸ��2)R
Type 11 -> �볪��L
Type 12 -> �볪��R
Type 13 -> ��ɲ�
Type 14 -> ȭ��
Type 15 -> �Ź���

Gameinfo
Type 1 -> ��Ʈ��
Type 2 -> Ÿ�ӹ�_BackGround
Type 3 -> Ÿ�ӹ�
Type 4 -> Ÿ�ӹ�_TimeArea

GameItem
Type 1 -> ���
Type 2 -> �ٳ���
Type 3 -> ü��

Hero
0: ������_1_up, 1: ������_2_up, 2: ������_3_up, 3: ������_4_right,... (up_sprite ���� ��ġ: 0, right_sprite ���� ��ġ: 3, left_sprite ���� ��ġ: 6)
*/

class LIST_IMG
{
public:
	HBITMAP hbmp_BackGround[2];
	//���� ���ۼ��� �̹���
	HBITMAP hbmp_Helpinfo_IMG[9];
	//Gameinfo hbmp
	HBITMAP hbmp_Gameinfo[COUNT_GAMEINFO_IMG];
	//Gameitem hbmp
	HBITMAP hbmp_Gameitem[COUNT_GAMEITEM_IMG];
	//Non Animation hbmp
	HBITMAP hbmp_Instance[COUNT_INSTANCE_IMG];
	//������ ��,�����
	HBITMAP hbmp_AnimateFlog[COUNT_AnimateFrame_Flog];
	//�ź��� ��,�����
	HBITMAP hbmp_AnimateTuttle[COUNT_AnimateFrame_Tuttle];

	//�����̹���
	HBITMAP hbmp_HeartAnimaition[COUNT_AnimateFrame_Heart];

	BITMAP bmp_Helpinfo_IMG[9];
	//Non Animation bmp
	BITMAP bmp_BackGround[2];
	//Gameinfo bmp
	BITMAP bmp_Gameinfo[COUNT_GAMEINFO_IMG];
	//Gameitem bmp
	BITMAP bmp_Gameitem[COUNT_GAMEITEM_IMG];
	//Gameinstnace bmp
	BITMAP bmp_Instance[COUNT_INSTANCE_IMG];
	//AnimateFlog bmp
	BITMAP bmp_AnimateFlog[COUNT_AnimateFrame_Flog];
	//AnimateTuttle hbmp
	BITMAP bmp_AnimateTuttle[COUNT_AnimateFrame_Tuttle];
	//AnimaiteHeart bmp
	BITMAP bmp_HeartAnimaition[COUNT_AnimateFrame_Heart];

	LIST_IMG() {}
	void SetIMG(HINSTANCE g_hInstnace)
	{
		//��� �̹��� 0: ��� ���, 1: ��� �ϴ�
		hbmp_BackGround[0] = (HBITMAP)LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_BG_TOP));
		hbmp_BackGround[1] = (HBITMAP)LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_BG_BOTTOM));
		GetObject(hbmp_BackGround[0], sizeof(BITMAP), &bmp_BackGround[0]);
		GetObject(hbmp_BackGround[1], sizeof(BITMAP), &bmp_BackGround[1]);

		//�������ۼ��� �̹���
		hbmp_Helpinfo_IMG[0] = (HBITMAP)LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_KEY_IMG));
		/*hbmp_Helpinfo_IMG[1] = (HBITMAP)LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_KEY_MANUAL2));
		hbmp_Helpinfo_IMG[2] = (HBITMAP)LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_KEY_MANUAL3));
		hbmp_Helpinfo_IMG[3] = (HBITMAP)LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_KEY_MANUAL4));
		hbmp_Helpinfo_IMG[4] = (HBITMAP)LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_KEY_MANUAL5));
		hbmp_Helpinfo_IMG[5] = (HBITMAP)LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_KEY_MANUAL4));
		hbmp_Helpinfo_IMG[6] = (HBITMAP)LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_KEY_MANUAL3));
		hbmp_Helpinfo_IMG[7] = (HBITMAP)LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_KEY_MANUAL2));
		hbmp_Helpinfo_IMG[8] = (HBITMAP)LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_KEY_MANUAL1));*/

		for (int i = 0; i < 9; i++) {
			GetObject(hbmp_Helpinfo_IMG[i], sizeof(BITMAP), &bmp_Helpinfo_IMG[i]);
		}

		//���� �̹���
		hbmp_HeartAnimaition[0] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_LIFE_1));
		hbmp_HeartAnimaition[1] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_LIFE_2));
		hbmp_HeartAnimaition[2] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_LIFE_3));
		hbmp_HeartAnimaition[3] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_LIFE_4));
		hbmp_HeartAnimaition[4] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_LIFE_5));
		for (int i = 0; i < COUNT_AnimateFrame_Heart; i++)
			GetObject(hbmp_HeartAnimaition[i], sizeof(BITMAP), &bmp_HeartAnimaition[i]);

		//Gameinfo �̹���
		hbmp_Gameinfo[0] = LoadBitmap(g_hInstnace,MAKEINTRESOURCE(IDB_INTRO));
		hbmp_Gameinfo[1] = LoadBitmap(g_hInstnace,MAKEINTRESOURCE(IDB_TIMEBAR_BACKGROUND));
		hbmp_Gameinfo[2] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_TIMEBAR));
		hbmp_Gameinfo[3] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_TIMEBAR_TIMEAREA));
		for (int i = 0; i < COUNT_GAMEINFO_IMG; i++)
			GetObject(hbmp_Gameinfo[i], sizeof(BITMAP), &bmp_Gameinfo[i]);

		//Gameitem �̹���
		hbmp_Gameitem[0] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_ITEM_APPLE));
		hbmp_Gameitem[1] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_ITEM_BANANA));
		hbmp_Gameitem[2] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_ITEM_CHERRY));
		hbmp_Gameitem[3] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_SPIDERWEB));
		for (int i = 0; i < COUNT_GAMEITEM_IMG; i++)
			GetObject(hbmp_Gameitem[i], sizeof(BITMAP), &bmp_Gameitem[i]);

		//���� ������Ʈ �̹���
		hbmp_Instance[0] = (HBITMAP)LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_CAR_BLUE_L));
		hbmp_Instance[1] = (HBITMAP)LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_CAR_BLUE_R));
		hbmp_Instance[2] = (HBITMAP)LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_CAR_RED_L));
		hbmp_Instance[3] = (HBITMAP)LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_CAR_RED_R));
		hbmp_Instance[4] = (HBITMAP)LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_CAR_YELLOW_L));
		hbmp_Instance[5] = (HBITMAP)LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_CAR_YELLOW_R));
		hbmp_Instance[6] = (HBITMAP)LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_CAR_GRAY1_L));
		hbmp_Instance[7] = (HBITMAP)LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_CAR_GRAY1_R));
		hbmp_Instance[8] = (HBITMAP)LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_CAR_GRAY2_L));
		hbmp_Instance[9] = (HBITMAP)LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_CAR_GRAY2_R));
		hbmp_Instance[10] = (HBITMAP)LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_WOOD_L));
		hbmp_Instance[11] = (HBITMAP)LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_WOOD_R));
		hbmp_Instance[12] = (HBITMAP)LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_HUNTER));
		hbmp_Instance[13] = (HBITMAP)LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_ARROW));
		for (int i = 0; i < COUNT_INSTANCE_IMG; i++)
			GetObject(hbmp_Instance[i], sizeof(BITMAP), &bmp_Instance[i]);

		//������ �����
		hbmp_AnimateFlog[0] = LoadBitmap(g_hInstnace,MAKEINTRESOURCE(IDB_FLOG_UP1));
		hbmp_AnimateFlog[1] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_FLOG_UP2));
		hbmp_AnimateFlog[2] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_FLOG_UP3));
		//������ �����
		hbmp_AnimateFlog[3] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_FLOG_L1));
		hbmp_AnimateFlog[4] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_FLOG_L2));
		hbmp_AnimateFlog[5] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_FLOG_L3));
		//������ �����
		hbmp_AnimateFlog[6] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_FLOG_DOWN1));
		hbmp_AnimateFlog[7] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_FLOG_DOWN2));
		hbmp_AnimateFlog[8] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_FLOG_DOWN3));
		//������ �����
		hbmp_AnimateFlog[9] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_FLOG_R1));
		hbmp_AnimateFlog[10] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_FLOG_R2));
		hbmp_AnimateFlog[11] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_FLOG_R3));

		//������ �׾��� �� �̹���
		hbmp_AnimateFlog[12] = (HBITMAP)LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_GHOST_1));
		hbmp_AnimateFlog[13] = (HBITMAP)LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_GHOST_2));
		hbmp_AnimateFlog[14] = (HBITMAP)LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_GHOST_3));

		for (int i = 0; i < COUNT_AnimateFrame_Flog; i++)
			GetObject(hbmp_AnimateFlog[i], sizeof(BITMAP), &bmp_AnimateFlog[i]);

		//�ź��� �����
		hbmp_AnimateTuttle[0] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_TUTTLE_R1));
		hbmp_AnimateTuttle[1] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_TUTTLE_R2));
		hbmp_AnimateTuttle[2] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_TUTTLE_R3));
		hbmp_AnimateTuttle[3] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_TUTTLE_R4));
		hbmp_AnimateTuttle[4] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_TUTTLE_R5));
		hbmp_AnimateTuttle[5] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_TUTTLE_R6));
		hbmp_AnimateTuttle[6] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_TUTTLE_R7));
		hbmp_AnimateTuttle[7] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_TUTTLE_R8));
		hbmp_AnimateTuttle[8] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_TUTTLE_R9));
		hbmp_AnimateTuttle[9] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_TUTTLE_R10));
		hbmp_AnimateTuttle[10] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_TUTTLE_R11));

		//�ź��� �����
		hbmp_AnimateTuttle[11] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_TUTTLE_L1));
		hbmp_AnimateTuttle[12] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_TUTTLE_L2));
		hbmp_AnimateTuttle[13] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_TUTTLE_L3));
		hbmp_AnimateTuttle[14] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_TUTTLE_L4));
		hbmp_AnimateTuttle[15] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_TUTTLE_L5));
		hbmp_AnimateTuttle[16] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_TUTTLE_L6));
		hbmp_AnimateTuttle[17] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_TUTTLE_L7));
		hbmp_AnimateTuttle[18] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_TUTTLE_L8));
		hbmp_AnimateTuttle[19] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_TUTTLE_L9));
		hbmp_AnimateTuttle[20] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_TUTTLE_L10));
		hbmp_AnimateTuttle[21] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_TUTTLE_L11));

		for (int i = 0; i < COUNT_AnimateFrame_Tuttle; i++)
			GetObject(hbmp_AnimateTuttle[i], sizeof(BITMAP), &bmp_AnimateTuttle[i]);
	}
	~LIST_IMG() {}
};

//�̹��� ���
LIST_IMG List_IMG;