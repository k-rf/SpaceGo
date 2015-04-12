#include <ctime>
#include "obstacle.h"

// ============================================================================
// デフォルトコンストラクタ
// ============================================================================
Obstacle::Obstacle() : Entity()
{
	spriteData.width = obstacleNS::WIDTH;
	spriteData.width = obstacleNS::HEIGHT;
	spriteData.x = obstacleNS::X;
	spriteData.y = obstacleNS::Y;
	spriteData.rect.bottom = obstacleNS::HEIGHT;
	spriteData.rect.right = obstacleNS::WIDTH;
	velocity.x = 0;
	velocity.y = 0;
	frameDelay = obstacleNS::OBSTACLE_ANIMATION_DELAY;
	startFrame = obstacleNS::OBSTACLE_START_FRAME;
	endFrame = obstacleNS::OBSTACLE_END_FRAME;
	currentFrame = startFrame;
	radius = obstacleNS::WIDTH / 2.0;
	collisionType = entityNS::CIRCLE;
	
}

// ============================================================================
// update
// 通常、フレームごとに１回呼び出される
// ============================================================================
void Obstacle::update(float frameTime, float bg)
{
	Entity::update(frameTime);

	setY(getY() + bg * frameTime);

	if(getY() > GAME_HEIGHT)
	{
		setY((rand() % (int)(GAME_HEIGHT / 6)));
		setX(rand() % (int)(GAME_WIDTH - getWidth()));
	}


}