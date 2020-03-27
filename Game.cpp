#include"Game.h"

Player::Player() :
	mPaddlePos({ 30.0f, (int)(768 / 2) }),
	mPaddleDir(0),
	mPaddleH(80),
	mThickness(10)
{

}

Game::Game() :
	mWindow(NULL),
	mIsRunning(true),
	mRenderer(NULL),
	player1(),
	mBallPos({ (int)(1024 / 2), (int)(768 / 2) }),
	mTicksCount(0),
	wallThickness(15),
	mBallVel({ -200.0f, 235.0f }),
	ballThickness(10)
{

}

// 游戏初始化
bool Game::Initialize()
{
	// 对SDL视频子系统进行初始化
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	// 创建窗口
	mWindow = SDL_CreateWindow(
		"Pong",    // 窗口标题
		100,       // 窗口左上角在屏幕中的x轴坐标
		100,       // 窗口左上角在屏幕中的y轴坐标
		1024,      // 窗口宽度
		768,       // 窗口长度
		0          // 标志位
		);
	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	// 创建渲染器
	mRenderer = SDL_CreateRenderer(
		mWindow,    // 使用渲染器的窗口
		-1,         // 用于指定使用哪一个驱动程序，-1表示使用第一个支持所需标志的驱动
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);  // 使用图形硬件 | 启用垂直同步
	if (!mRenderer)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	return true;
}

// 游戏循环
void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

// 处理输入
void Game::ProcessInput()
{
	SDL_Event event;
	// 当事件队列中还有事件时
	while (SDL_PollEvent(&event))
	{
		// 处理不同事件类型的事件
		switch (event.type)
		{
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}

	// 得到键盘按键状态
	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE])// 如果按下ESC键，同样停止循环
	{
		mIsRunning = false;
	}
	player1.mPaddleDir = 0;
	if (state[SDL_SCANCODE_W])
	{
		player1.mPaddleDir -= 1; // 使用加减是为了保证玩家同时按下W S时得到0
	}
	if (state[SDL_SCANCODE_S])
	{
		player1.mPaddleDir += 1;
	}


}

// 更新游戏
void Game::UpdateGame()
{
	// 等待从上一帧开始的16毫秒流逝（保证大约60帧的更新频率）
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;// 如果没有达到16毫秒，就不执行下面的更新
	
	// 计算增量时间
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	// 保存当前帧的ticks供下一帧使用
	mTicksCount = SDL_GetTicks();

	// 限制deltaTime的最大值为0.05
	if(deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}

	// 移动球拍
	if (player1.mPaddleDir != 0)
	{
		player1.mPaddlePos.y += player1.mPaddleDir * 360.0f * deltaTime;
		// 防止球拍移出屏幕
		if (player1.mPaddlePos.y < (player1.mPaddleH / 2.0f + wallThickness))
		{
			player1.mPaddlePos.y = player1.mPaddleH / 2.0f + wallThickness;
		}
		else if (player1.mPaddlePos.y > (768.0f - player1.mPaddleH / 2.0f - wallThickness))
		{
			player1.mPaddlePos.y = 768.0f - player1.mPaddleH / 2.0f - wallThickness;
		}
	}

	// 移动球
	mBallPos.x += mBallVel.x * deltaTime;// 更新球在x y两个方向上的位置
	mBallPos.y += mBallVel.y * deltaTime;
	// 球的碰撞检测
	if (mBallPos.y <= wallThickness + ballThickness / 2.0f && mBallVel.y < 0.0f)// 撞到上面的墙
	{
		mBallVel.y *= -1;
	}
	if (mBallPos.y >= 768.0f - wallThickness - ballThickness / 2.0f && mBallVel.y > 0.0f)// 撞到下面的墙
	{
		mBallVel.y *= -1;
	}
	if (mBallPos.x >= 1024.0f - wallThickness - ballThickness / 2.0f && mBallVel.x > 0.0f)// 撞到右边的墙
	{
		mBallVel.x *= -1;
	}
	if (mBallPos.x <= 30.0f && mBallPos.x >= 20.f &&
		mBallPos.y <= player1.mPaddlePos.y + player1.mPaddleH / 2.0f &&
		mBallPos.y >= player1.mPaddlePos.y - player1.mPaddleH / 2.0f &&
		mBallVel.x < 0.0f)
	{
		mBallVel.x *= -1;
	}
}

// 产生输出
void Game::GenerateOutput()
{

	// 后台缓存清除为当前需绘制的颜色
	SDL_SetRenderDrawColor(mRenderer, 50, 50, 50, 255);// 将要绘制的颜色设置为灰色
	SDL_RenderClear(mRenderer);

	// 绘制3个矩形作为墙壁
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);// 将要绘制的颜色社长为纯白色
	SDL_Rect wall1 = {//通过SDL_Rect结构体来表示一个矩形
		0,	      // 左上角在屏幕中的x轴坐标
		0,        // 左上角在屏幕中的y轴坐标
		1024,     // 长度
		wallThickness // 厚度
	};
	SDL_Rect wall2 = {//通过SDL_Rect结构体来表示一个矩形
		1024 - wallThickness,	      // 左上角在屏幕中的x轴坐标
		0,        // 左上角在屏幕中的y轴坐标
		wallThickness,     // 长度
		768 // 厚度
	};
	SDL_Rect wall3 = {//通过SDL_Rect结构体来表示一个矩形
		0,	      // 左上角在屏幕中的x轴坐标
		768 - wallThickness,        // 左上角在屏幕中的y轴坐标
		1024,     // 长度
		wallThickness // 厚度
	};
	SDL_RenderFillRect(mRenderer, &wall1);// 绘制wall1
	SDL_RenderFillRect(mRenderer, &wall2);// 绘制wall2
	SDL_RenderFillRect(mRenderer, &wall3);// 绘制wall3

	// 绘制球
	SDL_Rect ball = {
	static_cast<int>(mBallPos.x - ballThickness / 2),
	static_cast<int>(mBallPos.y - ballThickness / 2),
	ballThickness,
	ballThickness
	};
	SDL_RenderFillRect(mRenderer, &ball);

	// 绘制球拍
	SDL_Rect paddle = {
	static_cast<int>(player1.mPaddlePos.x - 7),
	static_cast<int>(player1.mPaddlePos.y - 40),
	player1.mThickness,
	player1.mPaddleH
	};
	SDL_RenderFillRect(mRenderer, &paddle);

	// 互换前台缓冲区和后台缓冲区，渲染当前的
	SDL_RenderPresent(mRenderer);
}

// 关闭游戏
void Game::Shutdown()
{
	// 销毁mWindow对象
	SDL_DestroyWindow(mWindow);
	// 销毁渲染器
	SDL_DestroyRenderer(mRenderer);
	// 关闭SDL库
	SDL_Quit();
}