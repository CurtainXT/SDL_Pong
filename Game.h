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
	// 球拍位置
	Vector2 mPaddlePos;
	// 球拍移动方向
	int mPaddleDir;
	// 球拍长度
	int mPaddleH;
	// 球拍宽度
	int mThickness;
};

class Game
{
public:
	Game();
	
	// 初始化游戏
	bool Initialize();

	// 运行游戏循环直到游戏结束
	void RunLoop();

	// 关闭游戏
	void Shutdown();

	// 墙的厚度
	int wallThickness;
private:
	// 游戏循环的辅助函数
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	// SDL创建的窗口
	SDL_Window* mWindow;
	// 游戏应该继续吗？
	bool mIsRunning;
	// 渲染器
	SDL_Renderer* mRenderer;

	// 玩家
	Player player1;
	// 球的位置
	Vector2 mBallPos;
	// 球的速度（速率和方向）
	Vector2 mBallVel;
	// 球的厚度
	int ballThickness;

	// 用于计算增量时间（delta time）
	Uint32 mTicksCount;
};