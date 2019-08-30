#pragma once
#include <fmod.h>
#include <mmsystem.h>

#define CHANNEL_SOUND 10
#define SOUND_BACKGROUND_MUSIC1 1
#define SOUND_BACKGROUND_MUSIC2 2

//���� ����
class GAMESOUND
{
private:
	FMOD_SYSTEM *system;
	FMOD_SOUND *soundFile[2];
	FMOD_CHANNEL *channel[2];
public:
	GAMESOUND()
	{
		FMOD_System_Create(&system);
		FMOD_System_Init(system, CHANNEL_SOUND, FMOD_INIT_NORMAL, NULL);	//�ʱ�ȭ
		FMOD_System_CreateSound(system, "Resource\\Sound\\town.mp3", FMOD_LOOP_NORMAL, 0, &soundFile[0]);//��� BGM ����
		FMOD_System_CreateSound(system, "Resource\\Sound\\car.mp3", FMOD_LOOP_NORMAL, 0, &soundFile[1]);//�ڵ��� BGM ����
	}
	void Reset_AllSound()
	{

	}
	void PlaySOUND(long Type)
	{
		FMOD_System_PlaySound(system, soundFile[Type], NULL, 0, &channel[Type]);//BGM ����
	}
	~GAMESOUND() {
		FMOD_System_Release(system);
	}
};