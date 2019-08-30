#pragma once
#include <math.h>

#define PI 3.1415926535897932
#define COUNT_POINT_TRANCE_ROTATION 3

double TORAD(double degree) { return degree * (PI / 180); }
double TODEGREE(double radian) { return radian * (180 / PI); }

/*
ȸ���� �׸��� �߾���ǥ(x2,y2) �� ���콺 �������� ��ǥ (x1,y1)���� ����
��ǻ���� ��ǥ��� Y���� �������� �ֱ⿡ Y�������� ������ �ο�
ź��Ʈ�Լ��� 2��и� 4��и鿡�� �������� ���⿡ 2��и�, 4��и鿡 180��, 360���� �������� �ٲ� ������.
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
theta: ��ȯ�� ������ ���� ��
CenterPoint = �׸��� �߾���ǥ(�׷��� ��ġ)
BitmapSize = �׸��� width, height
*/
void RotateBit(double degree, POINT CenterPoint, SIZE BitmapSize, POINT *GetPoint_TranceRotation)
{
	//��ǥ �ʱⰪ
	POINT tempPoint[COUNT_POINT_TRANCE_ROTATION];
	tempPoint[0] = { CenterPoint.x - (BitmapSize.cx / 2), CenterPoint.y - (BitmapSize.cy / 2) };//�»��
	tempPoint[1] = { CenterPoint.x + (BitmapSize.cx / 2), CenterPoint.y - (BitmapSize.cy / 2) };//����
	tempPoint[2] = { CenterPoint.x - (BitmapSize.cx / 2), CenterPoint.y + (BitmapSize.cy / 2) };//���ϴ�

	double radian = TORAD(degree); // ȸ���� ����(360�й�)��, ���ȴ����� ������ ����
	double cosine = cos(radian); // ȸ���̵���ȯ ��Ŀ� �� cos��Ÿ ���� ����
	double sine = sin(radian); // ȸ���̵���ȯ ��Ŀ� �� sin��Ÿ ���� ����

	//ȸ����ȯ ���Ŀ� ����
	for (int i = 0; i < COUNT_POINT_TRANCE_ROTATION; i++)
	{
		//������ ���� ȸ������ �ٲٱ� ���� �����̵�
		tempPoint[i].x -= CenterPoint.x;
		tempPoint[i].y -= CenterPoint.y;
		//ȸ����ȯ ���Ŀ� ����
		GetPoint_TranceRotation[i].x = (LONG)(((tempPoint[i].x) * cosine) - ((tempPoint[i].y) * sine));
		GetPoint_TranceRotation[i].y = (LONG)(((tempPoint[i].x) * sine) + ((tempPoint[i].y) * cosine));
		//���� �ִ� �߽������� �����̵�
		GetPoint_TranceRotation[i].x += CenterPoint.x;
		GetPoint_TranceRotation[i].y += CenterPoint.y;
	}
	
}