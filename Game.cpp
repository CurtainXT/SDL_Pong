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

// ��Ϸ��ʼ��
bool Game::Initialize()
{
	// ��SDL��Ƶ��ϵͳ���г�ʼ��
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	// ��������
	mWindow = SDL_CreateWindow(
		"Pong",    // ���ڱ���
		100,       // �������Ͻ�����Ļ�е�x������
		100,       // �������Ͻ�����Ļ�е�y������
		1024,      // ���ڿ��
		768,       // ���ڳ���
		0          // ��־λ
		);
	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	// ������Ⱦ��
	mRenderer = SDL_CreateRenderer(
		mWindow,    // ʹ����Ⱦ���Ĵ���
		-1,         // ����ָ��ʹ����һ����������-1��ʾʹ�õ�һ��֧�������־������
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);  // ʹ��ͼ��Ӳ�� | ���ô�ֱͬ��
	if (!mRenderer)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	return true;
}

// ��Ϸѭ��
void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

// ��������
void Game::ProcessInput()
{
	SDL_Event event;
	// ���¼������л����¼�ʱ
	while (SDL_PollEvent(&event))
	{
		// ����ͬ�¼����͵��¼�
		switch (event.type)
		{
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}

	// �õ����̰���״̬
	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE])// �������ESC����ͬ��ֹͣѭ��
	{
		mIsRunning = false;
	}
	player1.mPaddleDir = 0;
	if (state[SDL_SCANCODE_W])
	{
		player1.mPaddleDir -= 1; // ʹ�üӼ���Ϊ�˱�֤���ͬʱ����W Sʱ�õ�0
	}
	if (state[SDL_SCANCODE_S])
	{
		player1.mPaddleDir += 1;
	}


}

// ������Ϸ
void Game::UpdateGame()
{
	// �ȴ�����һ֡��ʼ��16�������ţ���֤��Լ60֡�ĸ���Ƶ�ʣ�
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;// ���û�дﵽ16���룬�Ͳ�ִ������ĸ���
	
	// ��������ʱ��
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	// ���浱ǰ֡��ticks����һ֡ʹ��
	mTicksCount = SDL_GetTicks();

	// ����deltaTime�����ֵΪ0.05
	if(deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}

	// �ƶ�����
	if (player1.mPaddleDir != 0)
	{
		player1.mPaddlePos.y += player1.mPaddleDir * 360.0f * deltaTime;
		// ��ֹ�����Ƴ���Ļ
		if (player1.mPaddlePos.y < (player1.mPaddleH / 2.0f + wallThickness))
		{
			player1.mPaddlePos.y = player1.mPaddleH / 2.0f + wallThickness;
		}
		else if (player1.mPaddlePos.y > (768.0f - player1.mPaddleH / 2.0f - wallThickness))
		{
			player1.mPaddlePos.y = 768.0f - player1.mPaddleH / 2.0f - wallThickness;
		}
	}

	// �ƶ���
	mBallPos.x += mBallVel.x * deltaTime;// ��������x y���������ϵ�λ��
	mBallPos.y += mBallVel.y * deltaTime;
	// �����ײ���
	if (mBallPos.y <= wallThickness + ballThickness / 2.0f && mBallVel.y < 0.0f)// ײ�������ǽ
	{
		mBallVel.y *= -1;
	}
	if (mBallPos.y >= 768.0f - wallThickness - ballThickness / 2.0f && mBallVel.y > 0.0f)// ײ�������ǽ
	{
		mBallVel.y *= -1;
	}
	if (mBallPos.x >= 1024.0f - wallThickness - ballThickness / 2.0f && mBallVel.x > 0.0f)// ײ���ұߵ�ǽ
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

// �������
void Game::GenerateOutput()
{

	// ��̨�������Ϊ��ǰ����Ƶ���ɫ
	SDL_SetRenderDrawColor(mRenderer, 50, 50, 50, 255);// ��Ҫ���Ƶ���ɫ����Ϊ��ɫ
	SDL_RenderClear(mRenderer);

	// ����3��������Ϊǽ��
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);// ��Ҫ���Ƶ���ɫ�糤Ϊ����ɫ
	SDL_Rect wall1 = {//ͨ��SDL_Rect�ṹ������ʾһ������
		0,	      // ���Ͻ�����Ļ�е�x������
		0,        // ���Ͻ�����Ļ�е�y������
		1024,     // ����
		wallThickness // ���
	};
	SDL_Rect wall2 = {//ͨ��SDL_Rect�ṹ������ʾһ������
		1024 - wallThickness,	      // ���Ͻ�����Ļ�е�x������
		0,        // ���Ͻ�����Ļ�е�y������
		wallThickness,     // ����
		768 // ���
	};
	SDL_Rect wall3 = {//ͨ��SDL_Rect�ṹ������ʾһ������
		0,	      // ���Ͻ�����Ļ�е�x������
		768 - wallThickness,        // ���Ͻ�����Ļ�е�y������
		1024,     // ����
		wallThickness // ���
	};
	SDL_RenderFillRect(mRenderer, &wall1);// ����wall1
	SDL_RenderFillRect(mRenderer, &wall2);// ����wall2
	SDL_RenderFillRect(mRenderer, &wall3);// ����wall3

	// ������
	SDL_Rect ball = {
	static_cast<int>(mBallPos.x - ballThickness / 2),
	static_cast<int>(mBallPos.y - ballThickness / 2),
	ballThickness,
	ballThickness
	};
	SDL_RenderFillRect(mRenderer, &ball);

	// ��������
	SDL_Rect paddle = {
	static_cast<int>(player1.mPaddlePos.x - 7),
	static_cast<int>(player1.mPaddlePos.y - 40),
	player1.mThickness,
	player1.mPaddleH
	};
	SDL_RenderFillRect(mRenderer, &paddle);

	// ����ǰ̨�������ͺ�̨����������Ⱦ��ǰ��
	SDL_RenderPresent(mRenderer);
}

// �ر���Ϸ
void Game::Shutdown()
{
	// ����mWindow����
	SDL_DestroyWindow(mWindow);
	// ������Ⱦ��
	SDL_DestroyRenderer(mRenderer);
	// �ر�SDL��
	SDL_Quit();
}