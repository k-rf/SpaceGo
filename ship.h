#ifndef _SHIP_H
#define _SHIP_H

#define WIN32_LEAN_AND_MEAN

#include "entity.h"
#include "constants.h"

namespace shipNS
{
	const int WIDTH = 32;                        // 幅
	const int HEIGHT = 32;                       // 高さ
	const int X = GAME_WIDTH / 2 - WIDTH / 2;    // スタート位置
	const int Y = GAME_HEIGHT / 2 - HEIGHT / 2;
	const float SPEED = 2.0f;                     // １００ピクセル毎秒
	const float MAX_SPEED = 10;
	const float MASS = 300.0f;                   // 質量
	const int TEXTURE_COLS = 8;
	const int SHIP_START_FRAME = 0;              // Shipの最初の画像
	const int SHIP_END_FRAME = 4;                // Shipアニメーションのフレーム数
	const float SHIP_ANIMATION_DELAY = 0.2F;     // フレーム間の時間
}

class Ship : public Entity
{
public:
	// コンストラクタ
	Ship();

	void update(float frameTime);
};

#endif