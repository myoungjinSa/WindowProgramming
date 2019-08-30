#pragma once
#include "BaseLib.h"
#include "Math.h"
#include "resource.h"

//개구리 관련 전체 이미지 갯수(↑(0),→(3),←(6),↓(9)방향)
#define COUNT_AnimateFrame_Flog 15
#define INDEX_DeathFrame_Flog 12
//개구리 한 모션에 필요한 이미지 갯수 ex) 오른쪽으로 이동 (오른쪽 방향 움직이는 이미지 3개, 도약 준비, 도약, 도약 후 착지)
#define COUNT_AnimateCycle_Flog 3
//거북이 관련 전체 이미지 갯수(→,←방향)
#define COUNT_AnimateFrame_Tuttle 22
//거북이 한 모션에 필요한 이미지 갯수
#define COUNT_AnimateCycle_Tuttle 11

//생명애니메이션에 필요한 이미지 갯수
#define COUNT_AnimateFrame_Heart 5


//Non Animation bmp 갯수(지형, 지물)
#define COUNT_INSTANCE_IMG 14

//게임 정보 instance bmp 갯수(목숨, 타임바, help, end 등)
#define COUNT_GAMEINFO_IMG 4

//게임 아이템 bmp 갯수(사과, 바나나, 체리 등)
#define COUNT_GAMEITEM_IMG 4

//제거할 배경 색상
#define COLOR_BACKGROUND_TRAN RGB(255, 255, 255)

/*
BackgroundType
0: 잔디, 1: 인도, 2: 도로, 3: 시냇물, 4: 모래사장(혹은 흙), 5: 해변(혹은 시냇가, 시냇물이랑 흙이 같이 있는 이미지), 6: 숲, 7: 숲과 모래사장의 경계, 8: 하늘
IntanceType
Type 0 -> Empty
Type 20 -> 0: 거북이_1_right, 1: 거북이_2_right, 2: 거북이_3_right,...(right_sprite 시작 위치: 0, left_sprite 시작 위치: 12)
Type 1 -> 차(파랑)L
Type 2 -> 차(파랑)R
Type 3 -> 차(빨간)L
Type 4 -> 차(빨간)R
Type 5 -> 차(노랑)L
Type 6 -> 차(노랑)R
Type 7 -> 차(회색1)L
Type 8 -> 차(회색1)R
Type 9 -> 차(회색2)L
Type 10 -> 차(회색2)R
Type 11 -> 통나무L
Type 12 -> 통나무R
Type 13 -> 사냥꾼
Type 14 -> 화살
Type 15 -> 거미줄

Gameinfo
Type 1 -> 인트로
Type 2 -> 타임바_BackGround
Type 3 -> 타임바
Type 4 -> 타임바_TimeArea

GameItem
Type 1 -> 사과
Type 2 -> 바나나
Type 3 -> 체리

Hero
0: 개구리_1_up, 1: 개구리_2_up, 2: 개구리_3_up, 3: 개구리_4_right,... (up_sprite 시작 위치: 0, right_sprite 시작 위치: 3, left_sprite 시작 위치: 6)
*/

class LIST_IMG
{
public:
	HBITMAP hbmp_BackGround[2];
	//게임 조작설명 이미지
	HBITMAP hbmp_Helpinfo_IMG[9];
	//Gameinfo hbmp
	HBITMAP hbmp_Gameinfo[COUNT_GAMEINFO_IMG];
	//Gameitem hbmp
	HBITMAP hbmp_Gameitem[COUNT_GAMEITEM_IMG];
	//Non Animation hbmp
	HBITMAP hbmp_Instance[COUNT_INSTANCE_IMG];
	//개구리 ↑,→방향
	HBITMAP hbmp_AnimateFlog[COUNT_AnimateFrame_Flog];
	//거북이 ←,→방향
	HBITMAP hbmp_AnimateTuttle[COUNT_AnimateFrame_Tuttle];

	//생명이미지
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
		//배경 이미지 0: 배경 상단, 1: 배경 하단
		hbmp_BackGround[0] = (HBITMAP)LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_BG_TOP));
		hbmp_BackGround[1] = (HBITMAP)LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_BG_BOTTOM));
		GetObject(hbmp_BackGround[0], sizeof(BITMAP), &bmp_BackGround[0]);
		GetObject(hbmp_BackGround[1], sizeof(BITMAP), &bmp_BackGround[1]);

		//게임조작설명 이미지
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

		//생명 이미지
		hbmp_HeartAnimaition[0] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_LIFE_1));
		hbmp_HeartAnimaition[1] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_LIFE_2));
		hbmp_HeartAnimaition[2] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_LIFE_3));
		hbmp_HeartAnimaition[3] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_LIFE_4));
		hbmp_HeartAnimaition[4] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_LIFE_5));
		for (int i = 0; i < COUNT_AnimateFrame_Heart; i++)
			GetObject(hbmp_HeartAnimaition[i], sizeof(BITMAP), &bmp_HeartAnimaition[i]);

		//Gameinfo 이미지
		hbmp_Gameinfo[0] = LoadBitmap(g_hInstnace,MAKEINTRESOURCE(IDB_INTRO));
		hbmp_Gameinfo[1] = LoadBitmap(g_hInstnace,MAKEINTRESOURCE(IDB_TIMEBAR_BACKGROUND));
		hbmp_Gameinfo[2] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_TIMEBAR));
		hbmp_Gameinfo[3] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_TIMEBAR_TIMEAREA));
		for (int i = 0; i < COUNT_GAMEINFO_IMG; i++)
			GetObject(hbmp_Gameinfo[i], sizeof(BITMAP), &bmp_Gameinfo[i]);

		//Gameitem 이미지
		hbmp_Gameitem[0] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_ITEM_APPLE));
		hbmp_Gameitem[1] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_ITEM_BANANA));
		hbmp_Gameitem[2] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_ITEM_CHERRY));
		hbmp_Gameitem[3] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_SPIDERWEB));
		for (int i = 0; i < COUNT_GAMEITEM_IMG; i++)
			GetObject(hbmp_Gameitem[i], sizeof(BITMAP), &bmp_Gameitem[i]);

		//정적 오브젝트 이미지
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

		//개구리 ↑방향
		hbmp_AnimateFlog[0] = LoadBitmap(g_hInstnace,MAKEINTRESOURCE(IDB_FLOG_UP1));
		hbmp_AnimateFlog[1] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_FLOG_UP2));
		hbmp_AnimateFlog[2] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_FLOG_UP3));
		//개구리 ←방향
		hbmp_AnimateFlog[3] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_FLOG_L1));
		hbmp_AnimateFlog[4] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_FLOG_L2));
		hbmp_AnimateFlog[5] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_FLOG_L3));
		//개구리 ↓방향
		hbmp_AnimateFlog[6] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_FLOG_DOWN1));
		hbmp_AnimateFlog[7] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_FLOG_DOWN2));
		hbmp_AnimateFlog[8] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_FLOG_DOWN3));
		//개구리 →방향
		hbmp_AnimateFlog[9] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_FLOG_R1));
		hbmp_AnimateFlog[10] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_FLOG_R2));
		hbmp_AnimateFlog[11] = LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_FLOG_R3));

		//개구리 죽었을 때 이미지
		hbmp_AnimateFlog[12] = (HBITMAP)LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_GHOST_1));
		hbmp_AnimateFlog[13] = (HBITMAP)LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_GHOST_2));
		hbmp_AnimateFlog[14] = (HBITMAP)LoadBitmap(g_hInstnace, MAKEINTRESOURCE(IDB_GHOST_3));

		for (int i = 0; i < COUNT_AnimateFrame_Flog; i++)
			GetObject(hbmp_AnimateFlog[i], sizeof(BITMAP), &bmp_AnimateFlog[i]);

		//거북이 →방향
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

		//거북이 ←방향
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

//이미지 목록
LIST_IMG List_IMG;