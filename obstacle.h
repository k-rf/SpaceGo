#ifndef _OBSTACLE_H
#define _OBSTACLE_H

#define WIN32_LEAN_AND_MEAN

#include "entity.h"
#include "constants.h"

namespace obstacleNS
{
	const int WIDTH = 32;                        // 幅
	const int HEIGHT = 32;                       // 高さ
	const int X = GAME_WIDTH / 2 - WIDTH / 2;    // スタート位置
	const int Y = GAME_HEIGHT / 2 - HEIGHT / 2;
	const float SPEED = 0;                       // １００ピクセル毎秒
	const float MASS = 300.0f;                   // 質量
	const int TEXTURE_COLS = 29;
	const int OBSTACLE_START_FRAME = 0;              // Shipの最初の画像
	const int OBSTACLE_END_FRAME = 0;                // Shipアニメーションのフレーム数
	const float OBSTACLE_ANIMATION_DELAY = 0.2F;     // フレーム間の時間
}

class Obstacle : public Entity
{
public:
	// コンストラクタ
	Obstacle();

	virtual ~Obstacle() {}

	void update(float frameTime, float bg);
};

#endif
