#ifndef _SPACEGO_H
#define _SPACEGO_H

#define WIN32_LEAN_AND_MEAN

#include "game.h"
#include "textureManager.h"
#include "image.h"
#include "ship.h"

// ==================================================================
// このクラスはゲームエンジンのコア
// ==================================================================
class SpaceGo : public Game
{
	// ゲームアイテム
	TextureManager shipTexture;
	TextureManager spaceTexture;
	Ship ship;
	Image space;

public:
	// コンストラクタ
	SpaceGo();

	// デストラクタ
	virtual ~SpaceGo();

	// ゲームを初期化
	void initialize(HWND hwnd);
	void update();      // Gameからの純粋仮想関数をオーバーライドする必要がある
	void ai();          // 〃
	void collisions();  // 〃
	void render();      // 〃
	void releaseAll();
	void resetAll();
};

#endif