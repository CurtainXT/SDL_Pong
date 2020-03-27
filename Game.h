#pragma once
#include "SDL.h"

struct Vector2
{
	float x;
	float y;
};

class Player
{
public:
	friend class Game;
	Player();
private:
	// ����λ��
	Vector2 mPaddlePos;
	// �����ƶ�����
	int mPaddleDir;
	// ���ĳ���
	int mPaddleH;
	// ���Ŀ��
	int mThickness;
};

class Game
{
public:
	Game();
	
	// ��ʼ����Ϸ
	bool Initialize();

	// ������Ϸѭ��ֱ����Ϸ����
	void RunLoop();

	// �ر���Ϸ
	void Shutdown();

	// ǽ�ĺ��
	int wallThickness;
private:
	// ��Ϸѭ���ĸ�������
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	// SDL�����Ĵ���
	SDL_Window* mWindow;
	// ��ϷӦ�ü�����
	bool mIsRunning;
	// ��Ⱦ��
	SDL_Renderer* mRenderer;

	// ���
	Player player1;
	// ���λ��
	Vector2 mBallPos;
	// ����ٶȣ����ʺͷ���
	Vector2 mBallVel;
	// ��ĺ��
	int ballThickness;

	// ���ڼ�������ʱ�䣨delta time��
	Uint32 mTicksCount;
};