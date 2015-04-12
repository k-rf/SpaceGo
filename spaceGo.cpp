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

	// 背景のテクスチャ
	if(!spaceTexture.initialize(graphics, BACK_GROUND))
		throw(GameError(gameErrorNS::WARNING, "Error initializing space texture"));

	// 背景
	if(!space.initialize(graphics, 0, 0, 0, &spaceTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing space"));

	space.setX(GAME_WIDTH * 0.5f - space.getWidth() * 0.5f * space.getScale());
	space.setFrameDelay(30);

	// 宇宙船のテクスチャ
	if(!shipTexture.initialize(graphics, CHARA_MAP))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ship texture"));

	// 宇宙船
	if(!ship.initialize(this, shipNS::WIDTH, shipNS::HEIGHT, shipNS::TEXTURE_COLS, &shipTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ship"));

	// 宇宙船を配置
	ship.setX(GAME_WIDTH * 0.5f - ship.getWidth() * 0.5f * ship.getScale());
	ship.setY(GAME_HEIGHT * 0.5f - ship.getWidth() * 0.5f * ship.getScale());
	//ship.setFrames(shipNS::SHIP_START_FRAME, shipNS::SHIP_END_FRAME);  
	ship.setCurrentFrame(ship.getCurrentFrame());
	ship.setFrameDelay(shipNS::SHIP_ANIMATION_DELAY);

	return;
}

// ==================================================================
// すべてのゲームアイテムを更新
// ==================================================================
void SpaceGo::update()
{
	space.setFrameDelay(space.getFrameDelay() + 0.01f);
	space.setY(space.getY() + frameTime * space.getFrameDelay());
	
	ship.update(frameTime);
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
	float y = space.getY();
	if(y > space.getHeight())
		y = 0;

	graphics->spriteBegin();  // スプライトの描画を開始
	
	space.draw();
	if(space.getY() + space.getHeight() < GAME_HEIGHT)
	{
		space.setY(space.getY() + space.getHeight());
		space.draw();
		space.setY(y);
	}
	if(space.getY() > 0)
	{
		space.setY(space.getY() - space.getHeight());
		space.draw();
		space.setY(y);
	}
	
	ship.draw();
	graphics->spriteEnd();    // スプライトの描画を終了
}

// ==================================================================
// グラフィックスデバイスが消失した場合
// グラフィックスデバイスをリセット可能にするため、
// 予約されていたビデオメモリをすべて解放
// ==================================================================
void SpaceGo::releaseAll()
{
	spaceTexture.onLostDevice();
	shipTexture.onLostDevice();
	Game::releaseAll();
	return;
}

// ==================================================================
// グラフィックスデバイスがリセットされた場合
// すべてのサーフェイスを再作成
// ==================================================================
void SpaceGo::resetAll()
{
	spaceTexture.onResetDevice();
	shipTexture.onResetDevice();
	Game::resetAll();
	return;
}
