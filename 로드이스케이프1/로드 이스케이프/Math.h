#pragma once
#include <math.h>

#define PI 3.1415926535897932
#define COUNT_POINT_TRANCE_ROTATION 3

double TORAD(double degree) { return degree * (PI / 180); }
double TODEGREE(double radian) { return radian * (180 / PI); }

/*
회전할 그림의 중앙좌표(x2,y2) 와 마우스 포인터의 좌표 (x1,y1)간의 내각
컴퓨터의 좌표계는 Y축이 뒤집어져 있기에 Y증가량에 음수를 부여
탄젠트함수는 2사분면 4사분면에서 음수값을 갖기에 2사분면, 4사분면에 180도, 360도를 라디안으로 바꿔 더해줌.
*/
double getRadian(double CurrentDegree, POINT Point1, POINT Point2)
{
	CurrentDegree = TORAD(CurrentDegree);
	if (Point1.x != Point2.x && Point1.y != Point2.y)
		CurrentDegree = atan((Point1.y - Point2.y) / (Point1.x - Point2.x));
	if ((Point1.x - Point2.x) < 0 && Point1.y != Point2.y)
		CurrentDegree += TORAD(180);
	if ((Point1.x - Point2.x) > 0 && (Point1.y - Point2.y) < 0)
		CurrentDegree += TORAD(360);
	return CurrentDegree;
}
double getDegree(double CurrentRadian, POINT Point1, POINT Point2)
{
	if (Point1.x != Point2.x && Point1.y != Point2.y)
		CurrentRadian = atan((Point1.y - Point2.y) / (Point1.x - Point2.x));
	if ((Point1.x - Point2.x) < 0 && Point1.y != Point2.y)
		CurrentRadian += TORAD(180);
	if ((Point1.x - Point2.x) > 0 && (Point1.y - Point2.y) < 0)
		CurrentRadian += TORAD(360);
	return TODEGREE(CurrentRadian);
}

/*
theta: 변환할 각도의 라디안 값
CenterPoint = 그림의 중앙좌표(그려질 위치)
BitmapSize = 그림의 width, height
*/
void RotateBit(double degree, POINT CenterPoint, SIZE BitmapSize, POINT *GetPoint_TranceRotation)
{
	//좌표 초기값
	POINT tempPoint[COUNT_POINT_TRANCE_ROTATION];
	tempPoint[0] = { CenterPoint.x - (BitmapSize.cx / 2), CenterPoint.y - (BitmapSize.cy / 2) };//좌상단
	tempPoint[1] = { CenterPoint.x + (BitmapSize.cx / 2), CenterPoint.y - (BitmapSize.cy / 2) };//우상단
	tempPoint[2] = { CenterPoint.x - (BitmapSize.cx / 2), CenterPoint.y + (BitmapSize.cy / 2) };//좌하단

	double radian = TORAD(degree); // 회전할 각도(360분법)를, 라디안단위의 각도로 변경
	double cosine = cos(radian); // 회전이동변환 행렬에 들어갈 cos세타 값을 구함
	double sine = sin(radian); // 회전이동변환 행렬에 들어갈 sin세타 값을 구함

	//회전변환 공식에 대입
	for (int i = 0; i < COUNT_POINT_TRANCE_ROTATION; i++)
	{
		//원점에 대한 회전으로 바꾸기 위해 평행이동
		tempPoint[i].x -= CenterPoint.x;
		tempPoint[i].y -= CenterPoint.y;
		//회전변환 공식에 대입
		GetPoint_TranceRotation[i].x = (LONG)(((tempPoint[i].x) * cosine) - ((tempPoint[i].y) * sine));
		GetPoint_TranceRotation[i].y = (LONG)(((tempPoint[i].x) * sine) + ((tempPoint[i].y) * cosine));
		//원래 있던 중심점으로 평행이동
		GetPoint_TranceRotation[i].x += CenterPoint.x;
		GetPoint_TranceRotation[i].y += CenterPoint.y;
	}
	
}