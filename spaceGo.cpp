#include "spaceGo.h"

// ==================================================================
// コンストラクタ
// ==================================================================
SpaceGo::SpaceGo()
{}

// ==================================================================
// デストラクタ
// ==================================================================
SpaceGo::~SpaceGo()
{
	releaseAll();  // すべてのグラフィックスアイテムについてonLostDevice()を呼び出す
}

// ==================================================================
// ゲームを初期化
// エラー時にGameErrorをスロー
// ==================================================================
void SpaceGo::initialize(HWND hwnd)
{
	Game::initialize(hwnd);  // GameErrorをスロー

	// ボールのテクスチャ
	if(!ballTexture.initialize(graphics, BALL_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ball texture"));

	// ボール
	if(!ball.initialize(graphics, 0, 0, 0, &ballTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ball"));

	// ボールを配置
	ball.setX(GAME_WIDTH * 0.5f - ball.getWidth() * 0.5f * ball.getScale());
	ball.setY(GAME_HEIGHT * 0.5f - ball.getWidth() * 0.5f * ball.getScale());
	ball.setframes(0, 3);
	ball.setCurrentFrame(0);
	ball.setFrameDelay(0.2f);

	return;
}

// ==================================================================
// すべてのゲームアイテムを更新
// ==================================================================
void SpaceGo::update()
{
	static float t;
	t += frameTime;

	ball.setY(t * t * GRAVITY / 2);
	ball.setRadians(10 * t);
	if(ball.getY() > GAME_HEIGHT - ball.getHeight() * ball.getScale())
	{
		ball.setY((float)-ball.getHeight());
		t = 0;
	}

	if(input->isKeyDown(LEFT_KEY))
	{
		ball.setX(ball.getX() - frameTime * BALL_SPEED);
		if(ball.getX() < -ball.getWidth() * ball.getScale())
			ball.setX((float)GAME_WIDTH);
	}

	if(input->isKeyDown(RIGHT_KEY))
	{
		ball.setX(ball.getX() + frameTime * BALL_SPEED);
		if(ball.getX() > GAME_WIDTH)
			ball.setX((float)-ball.getWidth() * ball.getScale());
	}

	ball.update(frameTime);
}

// ==================================================================
// 人工知能
// ==================================================================
void SpaceGo::ai()
{}

// ==================================================================
// 衝突を処理
// ==================================================================
void SpaceGo::collisions()
{}

// ==================================================================
// ゲームアイテムをレンダー
// ==================================================================
void SpaceGo::render()
{
	graphics->spriteBegin();  // スプライトの描画を開始
	ball.draw();
	graphics->spriteEnd();    // スプライトの描画を終了
}

// ==================================================================
// グラフィックスデバイスが消失した場合
// グラフィックスデバイスをリセット可能にするため、
// 予約されていたビデオメモリをすべて解放
// ==================================================================
void SpaceGo::releaseAll()
{
	ballTexture.onLostDevice();
	Game::releaseAll();
	return;
}

// ==================================================================
// グラフィックスデバイスがリセットされた場合
// すべてのサーフェイスを再作成
// ==================================================================
void SpaceGo::resetAll()
{
	ballTexture.onResetDevice();
	Game::resetAll();
	return;
}
