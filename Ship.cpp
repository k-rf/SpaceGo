#include "ship.h"

// ============================================================================
// デフォルトコンストラクタ
// ============================================================================
Ship::Ship() : Entity()
{
	spriteData.width = shipNS::WIDTH;
	spriteData.width = shipNS::HEIGHT;
	spriteData.x = shipNS::X;
	spriteData.y = shipNS::Y;
	spriteData.rect.bottom = shipNS::HEIGHT;
	spriteData.rect.right = shipNS::WIDTH;
	velocity.x = 0;
	velocity.y = 0;
	frameDelay = shipNS::SHIP_ANIMATION_DELAY;
	startFrame = shipNS::SHIP_START_FRAME;
	endFrame = shipNS::SHIP_END_FRAME;
	currentFrame = startFrame + 2;
	radius = 0.1f;  // 当たり判定を小さく
	collisionType = entityNS::CIRCLE;
}

// ============================================================================
// update
// 通常、フレームごとに１回呼び出される
// ============================================================================
void Ship::update(float frameTime)
{
	if(velocity.x < -0.5)
		currentFrame = 1;
	else if(velocity.x > 0.5)
		currentFrame = 3;
	else
		currentFrame = 2;

	// 上を入力した時
	if(input->isKeyDown(UP_KEY))
	{
		velocity.y += frameTime * -shipNS::SPEED;

		if(velocity.y < -shipNS::MAX_SPEED)
			velocity.y = shipNS::MAX_SPEED;
	}

	if(getY() < GAME_HEIGHT - getHeight())
		setY(getY() + velocity.y);
	else
	{
		setY(GAME_HEIGHT - getHeight() - 1);
		velocity.y = 0;
	}

	// 下を入力した時
	if(input->isKeyDown(DOWN_KEY))
	{
		velocity.y += frameTime * shipNS::SPEED;

		if(velocity.y < -shipNS::MAX_SPEED)
			velocity.y = -shipNS::MAX_SPEED;
	}

	if(getY() > 0)
		setY(getY() + velocity.y);
	else
	{
		setY(1);
		velocity.y = 0;
	}

	// 左を入力した時
	if(input->isKeyDown(LEFT_KEY))
	{
		velocity.x += frameTime * -shipNS::SPEED;

		currentFrame = 1;
		if(velocity.x < 0.3f)
			currentFrame = 0;

		if(velocity.x < -shipNS::MAX_SPEED)
			velocity.x = -shipNS::MAX_SPEED;
	}

	if(getX() > 0)
		setX(getX() + velocity.x);
	else
	{
		setX(1);
		velocity.x = 0;
	}

	// 右を入力した時
	if(input->isKeyDown(RIGHT_KEY))
	{
		velocity.x += frameTime * shipNS::SPEED;

		currentFrame = 3;
		if(velocity.x > -0.3f)
			currentFrame = 4;

		if(velocity.x > shipNS::MAX_SPEED)
			velocity.x = shipNS::MAX_SPEED;
	}


	if(getX() < GAME_WIDTH - getWidth())
		setX(getX() + velocity.x);
	else
	{
		setX(GAME_WIDTH - getWidth() - 1);
		velocity.x = 0;
	}

	Entity::update(frameTime);
}